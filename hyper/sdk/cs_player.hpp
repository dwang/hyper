#pragma once

#include "cs_types.hpp"

class cs_player {
	csptr_t self;
public:
	int     GetTeam(void);
	int     GetHealth(void);
	int     GetLifeState(void);
	int     GetTickCount(void);
	int     GetShotsFired(void);
	bool    IsScoped(void);
	bool    IsDormant(void);
	csptr_t GetWeapon(void);
	vec3    GetOrigin(void);
	vec3    GetVecView(void);
	vec3    GetEyePos(void);
	vec3    GetVecVelocity(void);
	vec3    GetVecPunch(void);
	int     GetFov(void);
	void    GetBoneMatrix(int index, matrix3x4_t *out);
	bool    IsValid(void);
};
