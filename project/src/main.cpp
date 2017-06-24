#include <stdio.h>
#include <string.h>
#include <math.h>
#include "common.h"
#include "math\vector3.h"
#include "math\ray.h"
#include "cameras\camera.h"
#include "textures\texture_constant.h"
#include "textures\texture_checker.h"
#include "materials\material_lambertian.h"
#include "materials\material_metal.h"
#include "materials\material_dialectric.h"
#include "scene\scene.h"
#include "scene\entities\entity.h"
#include "scene\entities\sphere.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include "common.cpp"
#include "math\aabb.cpp"
#include "textures\texture_constant.cpp"
#include "textures\texture_checker.cpp"
#include "materials\material_lambertian.cpp"
#include "materials\material_metal.cpp"
#include "materials\material_dialectric.cpp"
#include "scene\bvh.cpp"
#include "scene\scene.cpp"
extern "C" {
	extern void Log(int id, int n);
	extern void WorkerReady();
	extern void SetPixel(int workderId, int renderId, int x, int y, int r, int g, int b, int flush);

	extern int SetWorkerId(void* data, int size);
	extern int SetViewport(void* data, int size);
	extern int SetRenderArea(void* data, int size);
	extern int Clear(void* data, int size);
	extern int Render(void* data, int size);
}
#else
#define EMSCRIPTEN_KEEPALIVE
extern "C" {
	extern void Log(int id, int n) {};
	extern void WorkerReady() {};
	extern void SetPixel(int workderId, int renderId, int x, int y, int r, int g, int b, int flush) {};

	extern int SetWorkerId(void* data, int size);
	extern int SetViewport(void* data, int size);
	extern int SetRenderArea(void* data, int size);
	extern int Clear(void* data, int size);
	extern int Render(void* data, int size);
}
#endif

#define MAX_FLOAT				100000.0f

// Render configurations.
int WorkerId = 0;

int ViewportW = 0;
int ViewportH = 0;

int ViewporRenderAreaX = 0;
int ViewporRenderAreaY = 0;
int ViewporRenderAreaW = 1;
int ViewporRenderAreaH = 1;

int pixels_rendered_length = ViewporRenderAreaW * ViewporRenderAreaH;
char* pixels_rendered = new char[pixels_rendered_length];

// Scenes.
sphere* create_constant_labertian_sphere(float x, float y, float z, float radius, float r, float g, float b) {

	material* material = new material_lambertian(new texture_constant(vector3(r, g, b)));
	return new sphere(vector3(x, y, z), radius, material);
}

sphere* create_checker_labertian_sphere(float x, float y, float z, float radius, float s, float ro, float go, float bo, float re, float ge, float be) {

	material* material = new material_lambertian(new texture_checker(s, new texture_constant(ro, go, bo), new texture_constant(re, ge, be)));
	return new sphere(vector3(x, y, z), radius, material);
}

sphere* create_checker_metal_sphere(float x, float y, float z, float radius, float s, float ro, float go, float bo, float re, float ge, float be, float f) {

	material* material = new material_metal(new texture_checker(s, new texture_constant(ro, go, bo), new texture_constant(re, ge, be)), f);
	return new sphere(vector3(x, y, z), radius, material);
}

sphere* create_constant_metal_sphere(float x, float y, float z, float radius, float r, float g, float b, float f) {

	material* material = new material_metal(new texture_constant(vector3(r, g, b)), f);
	return new sphere(vector3(x, y, z), radius, material);
}

sphere* create_dialectric_sphere(float x, float y, float z, float radius, float r) {

	material* material = new material_dialectric(r);
	return new sphere(vector3(x, y, z), radius, material);
}

const entity* es1[] = {
	create_constant_labertian_sphere(0.0f, -100.5f, -1.0f, 100.0f, 0.8f, 0.8f, 0.8f),
	create_constant_labertian_sphere(-1.0, 0.0f,-1.0f, 0.5f, 0.0f, 0.0f, 1.0f),
	create_constant_labertian_sphere( 0.0, 0.0f,-1.0f, 0.5f, 0.0f, 1.0f, 0.0f),
	create_constant_labertian_sphere( 1.0, 0.0f,-1.0f, 0.5f, 1.0f, 0.0f, 0.0f),
};

const entity* es2[] = {
	create_constant_metal_sphere(0.0f, -1000.5f, -1.0f, 1000.0f, 0.8f, 0.3f, 0.3f, 0.4f),
	create_constant_metal_sphere(-1.0f, 0.0f, -1.0f, 0.5f, 0.1f, 0.1f, 0.1f, 0.1f),
	create_constant_metal_sphere(1.0f, 0.0f, -1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 0.1f),
	create_dialectric_sphere(0.0f, 0.0f, -1.0f, 0.5f, 1.5f),
	create_dialectric_sphere(0.0f, 0.0f, -1.0f,-0.499f, 1.5f)
};

