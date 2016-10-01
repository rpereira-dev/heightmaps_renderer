#include "renderer.h"

static int worldVec3Cmp(int * a, int * b) {
	return (a[0] == b[0] && a[1] == b[1] && a[2] == b[2]);
}

void worldInit(t_world * world) {
	world->terrains = (t_hmap*)malloc(sizeof(t_hmap));
	if (world->terrains == NULL) {
		fprintf(stderr, "world.c : l.10 : worldInit() : not enough memory\n");
		return ;
	}
	
	t_hmap terrains = hmap_new(4096, (t_hf)terrainHash, (t_cmpf)worldVec3Cmp, NULL, (t_f)terrainDelete);
	memcpy(world->terrains, &terrains, sizeof(t_hmap));
}

void worldDelete(t_world * world) {
	free(world->terrains);
}

void worldUpdate(t_world * world, t_camera * camera) {
	(void)world;
	(void)camera;
}