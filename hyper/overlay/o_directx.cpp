#include "o_directx.hpp"
#include "o_draw.hpp"
#include "../menu/m_menu.hpp"
#include <thread>
#include <Windows.h>

o_directx* cs_directx = new o_directx();

void o_directx::directx_init()
{
	Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object);

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hWnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = width;
	p_Params.BackBufferHeight = height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;
	p_Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device);

	if (!p_Line)
		D3DXCreateLine(p_Device, &p_Line);

	D3DXCreateFont(p_Device, 18, 0, 0, 0, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Verdana", &pFontSmall);
}

int o_directx::render()
{
	p_Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
	p_Device->BeginScene();

	if (tWnd == GetForegroundWindow())
	{
		cs_draw->DrawString("hyper - " __DATE__, 5, 0, 240, 240, 250, pFontSmall);

		cs_draw->FillRGB(width / 2 - 22, height / 2, 44, 1, 240, 240, 250, 255);
		cs_draw->FillRGB(width / 2, height / 2 - 22, 1, 44, 240, 240, 250, 255);
	}

	p_Device->EndScene();
	p_Device->PresentEx(0, 0, 0, 0, 0);

	return 0;
}

void o_directx::set_window_to_target()
{
	GetWindowRect(tWnd, &tSize);
	width = tSize.right - tSize.left;
	height = tSize.bottom - tSize.top;
	DWORD dwStyle = GetWindowLong(tWnd, GWL_STYLE);
	if (dwStyle & WS_BORDER)
	{
		tSize.top += 23;
		height -= 23;
	}
	MoveWindow(hWnd, tSize.left, tSize.top, width, height, true);
}