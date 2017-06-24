#include "common.h"
#include <random>

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

float normalized_random() {

	return distribution(generator);
}

vector3 random_in_unit_sphere() {

	vector3 point;

	do {
		point = vector3(normalized_random(), normalized_random(), normalized_random()) * 2.0f - vector3(1.0f, 1.0f, 0.0f);
	} while (point.length_squared() >= 1.0f);

	return point;
}

vector3 random_in_unit_disk() {

	vector3 point;

	do {
		point = 2.0f * vector3(normalized_random(), normalized_random(), 0.0f) * 2.0f - vector3(1.0f, 1.0f, 0.0f);
	} while (dot(point, point) >= 1.0f);

	return point;
}