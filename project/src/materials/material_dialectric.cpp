#include "material_dialectric.h"

float schilick(float cosine, float refractance_index) {

	float r0 = (1.0f - refractance_index) / (1.0f + refractance_index);
	r0 = r0 * r0;

	return r0 + (1.0f - r0) * powf(1.0f - cosine, 5);
}

material_dialectric::material_dialectric(float refractance_index) :
	refractance_index(refractance_index) {
}

bool material_dialectric::scatter(const ray& incident, const ray_hit& hit, vector3& attenuation, ray& scattered) const {
	
	vector3 outward_normal;
	vector3 reflected = reflect(incident.direction, hit.normal);
	float index;
	float cosine;

	if (dot(incident.direction, hit.normal) > 0) {
		outward_normal = -hit.normal;
		index = refractance_index;
		cosine = refractance_index * dot(incident.direction, hit.normal) / incident.direction.length();
	}
	else {
		outward_normal = hit.normal;
		index = 1.0f / refractance_index;
		cosine = -dot(incident.direction, hit.normal) / incident.direction.length();
	}

	vector3 refracted;
	if (refract(normalize(incident.direction), outward_normal, index, refracted)) {
		if (normalized_random() < schilick(cosine, refractance_index)) {
			attenuation = vector3(1.0f, 1.0f, 1.0f);
			scattered = ray(hit.point, reflected);
			return true;
		}
	}

	attenuation = vector3(1.0f, 1.0f, 1.0f);
	scattered = ray(hit.point, refracted);
	return true;
}
