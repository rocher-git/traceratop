#ifndef WEBRAY_HITTABLE_H
#define WEBRAY_HITTABLE_H

#include "../math/ray.h"
#include "../math/ray_hit.h"
#include "../math/aabb.h"

class hittable {

public:
	virtual bool hit(const ray& r, float t_min, float t_max, ray_hit& hit) const = 0;
	virtual bool bounding_box(aabb& bbox) const = 0;
};

#endif
