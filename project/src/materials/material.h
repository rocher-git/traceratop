#ifndef MATERIALS_MATERIAL
#define MATERIALS_MATERIAL

#include "../math/ray.h"
#include "../math/ray_hit.h"

class material {

public:
	virtual bool scatter(const ray& incident, const ray_hit& hit, vector3& attenuation, ray& scattered) const = 0;
};

#endif

