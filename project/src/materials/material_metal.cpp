#include "material_metal.h"

material_metal::material_metal(const texture* albedo, float fuzz) :
	albedo(albedo),
	fuzz(fuzz) {

	if (fuzz > 1.0f) {
		fuzz = 1.0f;
	}
}

bool material_metal::scatter(const ray& incident, const ray_hit& hit, vector3& attenuation, ray& scattered) const {
	
	vector3 reflected = reflect(normalize(incident.direction), hit.normal);

	//Calculate the texture coordinate for a sphere
	float temp1 = acosf(hit.normal.z());
	float temp2 = atan2f(hit.normal[1], hit.normal[0]);

	//u,v 
	float v = temp1 / PI;

	if (temp2 < 0) {
		temp2 = temp2 + (2 * PI);
	}

	float u = temp2 / (2 * PI);

	scattered = ray(hit.point, reflected + fuzz * random_in_unit_sphere());
	attenuation = albedo->sample(u, v, hit.point);

	return dot(scattered.direction, hit.normal) > 0;
}