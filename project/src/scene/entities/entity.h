#ifndef WEBRAY_ENTITY_H
#define WEBRAY_ENTITY_H

#include "../../math/ray_hit.h"
#include "../..//math/ray.h"
#include "../../math/aabb.h"
#include "../hitable.h"

class entity : public hittable {

public:
	virtual bool hit(const ray& r, float t_min, float t_max, ray_hit& hit) const = 0;
	virtual bool bounding_box(aabb& bbox) const = 0;
};

#endif