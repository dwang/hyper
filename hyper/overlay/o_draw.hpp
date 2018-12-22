#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

class o_draw {
public:
	void GradientFunc(int x, int y, int w, int h, int r, int g, int b, int a);
	void DrawCenterLine(float x, float y, int r, int g, int b, int a);
	void DrawLine(float x, float y, float xx, float yy, int r, int g, int b, int a);
	void FillRGB(float x, float y, float w, float h, int r, int g, int b, int a);
	void DrawBox(float x, float y, float width, float height, float px, int r, int g, int b, int a);
	void DrawGUIBox(float x, float y, float w, float h, int r, int g, int b, int a, int rr, int gg, int bb, int aa);
	void DrawHealthBar(float x, float y, float w, float h, int r, int g);
	void DrawHealthBarBack(float x, float y, float w, float h);

	int DrawString(const char* String, int x, int y, int r, int g, int b, ID3DXFont* ifont);
	int DrawShadowString(const char* String, int x, int y, int r, int g, int b, ID3DXFont* ifont);
};

extern o_draw* cs_draw;