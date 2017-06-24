#ifndef MATERIALS_METAL
#define MATERIALS_METAL

#include "..\common.h"
#include "..\textures\texture.h"
#include "material.h"

class material_metal : public material {

public:
	material_metal(const texture* albedo, float fuzz);
	virtual bool scatter(const ray& incident, const ray_hit& hit, vector3& attenuation, ray& scattered) const;

	const texture* albedo;
	float fuzz;
};

#endif

