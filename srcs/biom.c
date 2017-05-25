#include "renderer.h"

static float clamp(float val, float min, float max) {
	if (val > max) {
		return (max);
	}
    return (val < min ? min : val);
}

static int biomMountainGenColor(t_world * world, t_biom * biom, float wx, float wy, float wz) {

	(void)wz;
	(void)wx;
	(void)world;
	(void)biom;

	float r = wy / world->max_height;
	if (r <= 0.08f) {
		return (TX_WATER);
	} else if (r <= 0.60f) {
		return (TX_GRASS);
	} else if (r <= 0.75f) {
		return (TX_DIRT);
	} else if(r <= 0.90f) {
		return (TX_STONE);
	} else {
		return (TX_SNOW);
	}
}

static float biomMountainGenHeight(t_world * world, t_biom * biom, float wx, float wz) {
	(void)biom;
/*
	float f = world->max_height;
	float d1 = 0.005f;
	float d2 = 0.01f;
	float height = 0;
	height += noise2(world->octaves[0], wx  * d1, wz * d1) * f;
	height += noise2(world->octaves[1], wx  * d2, wz * d2) * f / 16.0f;
	height = clamp(height, -f, f);
*/


	float heightFactor = 0.0f;

    float layerF = 0.04f;
    float weight = 1.0f;
    for (int i = 0 ; i < 5 ; i++)
    {
        heightFactor += pnoise2(world->octaves[i], wx * layerF, wz * layerF) * weight;
        layerF *= 2.0f;
        weight *= 0.5f;
    }

    heightFactor += 1;
    heightFactor *= 0.5f;

    float minHeight = 0.08f;
    float maxHeight = 1.0f;
    if (heightFactor < minHeight) {
    	heightFactor = minHeight;
    } else if (heightFactor > maxHeight) {
    	heightFactor = maxHeight;
    }

	return (world->max_height * heightFactor);
}

static float biomHeightmapGenHeight(t_world * world, t_biom * biom, float wx, float wz) {
	(void)biom;

	float f = world->max_height;
	int px = (int)(wx / TERRAIN_UNIT);
	int py = (int)(wz / TERRAIN_UNIT);
	px = clamp(px, 0, world->heightmap->w - 1);
	py = clamp(py, 0, world->heightmap->h - 1);

	int rgb = heightmapGetHeight(world->heightmap, px, py);
	float height = rgb / (255.0f * 3.0f) * f;
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
							float heightGenStep,
							int (*colorGen)(t_world *, struct s_biom *, float, float, float),
							int (*canGen)(t_world *, struct s_biom *, float, float)) {
	t_biom biom;
	biom.heightGen = heightGen;
	biom.heightGenStep = heightGenStep;
	biom.colorGen = colorGen;
	biom.canGenerateAt = canGen;
	array_list_add(world->bioms, &biom);
}

void biomsInit(t_world * world) {
	if (world->heightmap == NULL) {
		printf("No heightmaps set, generating terrain procedurally\n");
		biomRegister(world, biomMountainGenHeight, TERRAIN_UNIT / 16.0f, biomMountainGenColor, biomCanGenerate);
	} else {
		printf("Heightmap in use\n");
		biomRegister(world, biomHeightmapGenHeight, TERRAIN_UNIT, biomMountainGenColor, biomHeightmapCanGenerate);
	}
}

void biomsDelete(t_world * world) {
	array_list_delete(world->bioms);
	free(world->bioms);
}
