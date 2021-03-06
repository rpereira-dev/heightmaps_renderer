#include "renderer.h"

static void worldLoadHeightmap(t_world * world, char * file) {
	if (file == NULL) {
		world->heightmap = NULL;
	} else {
		world->heightmap = imageNew(file);
	}
}

static unsigned int world_vec2i_hash(t_vec2i * vec) {
	unsigned int hash = vec->x * (TERRAIN_KEEP_LOADED_DISTANCE * 2 + 1) + vec->y;
	return (hash);
}

void worldInit(t_world * world, char * bmpfile, float max_height, long seed) {
	glhCheckError("pre worldInit()");

	world->time = 0;
	world->max_height = max_height;
	worldLoadHeightmap(world, bmpfile);
	world->bioms = array_list_new(16, sizeof(t_biom));
	biomsInit(world);

	//create the terrain hash map
	world->terrains = hmap_new(TERRAIN_KEEP_LOADED_DISTANCE * 4, (t_hf)world_vec2i_hash, (t_cmpf)vec2i_nequals, (t_f)NULL, (t_f)NULL);
	if (world->terrains == NULL) {
		fprintf(stderr, "world.c : l.10 : worldInit() : not enough memory\n");
		return ;
	}

	//noise creation
	int i;
	for (i = 0 ; i < WORLD_OCTAVES ; i++) {
		world->octaves[i] = noiseNew();
		noiseNextInt(&seed);
		noiseSeed(world->octaves[i], seed);
	}

	glhCheckError("post worldInit()");
}

void worldDelete(t_world * world) {
	if (world->heightmap != NULL) {
		imageDelete(world->heightmap);
	}

	HMAP_ITER_START(world->terrains, t_terrain *, terrain) {
		terrainDelete(terrain);
	}
	HMAP_ITER_END(world->terrains, t_terrain *, terrain);

	hmap_delete(world->terrains);
	free(world->terrains);

	int i;
	for (i = 0 ; i < WORLD_OCTAVES ; i++) {
		noiseDelete(world->octaves[i]);
	}

	biomsDelete(world);
}

t_biom * worldGetBiomAt(t_world * world, float wx, float wz) {
	float noise = (snoise2(world->octaves[0], wx * 0.002f, wz * 0.002f) + 1.0f) * 0.5f;
	int id = (int) (noise * world->bioms->size);
	return (array_list_get(world->bioms, id));
}

void worldGetGridIndex(t_world * world, float worldX, float worldZ, int * gridX, int * gridY) {
	*gridX = (int)worldX / TERRAIN_SIZE;
	*gridY = (int)worldZ / TERRAIN_SIZE;

	if (worldX < 0) {
		*gridX -= 1;
	}

	if (worldZ < 0) {
		*gridY -= 1;
	}
}

t_terrain * worldGetTerrain(t_world * world, int gridX, int gridY) {
	t_vec2i index;
	index.x = gridX;
	index.y = gridY;
	return (hmap_get(world->terrains, &index));
}

static void worldLoadNewTerrains(t_world * world, t_camera * camera) {

	int indexx = MAX(0, camera->terrain_index.x - TERRAIN_LOADED_DISTANCE);
	int indexy = MAX(0, camera->terrain_index.y - TERRAIN_LOADED_DISTANCE);
	int maxx = camera->terrain_index.x + TERRAIN_LOADED_DISTANCE;
	int maxy = camera->terrain_index.y + TERRAIN_LOADED_DISTANCE;
	int gridX, gridY;
	for (gridX = indexx ; gridX  < maxx; gridX++) {
		for (gridY = indexy ; gridY < maxy; gridY++) {

			if (gridX < 0 || gridY < 0) {
				continue ;
			}

			//if this terrain isnt generated yet
			if (worldGetTerrain(world, gridX, gridY) == NULL) {

				//can it be generated?
				float wx = gridX * TERRAIN_SIZE;
				float wz = gridY * TERRAIN_SIZE;
				t_biom * biom = worldGetBiomAt(world, wx, wz);
				if (!biom->canGenerateAt(world, biom, wx, wz)) {
					continue ;
				}

				//if so, generate it
				t_terrain * terrain = terrainNew(world, gridX, gridY);

				if (terrain != NULL) {
					hmap_insert(world->terrains, terrain, &(terrain->index));
				}
			}
		}
	}
}

void worldUpdate(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera) {
	//load new terrains
	worldLoadNewTerrains(world, camera);
	world->time++;
}