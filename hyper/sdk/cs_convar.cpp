#include "cs_convar.hpp"
#include "cs_interfaces.hpp"
#include "../utils/u_process.hpp"
#include "../utils/u_static.hpp"
#include <string.h>

SHORT_STRING cs_convar::GetName()
{
	return cs_process->read_ptr32<SHORT_STRING>(self + 0xC);
}

void cs_convar::SetString(const char *v)
{
	SHORT_STRING a0;
	memcpy(&a0, v, strlen(v));
	cs_process->write_ptr<SHORT_STRING>(self + 0x24, a0);
}

void cs_convar::SetInt(int v)
{
	cs_process->write<uint32_t>(self + 0x30, v ^ self);
}

void cs_convar::SetFloat(float v)
{
	cs_process->write<uint32_t>(self + 0x2C, *(uint32_t*)&v ^ self);
}

SHORT_STRING cs_convar::GetString()
{
	return cs_process->read_ptr32<SHORT_STRING>(self + 0x24);
}

int cs_convar::GetInt()
{
	return cs_process->read<int>(self + 0x30) ^ self;
}

float cs_convar::GetFloat()
{
	uint32_t v;
	v = cs_process->read<uint32_t>(self + 0x2C) ^ self;
	return *(float*)&v;
}

cs_convar cvar::find(const char *name)
{
	csptr_t a0;

	a0 = cs_process->read_ptr32<csptr_t>(cs_static->vt_cvar.address() + 0x34);
	while ((a0 = cs_process->read<csptr_t>(a0 + 0x4))) {
		if (!strcmp(name, cs_process->read_ptr32<SHORT_STRING>(a0 + 0xc).value)) {
			return *(cs_convar*)&a0;
		}
	}
	return {};
}

