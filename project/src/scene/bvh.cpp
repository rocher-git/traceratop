#include <limits>
#include "../common.h"
#include "bvh.h" 

void bubble(const entity** entities, int entities_count, int axis) {
	
	aabb bbox_a, bbox_b;

	for (int c = 0; c < entities_count - 1; c++) {
		for (int d = 0; d < entities_count - c - 1; d++) {
			const entity* a = entities[d];
			const entity* b = entities[d+1];

			a->bounding_box(bbox_a);
			b->bounding_box(bbox_b);

			if (bbox_a.min[axis] > bbox_b.min[axis]) {
				entities[d  ] = b;
				entities[d+1] = a;
			}
		}
	}
}

int bbox_compare_x(const void* a, const void* b) {

	aabb l_bbox, r_bbox;

	const entity& entity_a = *static_cast<const entity*>(a);
	const entity& entity_b = *static_cast<const entity*>(b);

	entity_a.bounding_box(l_bbox);
	entity_b.bounding_box(r_bbox);

	return l_bbox.min[0] < r_bbox.min[0] ? -1 : 1;
}

int bbox_compare_y(const void* a, const void* b) {

	aabb l_bbox, r_bbox;

	const entity& entity_a = *static_cast<const entity*>(a);
	const entity& entity_b = *static_cast<const entity*>(b);

	entity_a.bounding_box(l_bbox);
	entity_b.bounding_box(r_bbox);

	return l_bbox.min[1] < r_bbox.min[1] ? -1 : 1;
}

int bbox_compare_z(const void* a, const void* b) {

	aabb l_bbox, r_bbox;

	const entity& entity_a = *static_cast<const entity*>(a);
	const entity& entity_b = *static_cast<const entity*>(b);

	entity_a.bounding_box(l_bbox);
	entity_b.bounding_box(r_bbox);

	return ((l_bbox.min[2] < r_bbox.min[2]) ? -1 : 1);
}

bvh::bvh(const entity** entities, int entities_count) :
	l_hittable(nullptr),
	r_hittable(nullptr) {
	
	int axis = int(3 * normalized_random());
	bubble(entities, entities_count, axis);

    /*switch (axis) {
		case 0:
			qsort(entities, entities_count, sizeof(entity*), bbox_compare_x);
			break;

		case 1:
			qsort(entities, entities_count, sizeof(entity*), bbox_compare_y);
			break;

		case 3:
			qsort(entities, entities_count, sizeof(entity*), bbox_compare_z);
			break;
	}*/
	
	switch (entities_count) {
		case 1:
			l_hittable = static_cast<const hittable*>(entities[0]);
			l_hittable->bounding_box(bbox);
			return;

		case 2:
			l_hittable = static_cast<const hittable*>(entities[0]);
			r_hittable = static_cast<const hittable*>(entities[1]);
			break;
			
		default:
			const int half_count = entities_count / 2;

			l_hittable = new bvh(entities, half_count);
			r_hittable = new bvh(entities + half_count, entities_count - half_count);
			break;
	}

	aabb l_bbox;
	l_hittable->bounding_box(l_bbox);

	aabb r_bbox;
	r_hittable->bounding_box(r_bbox);

	bbox = l_bbox.merge(r_bbox);
}

bool bvh::hit(const ray& r, float t_min, float t_max, ray_hit& hit) const {

	if(!bbox.hit(r, t_min, t_max)) {
		return false;
	}

	ray_hit l_hit; l_hit.t = std::numeric_limits<float>::max();
	ray_hit r_hit; r_hit.t = std::numeric_limits<float>::max();

	if((!l_hittable || !l_hittable->hit(r, t_min, t_max, l_hit)) &&
	   (!r_hittable || !r_hittable->hit(r, t_min, t_max, r_hit))) {
		return false;
	}

	hit = l_hit.t < r_hit.t ? l_hit : r_hit;
	return true;
}

bool bvh::bounding_box(aabb& bbox) const {

	bbox = this->bbox;
	return false;
}