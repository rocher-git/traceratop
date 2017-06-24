#include "aabb.h"

bool aabb::hit(const ray& ray, float tmin, float tmax) const {
	
	for (int i = 0; i < 3; ++i) {
		float invDirection = 1.0f / ray.direction[i];
		float t0 = (min[i] - ray.origin[i]) * invDirection;
		float t1 = (max[i] - ray.origin[i]) * invDirection;

		if (invDirection < 0.0f) {
			float tx = t1;

			t1 = t0;
			t0 = tx;
		}

		tmin = t0 > tmin ? t0 : tmin;
		tmax = t1 < tmax ? t1 : tmax;

		if (tmax <= tmin) {
			return false;
		}
	}
	return true;
}

aabb& aabb::merge(const aabb& aabb) {

	min[0] = fminf(min[0], aabb.min[0]);
	min[1] = fminf(min[1], aabb.min[1]);
	min[2] = fminf(min[2], aabb.min[2]);

	max[0] = fmaxf(max[0], aabb.max[0]);
	max[1] = fmaxf(max[1], aabb.max[1]);
	max[2] = fmaxf(max[2], aabb.max[2]);

	return *this;
}

aabb& merge(const aabb& a, const aabb& b) {

	return aabb(a).merge(b);
}