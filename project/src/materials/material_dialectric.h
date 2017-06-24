#ifndef MATERIALS_DIALECTRIC
#define MATERIALS_DIALECTRIC

#include "material.h"
#include "..\common.h"

class material_dialectric : public material {

public:
	material_dialectric(float refractance);
	virtual bool scatter(const ray& incident, const ray_hit& hit, vector3& attenuation, ray& scattered) const;

	float refractance_index;
};

#endif

