#pragma once

#include "../sdk/cs_convar.hpp"
#include "../sdk/cs_types.hpp"
#include "../sdk/cs_player.hpp"

class f_aim {
public:
	void aimbot();
	void triggerbot();

	bool aimbot_enabled, triggerbot_enabled;
private:
	int current_tick, previous_tick;
	cs_player target;
	float flsensitivity;

	int crosshair_id(vec3 vangle, cs_player self);
	void aim_at_target(vec3 vangle, vec3 angle);
	bool get_target(cs_player self, vec3 vangle);
	vec3 get_target_angle(cs_player self, cs_player target);
	float get_fov(vec3 vangle, vec3 angle);

	typedef struct {
		int   bone;
		float radius;
		vec3  min;
		vec3  max;
	} HITBOX;

	HITBOX hitbox_list[2][6] = {
		{
			{5, 6.200000f, {4.800000f,  0.150000f,  -4.100000f}, {4.800000f,  0.150000f, 4.100000f}},
			{4, 6.500000f, {3.800000f,  0.800000f,  -2.400000f}, {3.800000f,  0.400000f, 2.400000f}},
			{3, 6.000000f, {1.400000f,  0.800000f,  3.100000f},  {1.400000f,  0.800000f, -3.100000f}},
			{0, 6.000000f, {-2.700000f, 1.100000f,  -3.200000f}, {-2.700000f, 1.100000f, 3.200000f}},
			{7, 3.500000f, {-1.300000f, -0.200000f, 0.000000f},  {1.400000f,  0.600000f, 0.000000f}},
			{8, 4.300000f, {-1.100000f, 1.400000f,  0.100000f},  {3.000000f,  0.800000f, 0.100000f}}
		},

		{
			{5, 6.200000f, {4.800000f,  0.150000f,  -4.100000f}, {4.800000f,  0.150000f, 4.100000f}},
			{4, 6.500000f, {3.800000f,  0.800000f,  -2.400000f}, {3.800000f,  0.400000f, 2.400000f}},
			{3, 6.000000f, {1.400000f,  0.800000f,  3.100000f},  {1.400000f,  0.800000f, -3.100000f}},
			{0, 6.000000f, {-2.700000f, 1.100000f,  -3.200000f}, {-2.700000f, 1.100000f, 3.200000f}},
			{7, 3.500000f, {-1.300000f, 0.900000f,  0.000000f},  {1.400000f,  1.300000f, 0.000000f}},
			{8, 3.200000f, {-0.200000f, 1.100000f,  0.000000f},  {3.600000f,  0.100000f, 0.000000f}}
		}
	};
};

extern f_aim* cs_aim;