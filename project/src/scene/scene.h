#ifndef WEBRAY_SCENE_H
#define WEBRAY_SCENE_H

#include "bvh.h"
#include "entities\entity.h"

class scene {

public:
	scene(const entity** entities, int entities_count);
	bool bouding_box(aabb& bbox) const;
	bool hit(const ray& ray, ray_hit& ray_hit) const;

	bool use_bvh;

	vector3 u_color;
	vector3 d_color;

	const bvh bvh;
	const entity** entities;
	const int entities_count;
};

#endif