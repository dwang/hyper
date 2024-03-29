#pragma once

#include "cs_types.hpp"

class cs_netvar_table {
	csptr_t self;
public:
	uint32_t offset(const char *name);
private:
	uint32_t offset_ex(csptr_t, const char *);
};

namespace netvars {
	cs_netvar_table find(const char *name);
}