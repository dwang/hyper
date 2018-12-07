#include "cs_player.hpp"
#include "../utils/u_process.hpp"
#include "../utils/u_static.hpp"
#include "cs_interfaces.hpp"

int cs_player::GetTeam(void)
{
	return cs_process->read<int>(self + cs_static->m_iTeamNum);
}

int cs_player::GetHealth(void)
{
	return cs_process->read<int>(self + cs_static->m_iHealth);
}

int cs_player::GetLifeState(void)
{
	return cs_process->read<int>(self + cs_static->m_lifeState);
}

int cs_player::GetTickCount(void)
{
	return cs_process->read<int>(self + cs_static->m_nTickBase);
}

int cs_player::GetShotsFired(void)
{
	return cs_process->read<int>(self + cs_static->m_iShotsFired);
}

bool cs_player::IsScoped(void)
{
	return cs_process->read<bool>(self + cs_static->m_bIsScoped);
}

bool cs_player::IsDormant(void)
{
	csptr_t a;

	a = (csptr_t)(self + 0x8);
	return cs_process->read<bool>(a + cs_process->read<uint8_t>((*(cs_virtual_table*)&a).function(9) + 0x8));
}

csptr_t cs_player::GetWeapon(void)
{
	csptr_t v;

	v = cs_process->read<csptr_t>(self + cs_static->m_hActiveWeapon);
	return cs_process->read<csptr_t>(cs_static->dwEntityList + ((v & 0xFFF) - 1) * 0x10);
}

vec3 cs_player::GetOrigin(void)
{
	return cs_process->read<vec3>(self + cs_static->m_vecOrigin);
}

vec3 cs_player::GetVecView(void)
{
	return cs_process->read<vec3>(self + cs_static->m_vecViewOffset);
}

vec3 cs_player::GetEyePos(void)
{
	vec3 v, o, r;
	v = this->GetVecView();
	o = this->GetOrigin();
	r.x = v.x += o.x; r.y = v.y += o.y; r.z = v.z += o.z;
	return r;
}

vec3 cs_player::GetVecVelocity(void)
{
	return cs_process->read<vec3>(self + cs_static->m_vecViewOffset);
}

vec3 cs_player::GetVecPunch(void)
{
	return cs_process->read<vec3>(self + cs_static->m_vecPunch);
}

int cs_player::GetFov(void)
{
	return cs_process->read<int>(self + cs_static->m_iFOV);
}

void cs_player::GetBoneMatrix(int index, matrix3x4_t *out)
{
	cs_process->read(cs_process->read<csptr_t>(self + cs_static->m_dwBoneMatrix) + 0x30 * index, out, sizeof(matrix3x4_t));
}

bool cs_player::IsValid(void)
{
	int health = GetHealth();

	return self && GetLifeState() == 0 && health > 0 && health < 1337;
}
