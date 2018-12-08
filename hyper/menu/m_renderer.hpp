#pragma once

class m_renderer
{
public:
	IDirect3D9Ex* d3d_interface;
	IDirect3DDevice9Ex* device;
	D3DPRESENT_PARAMETERS params;
	ID3DXLine* line;
	ID3DXFont* font;

	void draw_line(float x, float y, float xx, float yy, float line_width, D3DCOLOR line_color, bool outlined, float outline_width, D3DCOLOR outline_color);
	void draw_circle(float x, float y, float radius, float line_width, D3DCOLOR line_color, bool outlined, float outline_width, D3DCOLOR outline_color);
	void draw_rect(float x, float y, float width, float height, float line_width, D3DCOLOR line_color, bool filled, bool outlined, float outline_width, D3DCOLOR outline_color);
	void draw_cross(float x, float y, float size, float line_width, D3DCOLOR cross_color, bool outlined, float outline_width, D3DCOLOR outline_color);
	void draw_string(float x, float y, std::wstring text, D3DCOLOR text_color, bool outlined, D3DCOLOR outline_color, bool centered);
	bool setup();
	void begin_render();
	void end_render();
	void destroy();
};

extern m_renderer* cs_renderer;