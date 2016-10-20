#include "renderer.h"

void worldInit(t_world * world) {
	glhCheckError("pre worldInit()");

	//create the terrain hash map
	world->terrains = hmap_new(4096, (t_hf)vec2i_hash, (t_cmpf)vec2i_nequals, (t_f)NULL, (t_f)NULL);
	if (world->terrains == NULL) {
		fprintf(stderr, "world.c : l.10 : worldInit() : not enough memory\n");
		return ;
	}

	//noise creation
	long long unsigned int seed = time(NULL);
	int i;
	for (i = 0 ; i < WORLD_OCTAVES ; i++) {
		world->octaves[i] = noiseNew();
		noiseNextInt(&seed);
		noiseSeed(world->octaves[i], seed);
	}

/*
	terrain = terrainNew(0, 0);
	terrainGenerate(world, terrain);
	worldSpawnTerrain(world, terrain);
*/

	glhCheckError("post worldInit()");
}

void worldDelete(t_world * world) {
	hmap_delete(world->terrains);
	free(world->terrains);

	int i;
	for (i = 0 ; i < WORLD_OCTAVES ; i++) {
		noiseDelete(world->octaves[i]);
	}
}

void worldGetGridIndex(t_world * world, float worldX, float worldZ, int * gridX, int * gridY) {
	(void)world;

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
	int indexx = camera->terrain_index.x - TERRAIN_LOADED_DISTANCE;
	int indexy = camera->terrain_index.y - TERRAIN_LOADED_DISTANCE;
	int maxx = camera->terrain_index.x + TERRAIN_LOADED_DISTANCE;
	int maxy = camera->terrain_index.y + TERRAIN_LOADED_DISTANCE;
	int gridX, gridY;
	for (gridX = indexx ; gridX  < maxx; gridX++) {
		for (gridY = indexy ; gridY < maxy; gridY++) {
			if (worldGetTerrain(world, gridX, gridY) == NULL) {
//				printf("%ld vs %d\n", world->terrains->size, TERRAIN_KEEP_LOADED_DISTANCE * TERRAIN_KEEP_LOADED_DISTANCE * 2 * 2);
				t_terrain * terrain = terrainNew(world, gridX, gridY);
				hmap_insert(world->terrains, terrain, &(terrain->index));
			}
		}
	}
}

void worldUpdate(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera) {
	(void)context;
	(void)world;
	(void)renderer;
	(void)camera;

	//load new terrains
	worldLoadNewTerrains(world, camera);
}