const float separation = 1.6f;
const float high = -0.10f;
const float radius = 0.4f;
const entity* es3[] = {
	create_checker_metal_sphere(0.0f, -1000.5f, 1.0f, 1000.0f, 2000.0f, 0.8f, 0.8f, 0.8f, 0.3f, 0.0f, 0.0f, 0.2f),

	create_constant_metal_sphere(-3.5f * separation, high, 5.4f,    radius,    0.3f, 0.0f, 0.0f, 0.0f),
	create_constant_metal_sphere(-2.5f * separation, high, 5.4f,    radius,    0.3f, 0.0f, 0.0f, 0.0f),
	create_constant_metal_sphere(-1.5f * separation, high, 5.4f,    radius,    0.3f, 0.0f, 0.0f, 0.0f),
	create_constant_metal_sphere(-0.5f * separation, high, 5.4f,    radius,    0.3f, 0.0f, 0.0f, 0.0f),
	create_constant_metal_sphere( 0.5f * separation, high, 5.4f,    radius,    0.3f, 0.0f, 0.0f, 0.0f),
	create_constant_metal_sphere( 1.5f * separation, high, 5.4f,    radius,    0.3f, 0.0f, 0.0f, 0.0f),
	create_constant_metal_sphere( 2.5f * separation, high, 5.4f,    radius,    0.3f, 0.0f, 0.0f, 0.0f),
	create_constant_metal_sphere( 3.5f * separation, high, 5.4f,    radius,    0.3f, 0.0f, 0.0f, 0.0f),

	create_constant_metal_sphere(-3.5f * separation, high,-0.9f,    radius,    1.0f, 1.0f, 1.0f, 0.0f),
	create_constant_metal_sphere(-2.5f * separation, high,-0.9f,    radius,    1.0f, 1.0f, 1.0f, 0.0f),
	create_constant_metal_sphere(-1.5f * separation, high,-0.9f,    radius,    1.0f, 1.0f, 1.0f, 0.0f),
	create_constant_metal_sphere(-0.5f * separation, high,-0.9f,    radius,    1.0f, 1.0f, 1.0f, 0.0f),
	create_constant_metal_sphere( 0.5f * separation, high,-0.9f,    radius,    1.0f, 1.0f, 1.0f, 0.0f),
	create_constant_metal_sphere( 1.5f * separation, high,-0.9f,    radius,    1.0f, 1.0f, 1.0f, 0.0f),
	create_constant_metal_sphere( 2.5f * separation, high,-0.9f,    radius,    1.0f, 1.0f, 1.0f, 0.0f),
	create_constant_metal_sphere( 3.5f * separation, high,-0.9f,    radius,    1.0f, 1.0f, 1.0f, 0.0f),
};

scene* scenes[] = {
	new scene(es1, 4),
	new scene(es2, 5),
	new scene(es3, 17),
};

int main(int argc, char *argv[]) {

	scenes[0]->d_color = vector3(1.0f, 1.0f, 1.0f);
	scenes[0]->u_color = vector3(0.5f, 0.7f, 1.0f);

	scenes[1]->d_color = vector3(1.0f, 0.5f, 0.5f);
	scenes[1]->u_color = vector3(0.1f, 0.1f, 1.0f);

	scenes[2]->d_color = vector3(1.0f, 0, 0);
	scenes[2]->u_color = vector3(1.5f, 1.5f, 1.5f);

	WorkerReady();
	return 0;
}

vector3 shade(const ray& r, const scene& scene, int maxRayBounces, int depth) {
	
	ray_hit hit;

	if (scene.hit(r, hit)) {
		ray scattered;
		vector3 attenuation;
		if (depth < maxRayBounces && hit.material->scatter(r, hit, attenuation, scattered)) {
			return attenuation * shade(scattered, scene, maxRayBounces, depth + 1);
		}
		return vector3(0.0f, 0.0f, 0.0f);
	}
	else {
		vector3 direction(r.direction);
		direction.normalize();
		float t = 0.5f * (direction.y() + 1.0f);
		return (1.0f - t) * scene.d_color + t * scene.u_color;
	}
}

