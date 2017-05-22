#include "renderer.h"

static float clamp(float val, float min, float max) {
	if (val > max) {
		return (max);
	}
    return (val < min ? min : val);
}

static int biomPlainGenColor(t_world * world, t_biom * biom, float wx, float wy, float wz) {

	(void)wz;
	(void)wx;
	(void)world;
	(void)biom;

	float r = (wy + world->max_height) / (2 * world->max_height);
	if (r <= 0.10f) {
		return (TX_WATER);
	} else if (r <= 0.65f) {
		return (TX_GRASS);
	} else if (r <= 0.85f) {
		return (TX_DIRT);
	} else if(r <= 0.90f) {
		return (TX_STONE);
	} else {
		return (TX_SNOW);
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

static float biomHeightmapGenHeight(t_world * world, t_biom * biom, float wx, float wz) {
	(void)biom;
	float f = world->max_height;
	int px = (int)(wx / TERRAIN_UNIT);
	int py = (int)(wz / TERRAIN_UNIT);
	px = clamp(px, 0, world->heightmap->w - 1);
	py = clamp(py, 0, world->heightmap->h - 1);

	int rgb = heightmapGetHeight(world->heightmap, px, py);
	float height = (rgb / (255.0f * 3.0f) * 2 - 1) * f;
	height = clamp(height, -f, f);
	return (height);
}

static int biomHeightmapCanGenerate(t_world * world, t_biom * biom, float wx, float wz) {
	(void)biom;
	int px = (int)(wx / TERRAIN_UNIT);
	int py = (int)(wz / TERRAIN_UNIT);
	return (px >= 0 && py >= 0 && px < world->heightmap->w && py < world->heightmap->h);
}

static int biomCanGenerate(t_world * world, t_biom * biom, float wx, float wz) {
	(void)world;
	(void)biom;
	(void)wx;
	(void)wz;
	return (1);
}

static void biomRegister(t_world * world,
							float (*heightGen)(t_world *, struct s_biom *, float, float),
							int (*colorGen)(t_world *, struct s_biom *, float, float, float),
							int (*canGen)(t_world *, struct s_biom *, float, float)) {
	t_biom biom;
	biom.heightGen = heightGen;
	biom.colorGen = colorGen;
	biom.canGenerateAt = canGen;
	array_list_add(world->bioms, &biom);
}

void biomsInit(t_world * world) {
	if (world->heightmap == NULL) {
		printf("No heightmaps set, generating terrain procedurally\n");
		biomRegister(world, biomPlainGenHeight, biomPlainGenColor, biomCanGenerate);
	} else {
		printf("Heightmap in use\n");
		biomRegister(world, biomHeightmapGenHeight, biomPlainGenColor, biomHeightmapCanGenerate);
	}
}

void biomsDelete(t_world * world) {
	array_list_clear(world->bioms);
}
