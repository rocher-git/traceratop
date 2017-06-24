
#ifndef MATH_RAY_HIT
#define MATH_RAY_HIT

#include "vector3.h"
class material;

struct ray_hit {
	vector3 point;
	vector3 normal;
	vector3 uvw;
	float t;
	material* material;
};

#endif