#include "cs_netvars.hpp"
#include "../utils/u_process.hpp"
#include "../utils/u_static.hpp"
#include "cs_interfaces.hpp"
#include <string.h>

uint32_t cs_netvar_table::offset(const char *name)
{
	return offset_ex(self, name);
}

uint32_t cs_netvar_table::offset_ex(csptr_t address, const char *name)
{
	uint32_t  a0;   uint32_t  a1;
	csptr_t   a2;   uint32_t  a3;
	csptr_t   a4;   uint32_t  a5;

	a0 = 0;
	for (a1 = cs_process->read<uint32_t>(address + 0x4); a1--;) {
		a2 = a1 * 60 + cs_process->read<csptr_t>(address);
		a3 = cs_process->read<uint32_t>(a2 + 0x2C);
		if ((a4 = cs_process->read<csptr_t>(a2 + 0x28)) && cs_process->read<uint32_t>(a4 + 0x4)) {
			if ((a5 = offset_ex(a4, name))) {
				a0 += a3 + a5;
			}
		}
		if (strcmp(cs_process->read_ptr32<SHORT_STRING>(a2).value, name) == 0) {
			return a3 + a0;
		}
	}
	return a0;
}

cs_netvar_table netvars::find(const char *name)
{
	csptr_t a0, a1;

	a0 = cs_process->read_ptr32<csptr_t>(cs_static->vt_client.function(8) + 1);
	do {
		a1 = cs_process->read<csptr_t>(a0 + 0xC);
		if (strcmp(cs_process->read_ptr32<SHORT_STRING>(a1 + 0xC).value, name) == 0) {
			return *(cs_netvar_table*)&a1;
		}
	} while ((a0 = cs_process->read<csptr_t>(a0 + 0x10)));
	throw name;
}

