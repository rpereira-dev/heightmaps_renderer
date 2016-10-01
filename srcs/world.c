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

	//spawn terrain test
	t_terrain * terrain = terrainNew(0, 0);
	terrainGenerate(terrain);
	worldSpawnTerrain(world, terrain);

	glhCheckError("post worldInit()");
}

void worldDelete(t_world * world) {
	hmap_delete(world->terrains);
	free(world->terrains);
}

void worldUpdate(t_world * world, t_camera * camera) {
	(void)world;
	(void)camera;
}