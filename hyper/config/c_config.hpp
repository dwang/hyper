#pragma once

#include "../sdk/cs_inputsystem.hpp"

#define WIN32_LEAN_AND_MEAN  
#include <Windows.h>
#include <string>


class c_config {
public:
	void setup();
	void load();

	struct {
		int aimbot = VK_F1;
		int triggerbot = VK_F2;
		int glow = VK_F3;
		int chams = VK_F4;
		int bunnyhop = VK_F5;
		int reload_config = VK_F6;
		int panic = VK_END;
	} hotkeys;

	struct {
		inputsystem::CS_BUTTONCODE button = inputsystem::MOUSE_1;
		float fov = 2.0f;
		float smooth = 2.0f;
	} aimbot;

	struct {
		inputsystem::CS_BUTTONCODE button = inputsystem::MOUSE_5;
	} triggerbot;

	std::string vk_to_string(int vk);
};

extern c_config* cs_config;