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
	world->noise = noise2New();

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
	noise2Delete(world->noise);
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

void worldRemoveTerrain(t_world * world, t_terrain * terrain) {
	hmap_remove_key(world->terrains, &(terrain->index));
	terrainDelete(terrain);
}

static void worldSpawnTerrain(t_world * world, t_terrain * terrain) {
	hmap_insert(world->terrains, terrain, &(terrain->index));
}

static void worldLoadNewTerrains(t_world * world, t_renderer * renderer, t_camera * camera) {
	int indexx = camera->terrain_index.x - TERRAIN_LOADED_DISTANCE;
	int indexy = camera->terrain_index.y - TERRAIN_LOADED_DISTANCE;
	int maxx = camera->terrain_index.x + TERRAIN_LOADED_DISTANCE;
	int maxy = camera->terrain_index.y + TERRAIN_LOADED_DISTANCE;
	int gridX, gridY;
	for (gridX = indexx ; gridX  < maxx; gridX++) {
		for (gridY = indexy ; gridY < maxy; gridY++) {
			if (worldGetTerrain(world, gridX, gridY) == NULL) {
				t_terrain * terrain = terrainNew(renderer, gridX, gridY);
				terrainGenerate(world, terrain);
				worldSpawnTerrain(world, terrain);
			}
		}
	}
}

static void worldUpdateLists(t_world * world, t_renderer * renderer, t_camera * camera) {

	//clear list
	array_list_clear(renderer->delete_list);
	array_list_clear(renderer->render_list);

	//update listst
	HMAP_ITER_START(world->terrains, t_terrain *, terrain) {

		t_vec2i diff;

		vec2i_sub(&diff, &(terrain->index), &(camera->terrain_index));

		int distance = vec2i_length(&diff);

		if (distance > TERRAIN_KEEP_LOADED_DISTANCE) {
			array_list_add(renderer->delete_list, &terrain);
		} else if (distance < TERRAIN_RENDER_DISTANCE) {
			array_list_add(renderer->render_list, &terrain);
		}
	}
	HMAP_ITER_END(world->terrains, t_terrain *, terrain);
}

void worldUpdate(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera) {
	(void)context;
	(void)world;
	(void)renderer;
	(void)camera;

	//load new terrains
	worldLoadNewTerrains(world, renderer, camera);

	//update terrain to render and terrain to delete
	worldUpdateLists(world, renderer, camera);

}