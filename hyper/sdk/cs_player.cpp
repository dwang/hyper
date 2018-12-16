#include "cs_player.hpp"
#include "../utils/u_process.hpp"
#include "../utils/u_static.hpp"
#include "cs_interfaces.hpp"

#define FL_ONGROUND	(1 << 0)
#define FL_PARTIALGROUND (1 << 18)

int cs_player::get_team_num()
{
	return cs_process->read<int>(base + cs_static->m_iTeamNum);
}

int cs_player::get_health()
{
	return cs_process->read<int>(base + cs_static->m_iHealth);
}

int cs_player::get_class_id()
{
	uintptr_t vtable = cs_process->read<uintptr_t>(base + 0x8); // IClientNetworkable
	uintptr_t fn = cs_process->read<uintptr_t>(vtable + 0x8); // 4 bytes per fn, 2nd index
	uintptr_t ptr = cs_process->read<uintptr_t>(fn + 0x1); // mov eax, ptr_to_client_class
	return cs_process->read<int>(ptr + 0x14);
}

int cs_player::get_glow_index()
{
	return cs_process->read<int>(base + cs_static->m_flFlashDuration + 24);
}

int cs_player::get_life_state()
{
	return cs_process->read<int>(base + cs_static->m_lifeState);
}

int cs_player::get_tick_count()
{
	return cs_process->read<int>(base + cs_static->m_nTickBase);
}

int cs_player::get_shots_fired()
{
	return cs_process->read<int>(base + cs_static->m_iShotsFired);
}

bool cs_player::is_scoped()
{
	return cs_process->read<bool>(base + cs_static->m_bIsScoped);
}

bool cs_player::is_dormant()
{
	csptr_t a;

	a = (csptr_t)(base + 0x8);
	return cs_process->read<bool>(a + cs_process->read<uint8_t>((*(cs_virtual_table*)&a).function(9) + 0x8));
}

bool cs_player::is_on_ground()
{
	int flags = cs_process->read<int>(base + cs_static->m_fFlags);
	return ((flags & FL_ONGROUND) || (flags & FL_PARTIALGROUND));
}

csptr_t cs_player::get_weapon()
{
	csptr_t v;

	v = cs_process->read<csptr_t>(base + cs_static->m_hActiveWeapon);
	return cs_process->read<csptr_t>(cs_static->dwEntityList + ((v & 0xFFF) - 1) * 0x10);
}

vec3 cs_player::get_origin()
{
	return cs_process->read<vec3>(base + cs_static->m_vecOrigin);
}

vec3 cs_player::get_vec_view()
{
	return cs_process->read<vec3>(base + cs_static->m_vecViewOffset);
}

vec3 cs_player::get_eye_pos()
{
	vec3 v, o, r;
	v = get_vec_view();
	o = get_origin();
	r.x = v.x += o.x; r.y = v.y += o.y; r.z = v.z += o.z;
	return r;
}

vec3 cs_player::get_vec_velocity()
{
	return cs_process->read<vec3>(base + cs_static->m_vecViewOffset);
}

vec3 cs_player::get_vec_punch()
{
	return cs_process->read<vec3>(base + cs_static->m_vecPunch);
}

int cs_player::get_fov()
{
	return cs_process->read<int>(base + cs_static->m_iFOV);
}

void cs_player::get_bone_matrix(int index, matrix3x4_t *out)
{
	cs_process->read(cs_process->read<csptr_t>(base + cs_static->m_dwBoneMatrix) + 0x30 * index, out, sizeof(matrix3x4_t));
}

bool cs_player::is_valid()
{
	int health = get_health();

	return base && get_life_state() == 0 && health > 0 && health < 1337;
}

csptr_t cs_player::get_pointer()
{
	return base;
}
