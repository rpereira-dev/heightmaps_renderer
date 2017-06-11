#include "renderer.h"

static float clamp(float val, float min, float max) {
    return (val > max ? max : val < min ? min : val);
}

static int mountainGenColor(t_world * world, t_generator * generator, float wx, float wy, float wz) {
	float r = wy / world->max_height;
	if (r <= 0.08f) {
		return (TX_WATER);
	} else if (r <= 0.60f) {
		return (TX_GRASS);
	} else if (r <= 0.75f) {
		return (TX_DIRT);
	} else if(r <= 0.90f) {
		return (TX_STONE);
	return (TX_SNOW);
}

static float normalizeHeight(t_world * world, float heightFactor) {
	heightFactor += 1;
    heightFactor *= 0.5f;
    heightFactor = clamp(heightFactor, 0.0f, 1.0f);
    return (world->max_height * heightFactor);
}

/** the height generator function for moutains */
static float genNoiseHeight(t_world * world, t_generator * generator, float wx, float wz) {
	float heightFactor = 0.0f;
    float frequency = generator->frequency;
    float amplitude = generator->amplitude;
    for (int i = 0 ; i < generator->octaves ; i++) {
        heightFactor += pnoise2(world->octaves[i], wx * frequency, wz * frequency) * amplitude;
        frequency *= generator->lacunarity;
        amplitude *= generator->persistance;
    }
    return (normalizeHeight(world, heightFactor));
}

static float heightmapGenHeight(t_world * world, t_generator * generator, float wx, float wz) {
	int px = (int)(wx / TERRAIN_UNIT);
	int py = (int)(wz / TERRAIN_UNIT);
	px = clamp(px, 0, world->heightmap->w - 1);
	py = clamp(py, 0, world->heightmap->h - 1);

	int rgb = heightmapGetHeight(world->heightmap, px, py);
	float height = rgb / (255.0f * 3.0f);
	return (height * world->max_height);
}

static int heightmapCanGenerate(t_world * world, t_generator * generator, float wx, float wz) {
	int px = (int)(wx / TERRAIN_UNIT);
	int py = (int)(wz / TERRAIN_UNIT);
	return (px >= 0 && py >= 0 && px < world->heightmap->w && py < world->heightmap->h);
}

static int canGenerate(t_world * world, t_generator * generator, float wx, float wz) {
	return (1);
}

static void registerGenerator(t_world * world,
							float (*heightGen)(t_world *, struct s_generator *, float, float),
							int (*colorGen)(t_world *, struct s_generator *, float, float, float),
							int (*canGen)(t_world *, struct s_generator *, float, float),
							float heightGenStep, int octaves,
							float amplitude, float persistance,
							float frequency, float lacunarity) {
	t_generator generator;
	generator.heightGen = heightGen;
	generator.colorGen = colorGen;
	generator.canGenerateAt = canGen;
	generator.heightGenStep = heightGenStep;
	generator.octaves = octaves;
	generator.amplitude = amplitude;
	generator.frequency = frequency;
	generator.persistance = persistance;
	generator.lacunarity = lacunarity;
	array_list_add(world->generators, &generator);
}

void generatorsInit(t_world * world) {
	if (world->heightmap == NULL) {
		printf("No heightmaps set, generating terrain procedurally\n");
		float step = TERRAIN_UNIT / 16.0f;
		registerGenerator(world, genNoiseHeight, mountainGenColor, canGenerate, step, 4, 1.0f, 0.5f, 0.03f, 2.0f);
	} else {
		printf("Heightmap in use\n");
		registerGenerator(world, heightmapGenHeight, mountainGenColor, heightmapCanGenerate, TERRAIN_UNIT, 0, 0, 0, 0, 0);
	}
}

void generatorsDelete(t_world * world) {
	array_list_delete(world->generators);
	free(world->generators);
}
