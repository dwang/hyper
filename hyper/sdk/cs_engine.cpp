#include <stdio.h>

#include "cs_engine.hpp"
#include "../utils/u_process.hpp"
#include "../utils/u_static.hpp"

int engine::IsRunning()
{
	return cs_process->exists();
}

int engine::GetLocalPlayer()
{
	return cs_process->read<int>(cs_static->dwClientState + cs_static->m_dwGetLocalPlayer);
}

vec3 engine::GetViewAngles()
{
	return cs_process->read<vec3>(cs_static->dwClientState + cs_static->m_dwViewAngles);
}

void engine::SetViewAngles(vec3 v)
{
	cs_process->write<vec3>(cs_static->dwClientState + cs_static->m_dwViewAngles, v);
}

int engine::GetMaxClients()
{
	return cs_process->read<int>(cs_static->dwClientState + cs_static->m_dwMaxClients);
}

bool engine::IsInGame()
{
	return cs_process->read<unsigned char>(cs_static->dwClientState + cs_static->m_dwState) >> 2;
}

cs_player entity::GetClientEntity(int index)
{
	return cs_process->read<cs_player>(cs_static->dwEntityList + index * 0x10);
}

static csptr_t offset_entitylist()
{
	return cs_static->vt_entity.address() - (cs_process->read<uint32_t>(cs_static->vt_entity.function(5) + 0x22) - 0x38);
}

static csptr_t offset_clientstate()
{
	return cs_process->read_ptr32<csptr_t>(cs_static->vt_engine.function(18) + 0x16);
}

