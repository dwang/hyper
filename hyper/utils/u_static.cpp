#include "u_static.hpp"
#include "u_process.hpp"
#include "../sdk/cs_netvars.hpp"
#include "../sdk/cs_interfaces.hpp"
#include "../sdk/cs_convar.hpp"
#include "../sdk/cs_player.hpp"

#include <cstdio>

u_static* cs_static = new u_static();

void u_static::initialize_nv()
{
	static auto find_ptr = [](const wchar_t* module_name, const char* sig, uintptr_t sig_add, uintptr_t off_add, bool sub_base = true) -> uintptr_t
	{
		auto mod = cs_process->find_module(module_name);
		auto off = cs_process->find_pattern(module_name, sig);
		auto sb = sub_base ? mod : 0;
		off = cs_process->read<uintptr_t>(off + sig_add);

		return (!off ? 0 : off + off_add - sb);
	};

	cs_netvar_table t;

	dwEntityList = offset_entitylist();
	dwClientState = offset_clientstate();
	m_dwGetLocalPlayer = cs_process->read<uint32_t>(vt_engine.function(12) + 0x16);
	m_dwViewAngles = cs_process->read<uint32_t>(vt_engine.function(19) + 0xB2);
	m_dwMaxClients = cs_process->read<uint32_t>(vt_engine.function(20) + 0x07);
	m_dwState = cs_process->read<uint32_t>(vt_engine.function(26) + 0x07);
	m_dwButton = cs_process->read<uint32_t>(vt_inputsystem.function(15) + 0x21D);
	m_dwAnalogDelta = cs_process->read<uint32_t>(vt_inputsystem.function(18) + 0x29);
	m_dwAnalog = cs_process->read<uint32_t>(vt_inputsystem.function(18) + 0x09);
	m_dwLocalPlayer = find_ptr(L"client_panorama.dll", "8D 34 85 ? ? ? ? 89 15 ? ? ? ? 8B 41 08 8B 48 04 83 F9 FF", 0x3, 0x4);
	m_dwClientState = find_ptr(L"engine.dll", "A1 ? ? ? ? 33 D2 6A 00 6A 00 33 C9 89 B0", 0x1, 0);
	m_dwGlowObjectManager = find_ptr(L"client_panorama.dll", "A1 ? ? ? ? A8 01 75 4B", 0x1, 0x4);
	m_dwForceJump = find_ptr(L"client_panorama.dll", "89 0D ? ? ? ? 8B 0D ? ? ? ? 8B F2 8B C1 83 CE 08", 0x2, 0);
	t = netvars::find("DT_BasePlayer");
	m_iHealth = t.offset("m_iHealth");
	m_vecViewOffset = t.offset("m_vecViewOffset[0]");
	m_lifeState = t.offset("m_lifeState");
	m_nTickBase = t.offset("m_nTickBase");
	m_vecVelocity = t.offset("m_vecVelocity[0]");
	m_vecPunch = t.offset("m_Local") + 0x70;
	m_iFOV = t.offset("m_iFOV");
	m_fFlags = t.offset("m_fFlags");
	t = netvars::find("DT_BaseEntity");
	m_iTeamNum = t.offset("m_iTeamNum");
	m_vecOrigin = t.offset("m_vecOrigin");
	t = netvars::find("DT_CSPlayer");
	m_hActiveWeapon = t.offset("m_hActiveWeapon");
	m_iShotsFired = t.offset("m_iShotsFired");
	m_bIsScoped = t.offset("m_bIsScoped");
	m_flFlashDuration = t.offset("m_flFlashDuration");
	t = netvars::find("DT_BaseAnimating");
	m_dwBoneMatrix = t.offset("m_nForceBone") + 0x1C;
}

void u_static::initialize_vt()
{
	cs_interface_reg t;

	t = interfaces::find(L"client_panorama.dll");
	vt_client = t.find("VClient");
	vt_entity = t.find("VClientEntityList");
	t = interfaces::find(L"engine.dll");
	vt_engine = t.find("VEngineClient");
	t = interfaces::find(L"vstdlib.dll");
	vt_cvar = t.find("VEngineCvar");
	t = interfaces::find(L"inputsystem.dll");
	vt_inputsystem = t.find("InputSystemVersion");

	client_dll = cs_process->find_module(L"client_panorama.dll");
	engine_dll = cs_process->find_module(L"engine.dll");
}

