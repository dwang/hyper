#include "../common_includes.hpp"

m_renderer* cs_renderer = new m_renderer();

void m_renderer::draw_line(float x, float y, float xx, float yy, float line_width, D3DCOLOR line_color, bool outlined, float outline_width, D3DCOLOR outline_color)
{
	D3DXVECTOR2 Points[2];
	Points[0] = D3DXVECTOR2(x, y);
	Points[1] = D3DXVECTOR2(xx, yy);
	
	if (outlined)
	{
		line->SetWidth(outline_width * 2.f + line_width);
		line->Draw(Points, 2, outline_color);
	}

	line->SetWidth(line_width);
	line->Draw(Points, 2, line_color);
}

void m_renderer::draw_circle(float x, float y, float radius, float line_width, D3DCOLOR line_color, bool outlined, float outline_width, D3DCOLOR outline_color)
{
	D3DXVECTOR2 points[50];

	for (int i = 0; i < 50; i++)
	{
		points[i] = D3DXVECTOR2(x + (radius * cos(i)), y + (radius * sin(i)));
	}

	if (outlined)
	{
		line->SetWidth(line_width + outline_width * 2);
		line->Draw(points, 50, outline_color);
	}

	line->SetWidth(line_width);
	line->Draw(points, 50, line_color);
}


void m_renderer::draw_rect(float x, float y, float width, float height, float line_width, D3DCOLOR line_color, bool filled, bool outlined, float outline_width, D3DCOLOR outline_color)
{
	if (filled)
	{
		if (outlined)
		{
			draw_line(x - outline_width, y + height / 2, x + width + outline_width, y + height / 2.f, height, outline_color, true, outline_width, outline_color);
		}

		draw_line(x, y + height / 2.f, x + width, y + height / 2, height, line_color, false, 0, 0);
	}
	else
	{
		if (outlined)
		{
			outline_width *= 2.f + line_width;
			draw_line(x, y, x, y + height, outline_width, outline_color, false, 0, 0);
			draw_line(x, y + height, x + width, y + height, outline_width, outline_color, false, 0, 0);
			draw_line(x + width, y + height, x + width, y, outline_width, outline_color, false, 0, 0);
			draw_line(x + width, y, x, y, outline_width, outline_color, false, 0, 0);
			draw_line(x, y, x, y + height, line_width, line_color, false, 0, 0);
			draw_line(x, y + height, x + width, y + height, line_width, line_color, false, 0, 0);
			draw_line(x + width, y + height, x + width, y, line_width, line_color, false, 0, 0);
			draw_line(x + width, y, x, y, line_width, line_color, false, 0, 0);
		}
		else
		{
			draw_line(x, y, x, x + height, line_width, line_color, false, 0, 0);
			draw_line(x, y + height, x + width, y + height, line_width, line_color, false, 0, 0);
			draw_line(x + width, y + height, x + width, y, line_width, line_color, false, 0, 0);
			draw_line(x + width, y, x, y, line_width, line_color, false, 0, 0);
		}
	}
}

void m_renderer::draw_string(float X, float Y, std::wstring Text, D3DCOLOR TextColor, bool Outlined, D3DCOLOR OutlineColor, bool Centered)
{
	RECT Rect;
	Rect.top = Y;
	Rect.left = X;
	Rect.bottom = Y;
	Rect.right = X;

	if (Outlined)
	{
		RECT OutlineRectTop;
		OutlineRectTop.top = Y - 1.f;
		OutlineRectTop.left = X;
		OutlineRectTop.bottom = Y;
		OutlineRectTop.right = X - 1.f;
		RECT OutlineRectLeft;
		OutlineRectLeft.top = Y;
		OutlineRectLeft.left = X - 1.f;
		OutlineRectLeft.bottom = Y;
		OutlineRectLeft.right = X - 1.f;
		RECT OutlineRectRight;
		OutlineRectRight.top = Y;
		OutlineRectRight.left = X + 1.f;
		OutlineRectRight.bottom = Y;
		OutlineRectRight.right = X + 1.f;
		RECT OutlineRectBottom;
		OutlineRectBottom.top = Y + 1.f;
		OutlineRectBottom.left = X;
		OutlineRectBottom.bottom = Y + 1.f;
		OutlineRectBottom.right = X;

		if (Centered)
		{
			font->DrawTextW(NULL, Text.c_str(), wcslen(Text.c_str()), &OutlineRectTop, DT_CENTER | DT_NOCLIP, OutlineColor);
			font->DrawTextW(NULL, Text.c_str(), wcslen(Text.c_str()), &OutlineRectLeft, DT_CENTER | DT_NOCLIP, OutlineColor);
			font->DrawTextW(NULL, Text.c_str(), wcslen(Text.c_str()), &OutlineRectRight, DT_CENTER | DT_NOCLIP, OutlineColor);
			font->DrawTextW(NULL, Text.c_str(), wcslen(Text.c_str()), &OutlineRectBottom, DT_CENTER | DT_NOCLIP, OutlineColor);
		}
		else
		{
			font->DrawTextW(NULL, Text.c_str(), wcslen(Text.c_str()), &OutlineRectTop, DT_NOCLIP, OutlineColor);
			font->DrawTextW(NULL, Text.c_str(), wcslen(Text.c_str()), &OutlineRectLeft, DT_NOCLIP, OutlineColor);
			font->DrawTextW(NULL, Text.c_str(), wcslen(Text.c_str()), &OutlineRectRight, DT_NOCLIP, OutlineColor);
			font->DrawTextW(NULL, Text.c_str(), wcslen(Text.c_str()), &OutlineRectBottom, DT_NOCLIP, OutlineColor);
		}
	}
	if (Centered)
	{
		font->DrawTextW(NULL, Text.c_str(), wcslen(Text.c_str()), &Rect, DT_CENTER | DT_NOCLIP, TextColor);
	}
	else
	{
		font->DrawTextW(NULL, Text.c_str(), wcslen(Text.c_str()), &Rect, DT_NOCLIP, TextColor);
	}
}

