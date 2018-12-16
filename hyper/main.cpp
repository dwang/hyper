#include "utils/u_static.hpp"
#include "features/f_aim.hpp"
#include "features/f_visuals.hpp"
#include "features/f_misc.hpp"
#include "config/c_config.hpp"
#include "config/c_files.hpp"
#include "sdk/cs_engine.hpp"

#include <thread>
#include <iostream>

void aimbot() { cs_aim->aimbot(); }
void triggerbot() { cs_aim->triggerbot(); }
void glow() { cs_visuals->glow(); }
void chams() { cs_visuals->chams(); }
void no_hands() { cs_visuals->no_hands(); }
void bunnyhop() { cs_misc->bunnyhop(); }

int main()
{
	if (!cs_static->initialize()) {
		printf("[!] game is not running!\n");
		return -1;
	}

	cs_files->setup("config.ini", "C:\\hyper\\");
	cs_config->setup();
	cs_config->load();

	std::thread t_aimbot(aimbot);
	std::thread t_triggerbot(triggerbot);
	std::thread t_glow(glow);
	std::thread t_chams(chams);
	std::thread t_no_hands(no_hands);
	std::thread t_bunnyhop(bunnyhop);

	t_aimbot.detach();
	t_triggerbot.detach();
	t_glow.detach();
	t_chams.detach();
	t_no_hands.detach();
	t_bunnyhop.detach();

	while (true)
	{
		if (GetAsyncKeyState(cs_config->hotkeys.aimbot) & 0x8000)
		{
			cs_aim->aimbot_enabled = !cs_aim->aimbot_enabled;
			std::cout << cs_aim->aimbot_enabled << std::endl;
			Beep(330, 100);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (GetAsyncKeyState(cs_config->hotkeys.triggerbot) & 0x8000)
		{
			cs_aim->triggerbot_enabled = !cs_aim->triggerbot_enabled;
			std::cout << cs_aim->triggerbot_enabled << std::endl;
			Beep(330, 100);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (GetAsyncKeyState(cs_config->hotkeys.reload_config) & 0x8000)
		{
			cs_config->load();
			Beep(330, 100);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (GetAsyncKeyState(VK_END) & 0x8000 || FindWindowA(NULL, "Counter-Strike: Global Offensive") == NULL)
		{
			cvar::find("r_modelAmbientMin").SetFloat(0);
			
			t_aimbot.~thread();
			t_triggerbot.~thread();
			t_glow.~thread();
			t_chams.~thread();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			cs_process->detach();
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return 0;
}