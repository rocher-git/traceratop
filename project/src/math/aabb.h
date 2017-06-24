#ifndef WEBRAY_AABB_H
#define WEBRAY_AABB_H

#include "ray.h"

class aabb {

public:
	aabb() {
	};

	aabb(const vector3 min, const vector3 max) :
		min(min),
		max(max) {
	}

	bool hit(const ray& ray, float tmin, float tmax) const;
	aabb& merge(const aabb& aabb);

	vector3 min;
	vector3 max;
};

aabb& merge(const aabb& a, const aabb& b);

#endif