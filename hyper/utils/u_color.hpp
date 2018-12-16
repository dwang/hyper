#pragma once

struct u_color {
	float r;
	float g;
	float b;
	float a;

	u_color()
	{
	}

	u_color(float red, float green, float blue, float alpha) {
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}
};