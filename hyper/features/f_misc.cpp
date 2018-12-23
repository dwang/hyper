#include "f_misc.hpp"
#include "../sdk/cs_engine.hpp"
#include "../utils/u_static.hpp"
#include "../utils/u_process.hpp"

#include <thread>
#include <Windows.h>

f_misc* cs_misc = new f_misc();

void f_misc::bunnyhop()
{
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (entity::GetClientEntity(engine::GetLocalPlayer()).is_on_ground())
		{
			cs_process->write<int>(cs_static->client_dll + cs_static->m_dwForceJump, 6);
		}
	}
}
