#include "renderer.h"

static float clamp(float val, float min, float max) {
	if (val > max) {
		return (max);
	}
    return (val < min ? min : val);
}

static int biomMountainGenColor(t_world * world, t_biom * biom, float wx, float wy, float wz) {
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

static float normalizeHeight(t_world * world, float heightFactor) {
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

/** the height generator function for moutains */
static float biomGenHeight(t_world * world, t_biom * biom, float wx, float wz) {

	float heightFactor = 0.0f;

    float frequency = biom->frequency;
    float amplitude = biom->amplitude;
    for (int i = 0 ; i < biom->octaves ; i++) {
        heightFactor += pnoise2(world->octaves[i], wx * frequency, wz * frequency) * amplitude;
        frequency *= biom->lacunarity;
        amplitude *= biom->persistance;
    }
    return (normalizeHeight(world, heightFactor));
}

static float biomHeightmapGenHeight(t_world * world, t_biom * biom, float wx, float wz) {
	int px = (int)(wx / TERRAIN_UNIT);
	int py = (int)(wz / TERRAIN_UNIT);
	px = clamp(px, 0, world->heightmap->w - 1);
	py = clamp(py, 0, world->heightmap->h - 1);

	int rgb = heightmapGetHeight(world->heightmap, px, py);
	float height = rgb / (255.0f * 3.0f);
	return (height * world->max_height);
}

static int biomHeightmapCanGenerate(t_world * world, t_biom * biom, float wx, float wz) {
	int px = (int)(wx / TERRAIN_UNIT);
	int py = (int)(wz / TERRAIN_UNIT);
	return (px >= 0 && py >= 0 && px < world->heightmap->w && py < world->heightmap->h);
}

static int biomCanGenerate(t_world * world, t_biom * biom, float wx, float wz) {
	return (1);
}

static void biomRegister(t_world * world,
							float (*heightGen)(t_world *, struct s_biom *, float, float),
							int (*colorGen)(t_world *, struct s_biom *, float, float, float),
							int (*canGen)(t_world *, struct s_biom *, float, float),
							float heightGenStep, int octaves,
							float amplitude, float persistance,
							float frequency, float lacunarity) {
	t_biom biom;
	biom.heightGen = heightGen;
	biom.colorGen = colorGen;
	biom.canGenerateAt = canGen;
	biom.heightGenStep = heightGenStep;
	biom.octaves = octaves;
	biom.amplitude = amplitude;
	biom.frequency = frequency;
	biom.persistance = persistance;
	biom.lacunarity = lacunarity;
	array_list_add(world->bioms, &biom);
}

void biomsInit(t_world * world) {
	if (world->heightmap == NULL) {
		printf("No heightmaps set, generating terrain procedurally\n");
		float step = TERRAIN_UNIT / 16.0f;
		biomRegister(world, biomGenHeight, biomMountainGenColor, biomCanGenerate, step, 4, 1.0f, 0.5f, 0.03f, 2.0f);
	} else {
		printf("Heightmap in use\n");
		biomRegister(world, biomHeightmapGenHeight, biomMountainGenColor, biomHeightmapCanGenerate, TERRAIN_UNIT, 0, 0, 0, 0, 0);
	}
}

void biomsDelete(t_world * world) {
	array_list_delete(world->bioms);
	free(world->bioms);
}
