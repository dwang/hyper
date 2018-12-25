#pragma once

class f_skinchanger
{
	enum knife_definition_index             // id
	{
		WEAPON_KNIFE = 42,
		WEAPON_KNIFE_T = 59,
		WEAPON_KNIFE_BAYONET = 500,         // 0
		WEAPON_KNIFE_FLIP = 505,            // 1
		WEAPON_KNIFE_GUT = 506,             // 2
		WEAPON_KNIFE_KARAMBIT = 507,        // 3
		WEAPON_KNIFE_M9_BAYONET = 508,      // 4
		WEAPON_KNIFE_TACTICAL = 509,        // 5
		WEAPON_KNIFE_FALCHION = 512,        // 6
		WEAPON_KNIFE_SURVIVAL_BOWIE = 514,  // 7
		WEAPON_KNIFE_BUTTERFLY = 515,       // 8
		WEAPON_KNIFE_PUSH = 516,            // 9
		WEAPON_KNIFE_URSUS = 519,           // 10
		WEAPON_KNIFE_GYPSY_JACKKNIFE = 520, // 11
		WEAPON_KNIFE_STILETTO = 522,        // 12
		WEAPON_KNIFE_WIDOWMAKER = 523       // 13
	};

	const int itemIDHigh = -1;
	const int entityQuality = 3;
	const float fallbackWear = 0.0001f;
	int knife_id_offset = 0;
	int model_index = 0;
public:
	void skinchanger();
	int knife_id = 0;
	int paintkit = 415;
	short item_def = 500;
};

extern f_skinchanger* cs_skinchanger;