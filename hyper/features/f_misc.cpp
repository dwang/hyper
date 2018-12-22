#include "f_misc.hpp"
#include "../sdk/cs_engine.hpp"
#include "../utils/u_static.hpp"
#include "../utils/u_process.hpp"

#include <thread>
#include <Windows.h>

f_misc* cs_misc = new f_misc();

void f_misc::bunnyhop()
{
	while (true)
	{
		if (!engine::IsInGame() || !bunnyhop_enabled)
			continue;

		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			if (entity::GetClientEntity(engine::GetLocalPlayer()).is_on_ground())
			{
				cs_process->write<int>(cs_static->client_dll + cs_static->m_dwForceJump, 6);
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
