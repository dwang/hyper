#include "u_process.hpp"
#include <string.h>

#pragma comment(lib, "ntdll.lib")

namespace windows {
#include <Windows.h>
#include <winnt.h>
#include <malloc.h>
#include <TlHelp32.h>
}

#ifdef _WIN64
#define SNAP_LENGTH 0x188
#define SNAP_NAME   0x040
#define TEB_WOW64   0x100
#define SNAP_PID    0x128
#define SNAP_START  0x160
#define SNAP_TEB    0x168
#else
#define SNAP_LENGTH 0x110
#define SNAP_NAME   0x03C
#define TEB_WOW64   0x0C0
#define SNAP_PID    0x0D8
#define SNAP_START  0x100
#define SNAP_TEB    0x104
#endif

#define IS_WOW64_ADDRESS(address) (address <= 0xffffffffUL)

#define INRANGE(x,a,b) (x >= a && x <= b)
#define GETBITS(x) (INRANGE(x, '0', '9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define GETBYTE(x) (GETBITS(x[0]) << 4 | GETBITS(x[1]))

struct process_entry {
	const wchar_t *name;
	int           pid;
	uintptr_t     start;
	uintptr_t     teb;
};

struct process_snap {
	char *first, *entry;
};

extern "C"
{
	typedef int BOOL;
	typedef void *voidptr_t;
	typedef long NTSTATUS;
#define NTAPI __stdcall
#define WINAPI __stdcall
#define DECLSPEC_IMPORT __declspec(dllimport)
	DECLSPEC_IMPORT NTSTATUS NTAPI  NtQuerySystemInformation(uint32_t, voidptr_t, uint32_t, uint32_t *);
	DECLSPEC_IMPORT NTSTATUS NTAPI  NtClose(HANDLE);
	DECLSPEC_IMPORT NTSTATUS NTAPI  NtReadVirtualMemory(HANDLE, PVOID, PVOID, size_t, size_t*);
	DECLSPEC_IMPORT NTSTATUS NTAPI  NtWriteVirtualMemory(HANDLE, PVOID, PVOID, size_t, size_t*);
	DECLSPEC_IMPORT HANDLE   WINAPI OpenProcess(uint32_t, uint32_t, uint32_t);
	DECLSPEC_IMPORT BOOL     WINAPI GetExitCodeProcess(HANDLE, uint32_t*);
}

static void *create_snapshot(void);
static void close_snapshot(void *snapshot);
static bool get_next_process(void *snapshot, struct process_entry *entry);
static uintptr_t teb_to_peb(HANDLE process, uintptr_t teb, bool wow64);

u_process* cs_process = new u_process();

bool u_process::attach(void)
{
	void *snap = create_snapshot();
	struct process_entry entry;

	if (snap == 0)
		return false;

	while (get_next_process(snap, &entry)) {
		if (!_wcsicmp(entry.name, _name)) {
			_handle = OpenProcess(0x1fffff, 0, entry.pid);
			_wow64 = IS_WOW64_ADDRESS(entry.start);
			_peb = teb_to_peb(_handle, entry.teb, _wow64);
			break;
		}
	}

	close_snapshot(snap);
	return _handle != 0;
}

void u_process::detach(void)
{
	if (_handle != 0) {
		NtClose(_handle);
	}
}

bool u_process::exists(void)
{
	uint32_t e;
	GetExitCodeProcess(_handle, &e);
	return e == 0x00000103;
}

uintptr_t u_process::find_module(const wchar_t *name)
{
	uintptr_t a0 = 0, a1 = 0, a2 = 0, a3[30];
	uint32_t  rly[5];

	if (_wow64) {
		rly[0] = 0x04, rly[1] = 0x0C, rly[2] = 0x14, rly[3] = 0x28, rly[4] = 0x10;
	}
	else {
		rly[0] = 0x08, rly[1] = 0x18, rly[2] = 0x20, rly[3] = 0x50, rly[4] = 0x20;
	}

	read(_peb + rly[1], &a0, rly[0]);
	read(a0 + rly[2], &a0, rly[0]);
	read(a0 + rly[0], &a1, rly[0]);
	while (a0 != a1) {
		read(a0 + rly[3], &a2, rly[0]);
		read(a2, &a3, sizeof(a3));
		if (_wcsicmp((const wchar_t*)a3, name) == 0) {
			read(a0 + rly[4], &a0, rly[0]);
			return a0;
		}
		if (read(a0, &a0, rly[0]) != 0l)
			break;
	}

	return 0;
}

uintptr_t u_process::find_export(uintptr_t module, const char *name)
{
	uintptr_t a0;
	uint32_t a1[4], a2[30];

	a0 = module + read<uint16_t>(module + 0x3C);
	a0 = module + read<uint32_t>(a0 + 0x88 - _wow64 * 16);
	read(a0 + 0x18, &a1, sizeof(a1));
	while (a1[0]--) {
		a0 = read<uint32_t>(module + a1[2] + (a1[0] * 4));
		read(module + a0, &a2, sizeof(a2));
		if (!_stricmp((const char *)a2, name)) {
			return (module + read<uint32_t>(module + a1[1] + (read<uint16_t>(module + a1[3] + (a1[0] * 2)) * 4)));
		}
	}
	return 0;
}

NTSTATUS u_process::read(uintptr_t address, void *buffer, size_t length)
{
	return NtReadVirtualMemory(_handle, (PVOID)address, buffer, length, 0);
}

NTSTATUS u_process::write(uintptr_t address, void *buffer, size_t length)
{
	return NtWriteVirtualMemory(_handle, (PVOID)address, buffer, length, 0);
}