// Exports. 
extern "C" {
	int EMSCRIPTEN_KEEPALIVE SetWorkerId(void* data, int size) {
		int* paramters = (int*)data;

		WorkerId = paramters[0];
		return 0;
	}

	int EMSCRIPTEN_KEEPALIVE SetViewport(void* data, int size) {
		int* paramters = (int*)data;

		ViewportW = paramters[0];
		ViewportH = paramters[1];
		return 0;
	}

	int EMSCRIPTEN_KEEPALIVE SetViewportRenderArea(void* data, int size) {
		int* paramters = (int*)data;

		ViewporRenderAreaX = paramters[0];
		ViewporRenderAreaY = paramters[1];
		ViewporRenderAreaW = paramters[2];
		ViewporRenderAreaH = paramters[3];

		delete[] pixels_rendered;

		pixels_rendered_length = ViewporRenderAreaW * ViewporRenderAreaH;
		pixels_rendered = new char[pixels_rendered_length];
		memset(pixels_rendered, 0, sizeof(char) * pixels_rendered_length);

		return 0;
	}

	int EMSCRIPTEN_KEEPALIVE Clear(void* data, int size) {
		memset(pixels_rendered, 0, sizeof(char) * pixels_rendered_length);
		return 0;
	}

	int EMSCRIPTEN_KEEPALIVE Render(void* data, int size) {
		int* parameters = (int*)data;

		const int renderId = parameters[0];
		const int sceneIndex = parameters[1];

		vector3 ey(
			parameters[ 2] + parameters[ 3] / 100000.0f,
			parameters[ 4] + parameters[ 5] / 100000.0f,
			parameters[ 6] + parameters[ 7] / 100000.0f);

		vector3 at(
			parameters[ 8] + parameters[ 9] / 100000.0f,
			parameters[10] + parameters[11] / 100000.0f,
			parameters[12] + parameters[13] / 100000.0f);

		vector3 up(
			parameters[14] + parameters[15] / 100000.0f,
			parameters[16] + parameters[17] / 100000.0f,
			parameters[18] + parameters[19] / 100000.0f);

		const int raysToRender = parameters[20];
		const int pixelSize = parameters[21];
		const int samplesPerPixel = parameters[22];
		const int maxRayBounces = parameters[23];

		const int xO = ViewporRenderAreaX;
		const int yO = ViewporRenderAreaY;
		const int xF = ViewporRenderAreaX + ViewporRenderAreaW;
		const int yF = ViewporRenderAreaY + ViewporRenderAreaH;

		const int viewpor_w_pixel_count = int(ceilf(float(ViewporRenderAreaW) / float(pixelSize)));
		const int viewpor_h_pixel_count = int(ceilf(float(ViewporRenderAreaH) / float(pixelSize)));

		camera camera;
		camera.set_vfov(90.0f * PI / 180.0f);
		camera.set_aspect(float(ViewportW) / float(ViewportH));
		camera.set_look_up(ey, at, up);
		camera.set_lens(0.0001f, 1.0f/*(camera.eye-camera.at).length()*/);
		//		camera.set_lens(0.5f, (camera.eye - camera.at).length());
		camera.update();

		for (int i = 0; i < raysToRender; ++i) {

			// Find pixel to render or return.
			int pixels_to_render = viewpor_w_pixel_count * viewpor_h_pixel_count;
			int p = int(normalized_random() * pixels_to_render);
			int e = p;
			
			while (pixels_rendered[p]) {
				p++;
				if (p == pixels_to_render) {
					p = 0;
				}
				if (e == p) {
					SetPixel(WorkerId, renderId, 0, 0, 0, 0, 0, 1);
					return 0;
				}
			}

			pixels_rendered[p] = 1;

			// Render pixel.
			float x = xO + int(p % (viewpor_w_pixel_count) * pixelSize) + float(pixelSize / 2);
			float y = yO + int(p / (viewpor_w_pixel_count) * pixelSize) + float(pixelSize / 2);

			vector3 color(0, 0, 0);

			for (int s = 0; s < samplesPerPixel; ++s) {
				float u = float(x + normalized_random()) / float(ViewportW);
				float v = float(y + normalized_random()) / float(ViewportH);

				ray ray = camera.get_ray(u, v);
				color += shade(ray, *scenes[sceneIndex], maxRayBounces, 0);
			}

			color /= float(samplesPerPixel);
			color = vector3(sqrtf(color[0]), sqrtf(color[1]), sqrtf(color[2]));

			SetPixel(WorkerId, renderId, int(x), int(y), int(255.99f*color[0]), int(255.99f*color[1]), int(255.99f*color[2]), 0);
		}

		SetPixel(WorkerId, renderId, 0, 0, 0, 0, 0, 1);
		return 0;
	}
}