#include "renderer.h"

static void terrainCalculateNormal(t_world * world, t_biom * biom, t_vec3f * normal, float wx, float wy) {

	float left	= biom->heightGen(world, biom, wx - TERRAIN_UNIT, wy);
	float right	= biom->heightGen(world, biom, wx + TERRAIN_UNIT, wy);
	float down	= biom->heightGen(world, biom, wx, wy - TERRAIN_UNIT);
	float up	= biom->heightGen(world, biom, wx, wy + TERRAIN_UNIT);

	vec3f_set(normal, left - right, 2.0f, down - up);
	vec3f_normalize(normal, normal);
}

static void terrainGenerateVertices(t_world * world, float vertices[], int gridX, int gridY) {
	t_vec3f normal;
	int x, y;
	int i = 0;
	for (x = 0 ; x < TERRAIN_DETAIL ; x++) {
		for (y = 0 ; y < TERRAIN_DETAIL; y++) {

			float wx = (gridX * (TERRAIN_DETAIL - 1) + x) * TERRAIN_UNIT;
			float wz = (gridY * (TERRAIN_DETAIL - 1) + y) * TERRAIN_UNIT;
			t_biom * biom = worldGetBiomAt(world, wx, wz);
			float wy = biom->heightGen(world, biom, wx, wz);
			int textureID = biom->colorGen(world, biom, wx, wy, wz);
			terrainCalculateNormal(world, biom, &normal, wx, wz);

			*(vertices + i++) = wy;

			//generate normal
			*(vertices + i++) = normal.x;
			*(vertices + i++) = normal.y;
			*(vertices + i++) = normal.z;

			//generate texture id
			*((int*)(vertices + i++)) = textureID;
		}
	}
}

/** generate n terrain from the given bmp map file, and return them */
void terrainLoadvertices(t_terrain * terrains, int * n, char const * bmpfile) {
	(void)terrains;
	(void)n;
	(void)bmpfile;

	//TODO
}

void terrainGenerate(t_world * world, t_terrain * terrain) {
	int gridX = terrain->index.x;
	int gridY = terrain->index.y;
	terrain->vertices = (float *) malloc(TERRAIN_DETAIL * TERRAIN_DETAIL * TERRAIN_VERTEX_SIZE);
	terrainGenerateVertices(world, terrain->vertices, gridX, gridY);
}

/** allocate a new terrain on heap + gpu */
t_terrain * terrainNew(t_world * world, int gridX, int gridY) {
	//allocate the terrain
	t_terrain * terrain = (t_terrain*)malloc(sizeof(t_terrain));
	if (terrain == NULL) {
		return (NULL);
	}

	terrain->index.x = gridX;
	terrain->index.y = gridY;
	terrain->vertices = NULL;
	terrain->initialized = 0;

	terrainGenerate(world, terrain);

	return (terrain);
}