void m_renderer::draw_cross(float X, float Y, float Size, float LineWidth, D3DCOLOR CrossColor, bool Outlined, float OutlineWidth, D3DCOLOR OutlineColor)
{
	if (Outlined)
	{
		draw_line(X - Size / 2.f - OutlineWidth, Y, X - 1.f, Y, LineWidth, OutlineColor, true, OutlineWidth, OutlineColor);
		draw_line(X + Size / 2.f + OutlineWidth, Y, X, Y, LineWidth, OutlineColor, true, OutlineWidth, OutlineColor);
		draw_line(X, Y - Size / 2.f - OutlineWidth, X, Y - 1.f, LineWidth, OutlineColor, true, OutlineWidth, OutlineColor);
		draw_line(X, Y + Size / 2.f + OutlineWidth, X, Y, LineWidth, OutlineColor, true, OutlineWidth, OutlineColor);
	}

	draw_line(X - Size / 2.f, Y, X - 1.f, Y, LineWidth, CrossColor, false, OutlineWidth, OutlineColor);
	draw_line(X + Size / 2.f, Y, X, Y, LineWidth, CrossColor, false, OutlineWidth, OutlineColor);
	draw_line(X, Y - Size / 2.f, X, Y - 1.f, LineWidth, CrossColor, false, OutlineWidth, OutlineColor);
	draw_line(X, Y + Size / 2.f, X, Y, LineWidth, CrossColor, false, OutlineWidth, OutlineColor);
}

void m_renderer::begin_render()
{
	device->Clear(NULL, NULL, D3DCLEAR_TARGET, NULL, 1, NULL);
	device->BeginScene();
}

void m_renderer::end_render()
{
	device->EndScene();
	device->PresentEx(NULL, NULL, NULL, NULL, NULL);
}

bool m_renderer::setup()
{
	Direct3DCreate9Ex(D3D_SDK_VERSION, &d3d_interface);

	if (d3d_interface)
	{
		ZeroMemory(&params, sizeof(params));

		params.Windowed = TRUE;
		params.SwapEffect = D3DSWAPEFFECT_DISCARD;
		params.hDeviceWindow = cs_overlay->handle;
		params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
		params.BackBufferFormat = D3DFMT_A8R8G8B8;
		params.BackBufferWidth = cs_overlay->width;
		params.BackBufferHeight = cs_overlay->height;
		params.EnableAutoDepthStencil = TRUE;
		params.AutoDepthStencilFormat = D3DFMT_D16;
		d3d_interface->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, cs_overlay->handle, D3DCREATE_HARDWARE_VERTEXPROCESSING, &params, 0, &device);

		if (device)
		{
			D3DXCreateLine(device, &line);

			if (line)
			{
				D3DXCreateFont(device, 40, NULL, FW_NORMAL, NULL, false, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, "Calibri", &font);

				if (font)
				{
					line->SetAntialias(TRUE);
					return true;
				}
			}
		}
	}

	return false;
}

void m_renderer::destroy()
{
	line->Release();
	font->Release();
	device->Release();
	d3d_interface->Release();
}