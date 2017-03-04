#include "renderer.h"

static float clamp(float val, float min, float max) {
	if (val > max) {
		return (max);
	}
    return (val < min ? min : val);
}

static void biomPlainGenColor(t_world * world, t_biom * biom, t_vec3f * color, float wx, float wy, float wz) {

	static t_vec3f blue		= {{0.1f}, {0.2f}, {0.9f}};
	static t_vec3f green 	= {{0.2f}, {0.9f}, {0.14f}};
	static t_vec3f brown	= {{0.4f}, {0.35f}, {0.25f}};
	static t_vec3f gray		= {{0.6f}, {0.6f}, {0.6f}};
	static t_vec3f white	= {{0.95f}, {0.95f}, {0.95f}};

	(void)wz;
	(void)wx;
	(void)world;
	(void)biom;

	float r = (wy + world->max_height) / (2 * world->max_height);
	if (r <= 0.10f) {
		vec3f_set3(color, &blue);
	} else if (r <= 0.15f) {
		vec3f_mix(color, &blue, &green, 1 - (r - 0.10f) / 0.05f);
	} else if (r <= 0.5f) {
		vec3f_set3(color, &green);
	} else if (r <= 0.65f) {
		vec3f_mix(color, &green, &brown, 1 - (r - 0.5f) / 0.15f);
	} else if (r <= 0.7f) {
		vec3f_set3(color, &brown);
	} else if (r <= 0.8f) {
		vec3f_mix(color, &brown, &gray, 1 - (r - 0.7f) / 0.1f);
	} else if (r <= 0.85f) {
		vec3f_set3(color, &gray);
	} else if(r <= 0.95f) {
		vec3f_mix(color, &gray, &white, 1 - (r - 0.85f) / 0.1f);
	} else {
		vec3f_set3(color, &white);
	}
}


static float biomPlainGenHeight(t_world * world, t_biom * biom, float wx, float wz) {
	(void)biom;

	float f = world->max_height;
	float d1 = 0.002f;
	float d2 = 0.01f;
	float height = 0;
	height += noise2(world->octaves[0], wx  * d1, wz * d1) * f;
	height += noise2(world->octaves[1], wx  * d2, wz * d2) * f / 16.0f;
	height = clamp(height, -f, f);
	return (height);
}

static float biomBMPGenHeight(t_world * world, t_biom * biom, float wx, float wz) {
	(void)biom;
	float f = world->max_height;
	t_heightmap * hmap = world->bmpfile;
	int px = (int)(wx / (TERRAIN_SIZE * TERRAIN_DETAIL));
	int py = (int)(wy / (TERRAIN_SIZE * TERRAIN_DETAIL));
	px = clamp(px, 0, hmap->w);
	py = clamp(py, 0, hmap->h);
	float height = heightmapGetHeight(hmap, px, py);
	height = clamp(height, -f, f);
	return (height);
}

static void biomRegister(t_world * world,
							float (*heightGen)(t_world *, struct s_biom *, float, float),
							void (*colorGen)(t_world *, struct s_biom *, t_vec3f *, float, float, float)) {
	t_biom biom;
	biom.heightGen = heightGen;
	biom.colorGen = colorGen;
	array_list_add(world->bioms, &biom);
}

void biomsInit(t_world * world) {
	if (world->bmpfile == NULL) {
		biomRegister(world, biomPlainGenHeight, biomPlainGenColor);
	} else {
		biomRegister(world, biomBMPGenHeight, biomPlainGenColor);
	}
}

void biomsDelete(t_world * world) {
	array_list_clear(world->bioms);
}
