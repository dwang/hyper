#include "f_visuals.hpp"
#include "../utils/u_static.hpp"
#include "../utils/u_process.hpp"
#include "../sdk/cs_engine.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <thread>

f_visuals* cs_visuals = new f_visuals();

void f_visuals::esp()
{
	
}

void f_visuals::glow()
{
	auto set_glow_clr = [](glow_object_t* object, u_color color, int glow_style, float bloom = 1.0f) {
		object->m_rwo = true;
		object->m_rwuo = false;
		object->m_color = color;
		object->m_bloom_amt = bloom;
		object->m_full_bloom = false;
		object->m_glow_style = glow_style;
	};

	cs_player self;
	cs_player entity;

	uintptr_t obj_manager = cs_process->read<uintptr_t>(cs_static->client_dll + cs_static->m_dwGlowObjectManager);

	self = entity::GetClientEntity(engine::GetLocalPlayer());

	for (int i = 1; i < engine::GetMaxClients(); i++)
	{
		entity = entity::GetClientEntity(i);

		glow_object_t object = cs_process->read<glow_object_t>(obj_manager + entity.get_glow_index() * 0x38);

		if (!object.m_ent || object.m_ent == self.get_pointer() || entity.is_dormant())
			continue;

		switch (entity.get_class_id())
		{
		case CCSPlayer:
			if (entity.is_valid())
			{
				set_glow_clr(&object, entity.get_team_num() == self.get_team_num() ? u_color(0.17f, 0.67f, 0.8f, 0.7f) : u_color(1.0f, 0.17f, 0.37f, 0.7f), chams_enabled ? 1 : 0);
				cs_process->write<glow_object_t>(obj_manager + entity.get_glow_index() * 0x38, object);
			}
			break;
		}
	}
}

void f_visuals::chams()
{
	cs_player self;
	cs_player entity;

	self = entity::GetClientEntity(engine::GetLocalPlayer());

	for (int i = 1; i < 500; i++)
	{
		entity = entity::GetClientEntity(i);

		if (entity.get_class_id() == CCSPlayer)
			cs_process->write<u_color>(entity.get_pointer() + 0x70, entity.get_team_num() == self.get_team_num() ? u_color(1.0f, 0.17f, 0.37f, 0.7f) : u_color(0.17f, 0.67f, 0.8f, 0.7f));
		else if (entity.get_class_id() == CPredictedViewModel)
		{
			cs_process->write<BYTE>(entity.get_pointer() + 0x70, 237);
			cs_process->write<BYTE>(entity.get_pointer() + 0x71, 80);
			cs_process->write<BYTE>(entity.get_pointer() + 0x72, 192);
		}
	}

	if (model_ambient_min != 10)
	{
		cvar::find("r_modelAmbientMin").SetFloat(10);
	}
}

void f_visuals::no_hands()
{
	cs_process->write<int>(cs_process->read<uintptr_t>(cs_static->client_dll + cs_static->m_dwLocalPlayer) + 0x258, 0);
}

void f_visuals::radar()
{

}