#include "renderer.h"

static void worldSpawnTerrain(t_world * world, t_terrain * terrain) {
	hmap_insert(world->terrains, terrain, &(terrain->index));
}

void worldInit(t_world * world) {
	glhCheckError("pre worldInit()");

	//create the terrain hash map
	world->terrains = hmap_new(4096, (t_hf)terrainHash, (t_cmpf)vec2i_equals, (t_f)vec2i_delete, (t_f)terrainDelete);
	if (world->terrains == NULL) {
		fprintf(stderr, "world.c : l.10 : worldInit() : not enough memory\n");
		return ;
	}

	//noise
	world->noise = noise2New();

	//spawn terrain test
	t_terrain * terrain;

	int i, j;
	for (i = 0 ; i < 8; i++) {
		for (j = 0 ; j < 8; j++) {
			terrain = terrainNew(i, j);
			terrainGenerate(world, terrain);
			worldSpawnTerrain(world, terrain);
		}
	}


	glhCheckError("post worldInit()");
}

void worldDelete(t_world * world) {
	hmap_delete(world->terrains);
	free(world->terrains);
	noise2Delete(world->noise);
}

void worldUpdate(t_world * world, t_camera * camera) {
	(void)world;
	(void)camera;
}