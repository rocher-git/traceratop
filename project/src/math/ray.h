#ifndef MATH_RAY
#define MATH_RAY

#include "vector3.h"

class ray {

public:
	ray() {
	}

	ray(const vector3& origin, const vector3& direction) :
		origin(origin),
		direction(direction) {
	}

	ray(const ray& r1) :
		origin(r1.origin),
		direction(r1.direction) {
	}

	inline vector3 pointAt(float t) const {
		return this->origin + this->direction * t;
	}

	vector3 origin;
	vector3 direction;
};



#endif

