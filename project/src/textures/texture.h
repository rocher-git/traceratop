#ifndef WEBRAY_TEXTURE_H
#define WEBRAY_TEXTURE_H

#include "../math/vector3.h"

class texture {

public:
	virtual vector3 sample(float u, float v, const vector3& p) const = 0;
};

#endif