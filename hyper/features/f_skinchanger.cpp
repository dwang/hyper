#include "f_skinchanger.hpp"
#include "../utils/u_static.hpp"
#include "../utils/u_process.hpp"
#include "../sdk/cs_engine.hpp"
#include <Windows.h>

#define precache_bayonet_ct 87 // = v_knife_bayonet.mdl - v_knife_default_ct.mdl
#define precache_bayonet_t 63 // = v_knife_bayonet.mdl - v_knife_default_t.mdl

#define m_hViewModel 0x32F8
#define m_iViewModelIndex 0x3220
#define m_flFallbackWear 0x31C0
#define m_nFallbackPaintKit 0x31B8
#define m_iItemIDHigh 0x2FC0
#define m_iEntityQuality 0x2FAC
#define m_iItemDefinitionIndex 0x2FAA
#define m_hActiveWeapon 0x2EF8
#define m_hMyWeapons 0x2DF8
#define m_nModelIndex 0x258

f_skinchanger* cs_skinchanger = new f_skinchanger();

void f_skinchanger::skinchanger()
{
	while (true)
	{
		cs_player self;
		self = entity::GetClientEntity(engine::GetLocalPlayer());

		if (paintkit > 0 && model_index > 0)
		{
			for (int i = 0; i < 8; i++)
			{
				uintptr_t current_weapon = cs_process->read<uintptr_t>(self.get_pointer() + m_hMyWeapons + i * 0x4) & 0xfff;
				current_weapon = cs_process->read<uintptr_t>(cs_static->dwEntityList + (current_weapon - 1) * 0x10);
				short weapon_id = cs_process->read<short>(current_weapon + m_iItemDefinitionIndex);

				int fallback_paintkit = paintkit;
				if (weapon_id == 1) { fallback_paintkit = 37; } /* Desert Eagle | Blaze */
				else if (weapon_id == 4) { fallback_paintkit = 38; } /* Glock-18 | Fade */
				else if (weapon_id == 7) { fallback_paintkit = 180; } /* AK-47 | Fire Serpent */
				else if (weapon_id == 9) { fallback_paintkit = 344; } /* AWP | Dragon Lore */
				else if (weapon_id == 60) { fallback_paintkit = 445; } /* M4A1-S | Hot Rod */
				else if (weapon_id == 61) { fallback_paintkit = 653; } /* USP-S | Neo-Noir */
				else if (weapon_id != WEAPON_KNIFE && weapon_id != WEAPON_KNIFE_T && weapon_id != item_def) { continue; }
				else
				{
					cs_process->write<short>(current_weapon + m_iItemDefinitionIndex, item_def);
					cs_process->write<int>(current_weapon + m_nModelIndex, model_index);
					cs_process->write<int>(current_weapon + m_iViewModelIndex, model_index);
					cs_process->write<int>(current_weapon + m_iEntityQuality, entityQuality);
				}

				cs_process->write<int>(current_weapon + m_iItemIDHigh, itemIDHigh);
				cs_process->write<int>(current_weapon + m_nFallbackPaintKit, fallback_paintkit);
				cs_process->write<float>(current_weapon + m_flFallbackWear, fallbackWear);
			}
		}

		csptr_t active_weapon = self.get_weapon();

		short weapon_id = cs_process->read<short>(active_weapon + m_iItemDefinitionIndex);
		int weaponViewModelID = cs_process->read<int>(active_weapon + m_iViewModelIndex);

		if (weapon_id == WEAPON_KNIFE && weaponViewModelID > 0)
		{
			model_index = weaponViewModelID + precache_bayonet_ct + 3 * knife_id + knife_id_offset;
		}
		else if (weapon_id == WEAPON_KNIFE_T && weaponViewModelID > 0)
		{
			model_index = weaponViewModelID + precache_bayonet_t + 3 * knife_id + knife_id_offset;
		}
		else if (weapon_id != item_def || model_index == 0) { continue; }

		uintptr_t knifeViewModel = cs_process->read<uintptr_t>(self.get_pointer() + m_hViewModel) & 0xfff;
		knifeViewModel = cs_process->read<uintptr_t>(cs_static->dwEntityList + (knifeViewModel - 1) * 0x10);
		if (knifeViewModel == 0) { continue; }

		cs_process->write<uintptr_t>(knifeViewModel + m_nModelIndex, model_index);
	}
}