uintptr_t u_process::find_pattern(const wchar_t* module_name, const char* pattern)
{
	auto mod = find_module(module_name);

	static auto compare_bytes = [](const windows::byte* bytes, const char* pattern) -> bool
	{
		for (; *pattern; *pattern != ' ' ? ++bytes : bytes, ++pattern) {
			if (*pattern == ' ' || *pattern == '?')
				continue;
			if (*bytes != GETBYTE(pattern))
				return false;
			++pattern;
		}
		return true;
	};

	auto get_text_section = [&](uintptr_t& start, size_t& size)
	{
		auto header = (windows::byte*)windows::malloc(0x1000);
		read(mod, header, 0x1000);

		auto pDosHdr = (const windows::IMAGE_DOS_HEADER*)(header);
		if (pDosHdr->e_magic != IMAGE_DOS_SIGNATURE)
		{
			::windows::free(header);
			return false;
		}
		const windows::IMAGE_NT_HEADERS *pImageHdr = (const windows::IMAGE_NT_HEADERS*)((uint8_t*)pDosHdr + pDosHdr->e_lfanew);

		if (pImageHdr->Signature != IMAGE_NT_SIGNATURE)
		{
			::windows::free(header);
			return false;
		}

		auto pSection = (const windows::IMAGE_SECTION_HEADER*)((uint8_t*)pImageHdr + sizeof(windows::IMAGE_NT_HEADERS));
		for (int i = 0; i < pImageHdr->FileHeader.NumberOfSections; ++i, pSection++)
		{
			if (_stricmp((windows::LPCSTR)pSection->Name, ".text") == 0)
			{
				start = (uintptr_t)(mod + pSection->VirtualAddress);
				size = pSection->Misc.VirtualSize;
				::windows::free(header);
				return true;
			}
		}

		::windows::free(header);
		return false;
	};

	uintptr_t base; size_t size;
	if (!get_text_section(base, size))
	{
		base = mod;
		size = get_module_size(module_name);
	}

	auto pb = (windows::byte*)windows::malloc(size);
	auto max = size;
	read(base, pb, size);

	uintptr_t offset = 0;
	for (auto off = 0UL; off < max; ++off) {
		if (compare_bytes(pb + off, pattern)) {
			offset = base + off;
			break;
		}
	}

	return offset;
}

uintptr_t u_process::get_module_size(const wchar_t* module_name)
{
	windows::HMODULE hModule = windows::GetModuleHandle((char*)module_name);

	if (!hModule)
		return 0;

	windows::PIMAGE_DOS_HEADER pImage_Dos_Header = windows::PIMAGE_DOS_HEADER(hModule);
	windows::PIMAGE_NT_HEADERS pImage_PE_Header = windows::PIMAGE_NT_HEADERS(long(hModule) + pImage_Dos_Header->e_lfanew);
	windows::PIMAGE_OPTIONAL_HEADER pImage_Optional_Header = &pImage_PE_Header->OptionalHeader;

	return pImage_Optional_Header->SizeOfCode;
}

bool u_process::free_memory(void* address, size_t size) {
	return windows::VirtualFreeEx(_handle, address, size, MEM_RELEASE);
}
void* u_process::allocate(size_t size) {
	return windows::VirtualAllocEx(_handle, 0, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
}

void u_process::execute(void* shellcode, void* parameter) {
	auto execute_thread = windows::CreateRemoteThread(_handle, nullptr, NULL, static_cast<windows::LPTHREAD_START_ROUTINE>(shellcode), parameter, NULL, nullptr);
	windows::WaitForSingleObject(execute_thread, INFINITE);
	windows::CloseHandle(execute_thread);
}

static void *create_snapshot()
{
	struct process_snap *snap = new process_snap;
	uint32_t length = 0;
	NTSTATUS status;


	status = NtQuerySystemInformation(
		57,
		&length,
		SNAP_LENGTH,
		&length
	);

	if (status != 0xC0000004L) {
		delete[] snap;
		return 0;
	}

	length += 4096;
	snap->first = new char[length];
	status = NtQuerySystemInformation(
		57,
		snap->first,
		length,
		&length
	);

	if (status != 0L) {
		delete[] snap->first;
		delete[] snap;
		return 0;
	}
	else {
		snap->entry = snap->first;
	}
	return snap;
}

static void close_snapshot(void *snapshot)
{
	struct process_snap *snap = (struct process_snap *)snapshot;

	delete[] snap->first;
	delete[] snap;
}

static bool get_next_process(void *snapshot, struct process_entry *entry)
{
	struct process_snap *snap = (struct process_snap *)snapshot;

	if (*(uint32_t*)snap->entry == 0)
		return false;
	snap->entry = snap->entry + *(uint32_t*)snap->entry;
	entry->name = *(const wchar_t**)(snap->entry + SNAP_NAME);
	entry->pid = *(int*)(snap->entry + SNAP_PID);
	entry->start = *(uintptr_t*)(snap->entry + SNAP_START);
	entry->teb = *(uintptr_t*)(snap->entry + SNAP_TEB);
	return true;
}

static uintptr_t teb_to_peb(HANDLE process, uintptr_t teb, bool wow64)
{
	uintptr_t peb = 0;
	int length, offset;

	if (wow64) {
		length = 4, offset = 0x2030;
	}
	else {
		length = 8, offset = 0x60;
	}
	NtReadVirtualMemory(process, (PVOID)((char*)teb + offset), &peb, length, 0);
	return peb;
}
