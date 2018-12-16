#include "cs_interfaces.hpp"
#include "../utils/u_process.hpp"
#include <string.h>

csptr_t cs_virtual_table::address() const
{
	return self;
}

csptr_t cs_virtual_table::function(int index)
{
	return cs_process->read<csptr_t>(cs_process->read<csptr_t>(self) + index * 4);
}

cs_virtual_table cs_interface_reg::find(const char *name)
{
	csptr_t a0 = self, a1[30];

	do {
		cs_process->read(cs_process->read<csptr_t>(a0 + 0x4), &a1, sizeof(a1));
		if ((_stricmp((const char *)a1, name) >> 5) == 1) {
			return cs_process->read<cs_virtual_table>(cs_process->read<csptr_t>(a0) + 1);
		}
	} while ((a0 = cs_process->read<csptr_t>(a0 + 0x8)));
	throw name;
}

cs_interface_reg interfaces::find(const wchar_t *module_name)
{
	uintptr_t v;

	v = cs_process->find_export(cs_process->find_module(module_name), "CreateInterface");
	if (v == 0)
		throw module_name;
	return cs_process->read_ptr32<cs_interface_reg>(v - 0x6A);
}

