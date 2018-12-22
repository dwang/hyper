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

void aimbot() { cs_aim->aimbot(); }
void triggerbot() { cs_aim->triggerbot(); }
void glow() { cs_visuals->glow(); }
void chams() { cs_visuals->chams(); }
void no_hands() { cs_visuals->no_hands(); }
void bunnyhop() { cs_misc->bunnyhop(); }
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

	std::thread t_aimbot(aimbot);
	std::thread t_triggerbot(triggerbot);
	std::thread t_glow(glow);
	std::thread t_chams(chams);
	std::thread t_no_hands(no_hands);
	std::thread t_bunnyhop(bunnyhop);
	std::thread t_menu(menu);
	std::thread t_set_window(set_window);

	t_aimbot.detach();
	t_triggerbot.detach();
	t_glow.detach();
	t_chams.detach();
	t_no_hands.detach();
	t_bunnyhop.detach();
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
			cs_menu->update_overlay_state();
			Beep(330, 100);
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
			
			t_aimbot.~thread();
			t_triggerbot.~thread();
			t_glow.~thread();
			t_chams.~thread();
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