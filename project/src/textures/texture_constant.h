#ifndef WEBRAY_TEXTURE_CONSTANT_H
#define WEBRAY_TEXTURE_CONSTANT_H

#include "texture.h"

class texture_constant : public texture {

public:
	texture_constant(const vector3& color);
	texture_constant(float r, float g, float b);
	virtual vector3 sample(float u, float v, const vector3& p) const;

	vector3 color;
};

#endif