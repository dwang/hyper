#pragma once

#include <inttypes.h>

typedef void *HANDLE, *PVOID;
typedef long NTSTATUS;

class u_process {
	const wchar_t *_name;
	HANDLE _handle;
	bool _wow64;
	uintptr_t _peb;
public:
	u_process() : _name(L"csgo.exe"), _handle(0) {}
	~u_process() { detach(); }

	bool attach();
	void detach();
	bool exists();
	uintptr_t find_module(const wchar_t *name);
	uintptr_t find_export(uintptr_t module, const char *name);
	NTSTATUS read(uintptr_t address, void *buffer, size_t length);
	NTSTATUS write(uintptr_t address, void *buffer, size_t length);
	uintptr_t find_pattern(const wchar_t* module_name, const char* pattern);
	uintptr_t get_module_size(const wchar_t* module_name);
	void execute(void* shellcode, void* parameter = nullptr);
	bool free_memory(void* address, size_t size = 0);
	void* allocate(size_t size);
	template<typename t>
	inline t read(uintptr_t address)
	{
		t v;
		read(address, &v, sizeof(t));
		return v;
	}
	template<typename t>
	inline t read_ptr(uintptr_t address)
	{
		return read<t>(read<uintptr_t>(address));
	}
	template<typename t>
	inline t read_ptr32(uintptr_t address)
	{
		return read<t>(read<uint32_t>(address));
	}

	template<typename t>
	inline void write(uintptr_t address, t value)
	{
		write(address, &value, sizeof(t));
	}
	template <typename t>
	inline void write_ptr(uintptr_t address, t value)
	{
		write<t>(read<uintptr_t>(address), value);
	}
	template <typename t>
	inline void write_ptr32(uintptr_t address, t value)
	{
		write<t>(read<uint32_t>(address), value);
	}
};

extern u_process* cs_process;
