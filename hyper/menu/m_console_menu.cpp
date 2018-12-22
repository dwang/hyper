#include "m_console_menu.hpp"
#include "m_console_color.hpp"
#include "../features/f_aim.hpp"
#include "../features/f_misc.hpp"
#include "../features/f_visuals.hpp"
#include "../config/c_config.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>

m_console_menu* cs_console_menu = new m_console_menu();

void m_console_menu::update()
{
	system("cls");

	std::cout << std::endl;
	std::cout << white << "[hyper]" << std::endl << std::endl;

	std::cout << "[*] aimbot [" << cs_config->vk_to_string(cs_config->hotkeys.aimbot) << "] = ";
	if (cs_aim->aimbot_enabled)
		std::cout << green << "enabled " << white << std::endl << std::endl;
	else
		std::cout << red << "disabled" << white << std::endl << std::endl;

	std::cout << "[*] triggerbot [" << cs_config->vk_to_string(cs_config->hotkeys.triggerbot) << "] = ";
	if (cs_aim->triggerbot_enabled)
		std::cout << green << "enabled " << white << std::endl << std::endl;
	else
		std::cout << red << "disabled" << white << std::endl << std::endl;

	std::cout << "[*] glow [" << cs_config->vk_to_string(cs_config->hotkeys.glow) << "] = ";
	if (cs_visuals->glow_enabled)
		std::cout << green << "enabled " << white << std::endl << std::endl;
	else
		std::cout << red << "disabled" << white << std::endl << std::endl;

	std::cout << "[*] chams [" << cs_config->vk_to_string(cs_config->hotkeys.chams) << "] = ";
	if (cs_visuals->chams_enabled)
		std::cout << green << "enabled " << white << std::endl << std::endl;
	else
		std::cout << red << "disabled" << white << std::endl << std::endl;

	std::cout << "[*] bunnyhop [" << cs_config->vk_to_string(cs_config->hotkeys.bunnyhop) << "] = ";
	if (cs_misc->bunnyhop_enabled)
		std::cout << green << "enabled " << white << std::endl << std::endl;
	else
		std::cout << red << "disabled" << white << std::endl << std::endl;

	std::cout << "[*] reload config [" << cs_config->vk_to_string(cs_config->hotkeys.reload_config) << "]" << std::endl << std::endl;

	std::cout << "[*] panic key [" << cs_config->vk_to_string(cs_config->hotkeys.panic) << "]" << std::endl << std::endl;
}

void m_console_menu::show_console_cursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}