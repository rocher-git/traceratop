#ifndef MATERIALS_LAMBERTIAN
#define MATERIALS_LAMBERTIAN

#include "..\common.h"
#include "..\textures\texture.h"
#include "material.h"

class material_lambertian : public material {

public:
	material_lambertian(const texture* albedo);
	virtual bool scatter(const ray& incident, const ray_hit& hit, vector3& attenuation, ray& scattered) const;

	const texture* albedo;
};

#endif

