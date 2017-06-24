#ifndef WEBRAY_SPHERE_H
#define WEBRAY_SPHERE_H

#include "entity.h"

class sphere : public entity {

public:
	sphere() {
	}

	sphere(const vector3& center, float radius, material* material) : 
		center(center),
		radius(radius),
		material(material){
	}

	sphere(const sphere& sphere) :
		center(sphere.center),
		radius(sphere.radius), 
		material(sphere.material) {
	}

	virtual bool hit(const ray& r, float t_min, float t_max, ray_hit& hit) const;
	virtual bool bounding_box(aabb& aabb) const;

	vector3 center;
	float radius;
	material* material;
};

bool sphere::hit(const ray& r, float t_min, float t_max, ray_hit& hit) const {
	vector3 oc = r.origin - center;

	float a = dot(r.direction, r.direction);
	float b = dot(oc, r.direction);
	float c = dot(oc, oc) - radius * radius;

	float discriminat = b*b - a*c;
	if (discriminat > 0) {
		float s = sqrtf(b*b - a*c);
		float t = (-b - s) / a;
		if (t < t_max && t > t_min) {
			hit.point = r.pointAt(t);
			hit.normal = (hit.point - center) / radius;
			hit.t = t;
			hit.material = material;
			return true;
		}

		t = (-b + s) / a;
		if (t < t_max && t > t_min) {
			hit.point = r.pointAt(t);
			hit.normal = (hit.point - center) / radius;
			hit.t = t;
			hit.material = material;
			return true;
		}
	}
	return false;
}

bool sphere::bounding_box(aabb& bbox) const {

	bbox = ::aabb(
		center - vector3(radius, radius, radius),
		center + vector3(radius, radius, radius)
	);

	return true;
}

#endif

