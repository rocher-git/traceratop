#include "texture_checker.h"

texture_checker::texture_checker(float scale, const texture *o_texture, const texture *e_texture) :
	scale(scale),
	o_texture(o_texture),
	e_texture(e_texture) {
}

vector3 texture_checker::sample(float u, float v, const vector3& p) const {

	float pattern = (fmodf(u * scale, 1) > 0.5) ^ (fmodf(v * scale, 1) > 0.5);

	if (pattern < 0.5f) {
		return o_texture->sample(u, v, p);
	}
	else {
		return e_texture->sample(u, v, p);
	}
}