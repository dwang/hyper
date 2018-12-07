#pragma once

#include "cs_types.hpp"

class cs_convar {
	csptr_t self;
public:
	SHORT_STRING GetName(void);
	void         SetString(const char *v);
	void         SetInt(int v);
	void         SetFloat(float v);
	SHORT_STRING GetString(void);
	int          GetInt(void);
	float        GetFloat(void);
};

namespace cvar {
	cs_convar find(const char *name);
}
