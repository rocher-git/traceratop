#include "texture_constant.h"

texture_constant::texture_constant(const vector3& color) :
	color(color) {
}

texture_constant::texture_constant(float r, float g, float b) :
	color(r, g, b) {
}
vector3 texture_constant::sample(float u, float v, const vector3& p) const {

	return color;
}