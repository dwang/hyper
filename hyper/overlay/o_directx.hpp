#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

class o_directx {
public:
	IDirect3D9Ex* p_Object;
	IDirect3DDevice9Ex* p_Device;
	D3DPRESENT_PARAMETERS p_Params;
	ID3DXLine* p_Line;
	ID3DXFont* pFontSmall;

	int directx_init();
	int render();
	void set_window_to_target();

	int height = 1600;
	int width = 2560;

	HWND tWnd;
	HWND hWnd;
	RECT tSize;
	MSG message;
};

extern o_directx* cs_directx;