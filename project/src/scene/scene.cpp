#include <limits>
#include "scene.h"

scene:: scene(const entity** entities, int entities_count) :
	use_bvh(false),
	entities(entities),
	entities_count(entities_count),
	bvh(entities, entities_count) {
};

bool scene::bouding_box(aabb& bbox) const {
	
	if (entities_count < 1 || !entities[0]->bounding_box(bbox)) {
		return false;
	}

	aabb entity_bbox;
	for (int i = 1; i < entities_count; ++i) {
		entities[i]->bounding_box(entity_bbox);
		bbox.merge(entity_bbox);
	}

	return true;
}

bool scene::hit(const ray& ray, ray_hit& hit) const {

	if (use_bvh) {
		return bvh.hit(ray, 0.001f, std::numeric_limits<float>::max(), hit);
	} else {
		ray_hit currentHit;

		bool hitFound = false;
		float closestHit = std::numeric_limits<float>::max();

		for (int e = 0; e < entities_count; ++e) {
			if (entities[e]->hit(ray, 0.001f, closestHit, currentHit) && closestHit > currentHit.t) {
				hit = currentHit;
				closestHit = currentHit.t;
				hitFound = true;
			}
		}

		return hitFound;
	}
}