#include "material_lambertian.h"

material_lambertian::material_lambertian(const texture* albedo) :
	albedo(albedo) {
}

bool material_lambertian::scatter(const ray& incident, const ray_hit& hit, vector3& attenuation, ray& scattered) const {
	
	vector3 target = hit.point + hit.normal + random_in_unit_sphere();

	//Calculate the texture coordinate for a sphere
	float temp1 = acosf(hit.normal.z());
	float temp2 = atan2f(hit.normal[1], hit.normal[0]);

	//u,v 
	float v = temp1 / PI;

	if (temp2 < 0) {
		temp2 = temp2 + (2 * PI);
	}

	float u = temp2 / (2 * PI);

	scattered = ray(hit.point, target - hit.point);
	attenuation = albedo->sample(u, v, target);	
	return true;
}