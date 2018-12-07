#pragma once

#include "cs_interfaces.hpp"
#include "cs_netvars.hpp"
#include "cs_convar.hpp"
#include "cs_inputsystem.hpp"
#include "cs_player.hpp"

namespace engine {
	int       IsRunning(void);
	int       GetLocalPlayer(void);
	vec3      GetViewAngles(void);
	void      SetViewAngles(vec3 v);
	int       GetMaxClients(void);
	bool      IsInGame(void);
}

namespace entity {
	cs_player GetClientEntity(int index);
}
