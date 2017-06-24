#ifndef WEBRAY_TEXTURE_CHECKER_H
#define WEBRAY_TEXTURE_CHECKER_H

#include "texture.h"

class texture_checker : public texture {

public:
	texture_checker(float scale, const texture *o_texture, const texture *e_texture);
	virtual vector3 sample(float u, float v, const vector3& p) const;

	float scale;

	const texture* o_texture; // odd
	const texture* e_texture; // even
};

#endif