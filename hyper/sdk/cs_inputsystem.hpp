#pragma once

#include "cs_types.hpp"

namespace inputsystem {
	typedef enum _CS_BUTTONCODE : int {
		MOUSE_1 = 107,
		MOUSE_2 = 108,
		MOUSE_3 = 109,
		MOUSE_4 = 110,
		MOUSE_5 = 111
	} CS_BUTTONCODE;
	bool  IsButtonDown(CS_BUTTONCODE button);
	vec2i GetMouseAnalog();
	vec2i GetMouseAnalogDelta();
}
