#pragma once

#include "../sdk/cs_types.hpp"

class f_misc
{
public:
	void bunnyhop();
	bool bunnyhop_enabled = false;
};

extern f_misc* cs_misc;