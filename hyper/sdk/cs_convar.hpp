#pragma once

#include "cs_types.hpp"

class cs_convar {
	csptr_t self;
public:
	SHORT_STRING GetName();
	void         SetString(const char *v);
	void         SetInt(int v);
	void         SetFloat(float v);
	SHORT_STRING GetString();
	int          GetInt();
	float        GetFloat();
	csptr_t		 GetPointer();
};

namespace cvar {
	cs_convar find(const char *name);
}
