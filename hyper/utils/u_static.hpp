#pragma once

#include "../common_includes.hpp"

class u_static
{
public:
	bool initialize();

	cs_virtual_table vt_client;
	cs_virtual_table vt_entity;
	cs_virtual_table vt_engine;
	cs_virtual_table vt_cvar;
	cs_virtual_table vt_inputsystem;
	csptr_t dwEntityList;
	csptr_t dwClientState;
	uint32_t m_dwGetLocalPlayer;
	uint32_t m_dwViewAngles;
	uint32_t m_dwMaxClients;
	uint32_t m_dwState;
	uint32_t m_dwButton;
	uint32_t m_dwAnalogDelta;
	uint32_t m_dwAnalog;
	uint32_t m_iHealth;
	uint32_t m_vecViewOffset;
	uint32_t m_lifeState;
	uint32_t m_nTickBase;
	uint32_t m_vecVelocity;
	uint32_t m_vecPunch;
	uint32_t m_iFOV;
	uint32_t m_iTeamNum;
	uint32_t m_vecOrigin;
	uint32_t m_hActiveWeapon;
	uint32_t m_iShotsFired;
	uint32_t m_bIsScoped;
	uint32_t m_dwBoneMatrix;

private:
	void initialize_nv();
	void initialize_vt();
	csptr_t offset_entitylist();
	csptr_t offset_clientstate();
};

extern u_static* cs_static;
