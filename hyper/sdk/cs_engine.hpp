#pragma once

#include "cs_interfaces.hpp"
#include "cs_netvars.hpp"
#include "cs_convar.hpp"
#include "cs_inputsystem.hpp"
#include "cs_player.hpp"

namespace engine {
	int IsRunning();
	int GetLocalPlayer();
	vec3 GetViewAngles();
	void SetViewAngles(vec3 v);
	int GetMaxClients();
	bool IsInGame();
}

namespace entity {
	cs_player GetClientEntity(int index);
}
