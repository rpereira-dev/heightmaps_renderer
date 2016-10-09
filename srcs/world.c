#include "renderer.h"

static void worldSpawnTerrain(t_world * world, t_terrain * terrain) {
	hmap_insert(world->terrains, terrain, &(terrain->index));
}

static void worldTerrainGenerateDefaultVertices(t_world * world) {
	float unit = 1 / (float)(TERRAIN_DETAIL - 1);
	int x, z;
	int i = 0;
	for (x = 0 ; x < TERRAIN_DETAIL - 1; x++) {
		for (z = 0 ; z < TERRAIN_DETAIL - 1; z++) {
			world->terrain_default_vertices[i++] = x * unit;
			world->terrain_default_vertices[i++] = 0;
			world->terrain_default_vertices[i++] = z * unit;
		}
	}
}

void worldInit(t_world * world) {
	glhCheckError("pre worldInit()");

	//create the terrain hash map
	world->terrains = hmap_new(4096, (t_hf)terrainHash, (t_cmpf)vec2i_equals, (t_f)vec2i_delete, (t_f)terrainDelete);
	if (world->terrains == NULL) {
		fprintf(stderr, "world.c : l.10 : worldInit() : not enough memory\n");
		return ;
	}

	//generate default terrain vertices which will be copy on each terrain generation
	worldTerrainGenerateDefaultVertices(world);

	//noise creation
	world->noise = noise2New();

	//TESTS
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