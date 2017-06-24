#ifndef CAMEARAS_CAMERA
#define CAMEARAS_CAMERA

#include "../math/ray.h"

class camera {

public:
	camera() :
		vfov(PI/2.0f), 
		aspect(1.0f),
		aperture(0.1f),
		focus_distance(2.0f),
		eye(0.0f, 0.0f, 1.0f),
		at(0.0f, 0.0f, -1.0f),
		vup(0.0f, 1.0f, 0.0f) {
		update();
	}

	inline void set_vfov(float vfov) { 
		this->vfov = vfov;
	}

	inline void set_aspect(float aspect) {
		this->aspect = aspect;
	}

	inline void set_lens(float aperture, float focus_distance) {
		this->aperture = aperture;
		this->focus_distance = focus_distance;
		this->lens_radius = aperture / 2.0f;
	}

	inline void set_look_up(const vector3& eye, const vector3& at, const vector3& vup) {
		this->eye = eye;
		this->at = at;
		this->vup = vup;
	}

	void update() {
		float half_height = tanf(vfov / 2);
		float half_width = aspect * half_height;

		w = normalize(eye - at);
		u = normalize(cross(vup, w));
		v = normalize(cross(w, u));

		lower_left_corner = eye - half_width * focus_distance * u - half_height * focus_distance * v - focus_distance * w;

		horizontal = 2.0f * half_width * focus_distance * u;
		vertical = 2.0f * half_height * focus_distance * v;
	}

	ray get_ray(float s, float t) const {
		vector3 radius = lens_radius * random_in_unit_disk();
		vector3 offset = u * radius.x() + v * radius.y();
		return ray(eye + offset, lower_left_corner + s * horizontal + t * vertical - eye - offset);
	};

	float vfov; // vfov is top to bottom is radians.
	float aspect;
	
	float aperture;
	float focus_distance;
	float lens_radius;

	vector3 eye;
	vector3 at;
	vector3 vup;

	vector3 w;
	vector3 u;
	vector3 v;
	vector3 lower_left_corner;
	vector3 horizontal;
	vector3 vertical;
};

#endif

