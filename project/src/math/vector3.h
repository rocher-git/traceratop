#ifndef WEBRAY_VECTOR_3_H
#define WEBRAY_VECTOR_3_H

#include <math.h>

class vector3 {

public:
	vector3() {
	}
	
	vector3(float x, float y, float z) {
		e[0] = x;
		e[1] = y;
		e[2] = z;
	}

	vector3(const vector3& v1) {
		e[0] = v1.e[0];
		e[1] = v1.e[1];
		e[2] = v1.e[2];
	}

	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }

	inline float r() const { return e[0]; }
	inline float g() const { return e[1]; }
	inline float b() const { return e[2]; }

	inline const vector3& operator+() const { return *this; }
	inline vector3 operator-() const { return vector3(-e[0], -e[1], -e[2]); }

	inline float operator[](int i) const { return e[i]; }
	inline float& operator[](int i) { return e[i]; }

	inline vector3& operator=(const vector3& v1);
	inline vector3& operator+=(const vector3& v1);
	inline vector3& operator-=(const vector3& v1);
	inline vector3& operator*=(const vector3& v1);
	inline vector3& operator/=(const vector3& v1);
	inline vector3& operator*=(const float s);
	inline vector3& operator/=(const float s);

	inline vector3& normalize();

	inline float length() const;
	inline float length_squared() const;

	float e[3];
};

inline vector3& vector3::operator=(const vector3& v1) {
	e[0] = v1.e[0];
	e[1] = v1.e[1];
	e[2] = v1.e[2];

	return *this;
}

inline vector3& vector3::operator+=(const vector3 &v1) {
	e[0] += v1.e[0];
	e[1] += v1.e[1];
	e[2] += v1.e[2];

	return *this;
}

inline vector3& vector3::operator-=(const vector3 &v1) {
	e[0] -= v1.e[0];
	e[1] -= v1.e[1];
	e[2] -= v1.e[2];

	return *this;
}

inline vector3& vector3::operator*=(const vector3 &v1) {
	e[0] *= v1.e[0];
	e[1] *= v1.e[1];
	e[2] *= v1.e[2];

	return *this;
}

inline vector3& vector3::operator/=(const vector3 &v1) {
	e[0] /= v1.e[0];
	e[1] /= v1.e[1];
	e[2] /= v1.e[2];

	return *this;
}

inline vector3& vector3::operator*=(const float s) {
	e[0] *= s;
	e[1] *= s;
	e[2] *= s;

	return *this;
}

inline vector3& vector3::operator/=(const float s) {
	const float d = 1.0f / s;

	e[0] *= d;
	e[1] *= d;
	e[2] *= d;

	return *this;
}

inline vector3& vector3::normalize() {
	const float n = 1.0f / length();

	e[0] *= n;
	e[1] *= n;
	e[2] *= n;

	return *this;
}

inline float vector3::length() const {
	return (float)sqrt(
		e[0] * e[0] + 
		e[1] * e[1] +
		e[2] * e[2]
		);
}

inline float vector3::length_squared() const {
	return (
		e[0] * e[0] +
		e[1] * e[1] +
		e[2] * e[2]
		);
}

inline vector3 operator+(const vector3& v1, const vector3& v2) {
	return vector3(
		v1.e[0] + v2.e[0],
		v1.e[1] + v2.e[1],
		v1.e[2] + v2.e[2]
		);
}

inline vector3 operator-(const vector3& v1, const vector3& v2) {
	return vector3(
		v1.e[0] - v2.e[0],
		v1.e[1] - v2.e[1],
		v1.e[2] - v2.e[2]
		);
}

inline vector3 operator*(const vector3& v1, const vector3& v2) {
	return vector3(
		v1.e[0] * v2.e[0],
		v1.e[1] * v2.e[1],
		v1.e[2] * v2.e[2]
		);
}

inline vector3 operator/(const vector3& v1, const vector3& v2) {
	return vector3(
		v1.e[0] / v2.e[0],
		v1.e[1] / v2.e[1],
		v1.e[2] / v2.e[2]
		);
}

inline vector3 operator*(const vector3& v1, const float s) {
	return vector3(
		v1.e[0] * s,
		v1.e[1] * s,
		v1.e[2] * s
		);
}

inline vector3 operator/(const vector3& v1, const float s) {
	const float d = 1.0f / s;

	return vector3(
		v1.e[0] * d,
		v1.e[1] * d,
		v1.e[2] * d
		);
}

inline vector3 operator*(const float s, const vector3& v1) {
	return vector3(
		v1.e[0] * s,
		v1.e[1] * s,
		v1.e[2] * s
		);
}

inline vector3 operator/(const float s, const vector3& v1) {
	const float d = 1.0f / s;

	return vector3(
		v1.e[0] * d,
		v1.e[1] * d,
		v1.e[2] * d
		);
}

inline float dot(const vector3& v1, const vector3& v2) {
	return (
		v1.e[0] * v2.e[0] +
		v1.e[1] * v2.e[1] +
		v1.e[2] * v2.e[2]
		);
}

inline vector3 cross(const vector3& v1, const vector3& v2) {
	return vector3(
		v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
	   -v1.e[0] * v2.e[2] + v1.e[2] * v2.e[0], 
		v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0] 
		);
}

inline vector3 normalize(const vector3& v1) {

	return vector3(v1).normalize();
}

inline vector3 reflect(const vector3& v1, const vector3& n) {

	return v1  - 2.0f * dot(v1, n) * n;
}

inline bool refract(const vector3& v1, const vector3& n, float index, vector3& refracted) {

	vector3 uv = normalize(v1);

	float dt = dot(uv, n);
	float discriminant = 1.0f - index * index * (1.0f - dt * dt);

	if (discriminant <= 0) {
		return false;
	}

	refracted = index * (uv - n * dt) - n * sqrtf(discriminant);
	return true;
}

#endif

