#pragma once

#include "../common_includes.hpp"

#include <Windows.h>
#include <string>

class c_config {
public:
	void setup();
	void load();

	struct {
		int aimbot = VK_F1;
		int triggerbot = VK_F2;
		int bunnyhop = VK_F3;
		int reload_config = VK_F4;
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