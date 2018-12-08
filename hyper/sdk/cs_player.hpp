#pragma once

#include "cs_types.hpp"

class cs_player {
	csptr_t self;
public:
	int     GetTeam();
	int     GetHealth();
	int     GetLifeState();
	int     GetTickCount();
	int     GetShotsFired();
	bool    IsScoped();
	bool    IsDormant();
	csptr_t GetWeapon();
	vec3    GetOrigin();
	vec3    GetVecView();
	vec3    GetEyePos();
	vec3    GetVecVelocity();
	vec3    GetVecPunch();
	int     GetFov();
	void    GetBoneMatrix(int index, matrix3x4_t *out);
	bool    IsValid();
};
