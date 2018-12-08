#pragma once

#include "../common_includes.hpp"

class m_overlay
{
public:
	HWND handle;
	MSG message;
	WNDCLASSEX window_class;
	RECT bounds;
	float width;
	float height;
	float x;
	float y;
	static LRESULT CALLBACK WndProc(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);
	bool setup();
	void refresh();
	void destroy();
};

extern m_overlay* cs_overlay;