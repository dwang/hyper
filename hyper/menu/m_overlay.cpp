#include "../common_includes.hpp"

m_overlay* cs_overlay = new m_overlay();

LRESULT CALLBACK m_overlay::WndProc(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_PAINT:
	{
		cs_menu->draw();
	}
	case WM_ERASEBKGND:
	{
		return 0;
	}
	default:
	{
		return DefWindowProc(Handle, Message, wParam, lParam);
	}
	}
}

bool m_overlay::setup()
{
	ZeroMemory(&window_class, sizeof(window_class));
	window_class.cbSize = sizeof(window_class);
	window_class.hInstance = GetModuleHandle(NULL);
	window_class.lpfnWndProc = WndProc;
	window_class.lpszClassName = "hyper";
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	if (RegisterClassEx(&window_class))
	{
		handle = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT, window_class.lpszClassName, window_class.lpszClassName, WS_POPUP, x, y, width, height, NULL, NULL, window_class.hInstance, NULL);
		if (handle)
		{
			SetLayeredWindowAttributes(handle, RGB(0, 0, 0), 255, LWA_COLORKEY | LWA_ALPHA);
			ShowWindow(handle, 1);
			MARGINS margin = { -1, -1, -1, -1 };
			DwmExtendFrameIntoClientArea(handle, &margin);
			HWND WinHandle = FindWindow(NULL, "Counter-Strike: Global Offensive");
			if (WinHandle)
			{
				if (IsWindowVisible(WinHandle))
				{
					GetClientRect(WinHandle, &bounds);
					width = bounds.right - bounds.left;
					height = bounds.bottom - bounds.top;
					ZeroMemory(&bounds, sizeof(bounds));
					GetWindowRect(WinHandle, &bounds);
					x = bounds.left;
					y = bounds.top;
					SetWindowPos(handle, HWND_TOPMOST, x, y, width, height, NULL);
					return true;
				}
			}
		}
	}
	return false;
}

void m_overlay::refresh()
{
	while (true)
	{
		if (handle)
		{
			InvalidateRect(handle, NULL, false);
			if (PeekMessage(&message, handle, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}
		else
		{
			setup();
			cs_renderer->setup();
			cs_menu->initialize();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void m_overlay::destroy()
{
	DestroyWindow(handle);
	UnregisterClass(window_class.lpszClassName, window_class.hInstance);
}