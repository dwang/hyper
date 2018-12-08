#include "cs_inputsystem.hpp"
#include "../utils/u_process.hpp"
#include "../utils/u_static.hpp"
#include "cs_interfaces.hpp"

bool inputsystem::IsButtonDown(CS_BUTTONCODE button)
{
	uint32_t v;

	v = cs_process->read<uint32_t>(cs_static->vt_inputsystem.address() + (((button >> 5) * 4) + cs_static->m_dwButton));
	return (v >> (button & 31)) & 1;
}

vec2i inputsystem::GetMouseAnalog()
{
	return cs_process->read<vec2i>(cs_static->vt_inputsystem.address() + cs_static->m_dwAnalog);
}

vec2i inputsystem::GetMouseAnalogDelta()
{
	return cs_process->read<vec2i>(cs_static->vt_inputsystem.address() + cs_static->m_dwAnalogDelta);
}

