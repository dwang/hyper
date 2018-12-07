#pragma once

#include "cs_types.hpp"

class cs_virtual_table {
	csptr_t self;
public:
	csptr_t address(void) const;
	csptr_t function(int index);
};

class cs_interface_reg {
	csptr_t self;
public:
	cs_virtual_table find(const char *name);
};

namespace interfaces {
	cs_interface_reg find(const wchar_t *module_name);
}
