#pragma once

#include "../overlay/o_directx.hpp"
#include "../overlay/o_draw.hpp"

class m_menu {
public:
	void initialize();
	void render();
	void handle_messages();
	void update_overlay_state();

	static LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

	HHOOK MouseHook;
	HHOOK hMouseHook;

	bool menu_open = true;
};

extern m_menu* cs_menu;