#include "common_includes.hpp"

void aimbot() { cs_aim->aimbot(); }
void triggerbot() { cs_aim->triggerbot(); }
//void overlay() { cs_overlay->refresh(); }

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
	//std::thread t_overlay(overlay);

	t_aimbot.detach();
	t_triggerbot.detach();
	//t_overlay.detach();

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
			t_aimbot.~thread();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			cs_process->detach();
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return 0;
}