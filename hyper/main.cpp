#include "utils/u_static.hpp"
#include "features/f_aim.hpp"
#include "features/f_visuals.hpp"
#include "features/f_misc.hpp"
#include "config/c_config.hpp"
#include "config/c_files.hpp"
#include "menu/m_console_menu.hpp"
#include "menu/m_menu.hpp"
#include "sdk/cs_engine.hpp"

#include <thread>
#include <iostream>

void aim()
{
	while (true)
	{
		if (!engine::IsInGame())
			continue;

		if (cs_aim->aimbot_enabled)
			cs_aim->aimbot();

		if (cs_aim->triggerbot_enabled)
			cs_aim->triggerbot();

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void visuals()
{
	while (true)
	{
		if (!engine::IsInGame())
			continue;

		if (cs_visuals->glow_enabled)
			cs_visuals->glow();

		if (cs_visuals->chams_enabled)
			cs_visuals->chams();

		cs_visuals->no_hands();

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void misc()
{
	while (true)
	{
		if (!engine::IsInGame())
			continue;

		if (cs_misc->bunnyhop_enabled)
			cs_misc->bunnyhop();

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

//void menu()
//{

//}

void bunnyhop() {  }
void menu() { cs_menu->handle_messages(); }
void set_window() { cs_directx->set_window_to_target(); }

int main()
{
	SetConsoleTitleA("hyper");
	cs_console_menu->show_console_cursor(false);

	if (!cs_static->initialize()) {
		printf("[!] game is not running!\n");
		return -1;
	}

	cs_files->setup("config.ini", "C:\\hyper\\");
	cs_config->setup();
	cs_config->load();

	std::thread t_aim(aim);
	std::thread t_visuals(visuals);
	std::thread t_misc(misc);
	std::thread t_menu(menu);
	std::thread t_set_window(set_window);

	t_aim.detach();
	t_visuals.detach();
	t_misc.detach();
	t_menu.detach();
	t_set_window.detach();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	cs_console_menu->update();

	while (true)
	{
		cs_aim->sensitivity = cvar::find("sensitivity").GetFloat();
		cs_aim->mp_teammates_are_enemies = cvar::find("mp_teammates_are_enemies").GetInt();
		cs_visuals->model_ambient_min = cvar::find("r_modelAmbientMin").GetFloat();

		if (GetAsyncKeyState(cs_config->hotkeys.aimbot) & 0x8000)
		{
			cs_aim->aimbot_enabled = !cs_aim->aimbot_enabled;
			cs_console_menu->update();
			Beep(330, 100);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (GetAsyncKeyState(cs_config->hotkeys.triggerbot) & 0x8000)
		{
			cs_aim->triggerbot_enabled = !cs_aim->triggerbot_enabled;
			cs_console_menu->update();
			Beep(330, 100);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (GetAsyncKeyState(cs_config->hotkeys.glow) & 0x8000)
		{
			cs_visuals->glow_enabled = !cs_visuals->glow_enabled;
			cs_console_menu->update();
			Beep(330, 100);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (GetAsyncKeyState(cs_config->hotkeys.chams) & 0x8000)
		{
			cs_visuals->chams_enabled = !cs_visuals->chams_enabled;
			cs_console_menu->update();
			Beep(330, 100);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (GetAsyncKeyState(cs_config->hotkeys.bunnyhop) & 0x8000)
		{
			cs_misc->bunnyhop_enabled = !cs_misc->bunnyhop_enabled;
			cs_console_menu->update();
			Beep(330, 100);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (GetAsyncKeyState(VK_INSERT) & 0x8000)
		{
			cs_menu->menu_open = !cs_menu->menu_open;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (GetAsyncKeyState(cs_config->hotkeys.reload_config) & 0x8000)
		{
			cs_config->load();
			cs_console_menu->update();
			Beep(330, 100);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (GetAsyncKeyState(VK_END) & 0x8000 || FindWindowA(NULL, "Counter-Strike: Global Offensive") == NULL)
		{
			cvar::find("r_modelAmbientMin").SetFloat(0);
			
			t_aim.~thread();
			t_visuals.~thread();
			t_misc.~thread();
			t_menu.~thread();
			t_set_window.~thread();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			cs_process->detach();
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}