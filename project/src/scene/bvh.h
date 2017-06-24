#ifndef WEBRAY_BVH_H
#define WEBRAY_BVH_H

#include "../common.h"
#include "hitable.h"
#include "entities/entity.h"

class bvh : public hittable {

public:
	bvh(const entity** entities, int entities_count);
	
	virtual bool hit(const ray& r, float t_min, float t_max, ray_hit& hit) const;
	virtual bool bounding_box(aabb& bbox) const;
	
	aabb bbox;
	
	const hittable* l_hittable;
	const hittable* r_hittable;
};

#endif