csptr_t u_static::offset_entitylist()
{
	return vt_entity.address() - (cs_process->read<uint32_t>(vt_entity.function(5) + 0x22) - 0x38);
}

csptr_t u_static::offset_clientstate()
{
	return cs_process->read_ptr32<csptr_t>(vt_engine.function(18) + 0x16);
}

bool u_static::initialize()
{
	if (!cs_process->attach()) {
		return false;
	}

	initialize_vt();
	initialize_nv();

	printf(
		"[*]vtables:\n"
		"    VClient:                         0x%lx\n"
		"    VClientEntityList:               0x%lx\n"
		"    VEngineClient:                   0x%lx\n"
		"    VEngineCvar:                     0x%lx\n"
		"    InputSystemVersion:              0x%lx\n"
		"[*]offsets:\n"
		"    dwEntityList:                    0x%lx\n"
		"    dwClientState:                   0x%lx\n"
		"    m_dwGetLocalPlayer:              0x%x\n"
		"    m_dwViewAngles:                  0x%x\n"
		"    m_dwMaxClients:                  0x%x\n"
		"    m_dwState:                       0x%x\n"
		"    m_dwButton:                      0x%x\n"
		"    m_dwAnalog:                      0x%x\n"
		"    m_dwAnalogDelta:                 0x%x\n"
		"    m_dwLocalPlayer:                 0x%x\n"
		"    m_dwClientState:                 0x%x\n"
		"    m_dwGlowObjectManager:           0x%x\n"
		"    m_dwForceJump:                   0x%x\n"
		"[*]netvars:\n"
		"    DT_BasePlayer:                   m_iHealth:           0x%x\n"
		"    DT_BasePlayer:                   m_vecViewOffset:     0x%x\n"
		"    DT_BasePlayer:                   m_lifeState:         0x%x\n"
		"    DT_BasePlayer:                   m_nTickBase:         0x%x\n"
		"    DT_BasePlayer:                   m_vecVelocity:       0x%x\n"
		"    DT_BasePlayer:                   m_vecPunch:          0x%x\n"
		"    DT_BasePlayer:                   m_iFOV:              0x%x\n"
		"    DT_BasePlayer:                   m_fFlags:            0x%x\n"
		"    DT_BaseEntity:                   m_iTeamNum:          0x%x\n"
		"    DT_BaseEntity:                   m_vecOrigin:         0x%x\n"
		"    DT_CSPlayer:                     m_hActiveWeapon:     0x%x\n"
		"    DT_CSPlayer:                     m_iShotsFired:       0x%x\n"
		"    DT_CSPlayer:                     m_bIsScoped:         0x%x\n"
		"    DT_CSPlayer:                     m_flFlashDuration:   0x%x\n"
		"    DT_BaseAnimating:                m_dwBoneMatrix:      0x%x\n"
		"[*]convars:\n"
		"    sensitivity:                     %ff\n"
		"    volume:                          %ff\n"
		"    cl_crosshairsize:                %ff\n"
		,
		vt_client.address(),
		vt_entity.address(),
		vt_engine.address(),
		vt_cvar.address(),
		vt_inputsystem.address(),
		dwEntityList,
		dwClientState,
		m_dwGetLocalPlayer,
		m_dwViewAngles,
		m_dwMaxClients,
		m_dwState,
		m_dwButton,
		m_dwAnalog,
		m_dwAnalogDelta,
		m_dwLocalPlayer,
		m_dwClientState,
		m_dwGlowObjectManager,
		m_dwForceJump,
		m_iHealth,
		m_vecViewOffset,
		m_lifeState,
		m_nTickBase,
		m_vecVelocity,
		m_vecPunch,
		m_iFOV,
		m_fFlags,
		m_iTeamNum,
		m_vecOrigin,
		m_hActiveWeapon,
		m_iShotsFired,
		m_bIsScoped,
		m_flFlashDuration,
		m_dwBoneMatrix,
		cvar::find("sensitivity").GetFloat(),
		cvar::find("volume").GetFloat(),
		cvar::find("cl_crosshairsize").GetFloat()
	);

	return true;
}
