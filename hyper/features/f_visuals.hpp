#pragma once

#include "../utils/u_color.hpp"
#include "../sdk/cs_types.hpp"

#include <inttypes.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class f_visuals
{
public:
	void glow();
	void chams();
	void radar();
	void esp();
	void no_hands();
	bool glow_enabled = false;
	bool chams_enabled = false;
	bool radar_enabled = false;
	bool no_hands_enabled = false;
	bool viewmodel_chams_enabled = false;
	float model_ambient_min;
private:
	struct glow_object_t {
		csptr_t				m_ent;
		u_color				m_color;
		uint8_t				pad_01[8];
		float				m_bloom_amt;
		uint8_t				pad_02[4];
		bool				m_rwo;
		bool				m_rwuo;
		bool				m_full_bloom;
		uint8_t				pad_03[5];
		int32_t				m_glow_style;
	};

	struct model_color {
		BYTE r;
		BYTE g;
		BYTE b;

		model_color()
		{
		}

		model_color(BYTE red, BYTE green, BYTE blue) {
			r = red;
			g = green;
			b = blue;
		}
	};
};

extern f_visuals* cs_visuals;