#include "m_menu.hpp"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include <thread>

m_menu* cs_menu = new m_menu();


LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (cs_menu->menu_open)
	{
		ImGuiIO& io = ImGui::GetIO();
		MOUSEHOOKSTRUCT * pMouseStruct = (MOUSEHOOKSTRUCT *)lParam;
		if (pMouseStruct != NULL) {
			switch (wParam)
			{
			case WM_LBUTTONDOWN:
				io.MouseDown[0] = true;
				break;
			case WM_LBUTTONUP:
				io.MouseDown[0] = false;
				io.MouseReleased[0] = true;
				break;
			case WM_RBUTTONDOWN:
				io.MouseDown[1] = true;
				break;
			case WM_RBUTTONUP:
				io.MouseDown[1] = false;
				io.MouseReleased[1] = true;
				break;
			}
		}
	}
	return CallNextHookEx(cs_menu->hMouseHook, nCode, wParam, lParam);
}

void m_menu::initialize()
{
	WNDCLASSEX wClass;
	wClass.cbClsExtra = NULL;
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.cbWndExtra = NULL;
	wClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wClass.hCursor = LoadCursor(0, IDC_ARROW);
	wClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
	wClass.hInstance = (HINSTANCE)GetModuleHandle(NULL);
	wClass.lpfnWndProc = WinProc;
	wClass.lpszClassName = "hyper";
	wClass.lpszMenuName = "hyper";
	wClass.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wClass))
		exit(1);

	cs_directx->tWnd = FindWindow(0, "Counter-Strike: Global Offensive");
	if (cs_directx->tWnd)
	{
		GetWindowRect(cs_directx->tWnd, &cs_directx->tSize);
		cs_directx->width = cs_directx->tSize.right - cs_directx->tSize.left;
		cs_directx->height = cs_directx->tSize.bottom - cs_directx->tSize.top;
		cs_directx->hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, "hyper", "hyper", WS_POPUP, 1, 1, cs_directx->width, cs_directx->height, 0, 0, 0, 0);
		SetLayeredWindowAttributes(cs_directx->hWnd, 0, 1.0f, LWA_ALPHA);
		SetLayeredWindowAttributes(cs_directx->hWnd, 0, RGB(0, 0, 0), LWA_COLORKEY);
		ShowWindow(cs_directx->hWnd, SW_SHOW);
	}

	cs_directx->directx_init();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = 1.0f / 60.0f;
	D3DDEVICE_CREATION_PARAMETERS d3dcp{ 0 };
	cs_directx->p_Device->GetCreationParameters(&d3dcp);
	//io.Fonts->AddFontDefault();
	ImGui_ImplWin32_Init(cs_directx->hWnd);
	ImGui_ImplDX9_Init(cs_directx->p_Device);
	MouseHook = SetWindowsHookEx(WH_MOUSE, MouseProc, 0, GetCurrentThreadId());
	ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Arial.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
}

LRESULT CALLBACK m_menu::WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	MARGINS margin = { 0, 0, cs_directx->width, cs_directx->height };

	switch (Message)
	{
	case WM_PAINT:
		cs_directx->render();
		break;

	case WM_CREATE:
		DwmExtendFrameIntoClientArea(hWnd, &margin);
		break;

	case WM_DESTROY:
		PostQuitMessage(1);
		return 0;

	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

void m_menu::render()
{
	if (menu_open)
	{
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowTestWindow();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}
}

void m_menu::update_overlay_state()
{
	long style = GetWindowLong(cs_directx->hWnd, GWL_EXSTYLE);
	if (menu_open)
	{
		style &= ~WS_EX_LAYERED;
		SetWindowLong(cs_directx->hWnd,
			GWL_EXSTYLE, style);
		SetForegroundWindow(cs_directx->hWnd);
	}
	else
	{
		style |= WS_EX_LAYERED;
		SetWindowLong(cs_directx->hWnd,
			GWL_EXSTYLE, style);
	}
}

void m_menu::handle_messages()
{
	initialize();

	while (true)
	{
		if (PeekMessage(&cs_directx->message, cs_directx->hWnd, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&cs_directx->message);
			TranslateMessage(&cs_directx->message);
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}