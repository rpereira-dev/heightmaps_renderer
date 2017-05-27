#include "renderer.h"

static void terrainCalculateNormal(t_world * world, t_biom * biom, float * nx, float * nz, float wx, float wz) {

	float dx = biom->heightGenStep;
	float dz = biom->heightGenStep;

	float left	= biom->heightGen(world, biom, wx - dx, wz);
	float right	= biom->heightGen(world, biom, wx + dx, wz);
	float down	= biom->heightGen(world, biom, wx, wz - dz);
	float up	= biom->heightGen(world, biom, wx, wz + dz);

	*nx = (right - left) / (2.0f * dx);
	//*ny = 1.0f;
	*nz = (up - down) / (2.0f * dz);
}

static void terrainGenerateVertices(t_world * world, float vertices[], int gridX, int gridY) {
	int x, y;
	float nx, nz;
	int i = 0;

	for (x = 0 ; x < TERRAIN_DETAIL ; x++) {
		for (y = 0 ; y < TERRAIN_DETAIL; y++) {

			float wx = (gridX * (TERRAIN_DETAIL - 1) + x) * TERRAIN_UNIT;
			float wz = (gridY * (TERRAIN_DETAIL - 1) + y) * TERRAIN_UNIT;
			t_biom * biom = worldGetBiomAt(world, wx, wz);
			float wy = biom->heightGen(world, biom, wx, wz);
			int textureID = biom->colorGen(world, biom, wx, wy, wz);

			terrainCalculateNormal(world, biom, &nx, &nz, wx, wz);

			*(vertices + i++) = wy;

			//generate normal
			*(vertices + i++) = nx;
			*(vertices + i++) = nz;

			//generate texture id
			*((int*)(vertices + i++)) = textureID;
		}
	}
}

void terrainGenerate(t_world * world, t_terrain * terrain) {
	int gridX = terrain->index.x;
	int gridY = terrain->index.y;
	terrain->vertices = (float *) malloc(TERRAIN_DETAIL * TERRAIN_DETAIL * TERRAIN_VERTEX_SIZE);
	terrainGenerateVertices(world, terrain->vertices, gridX, gridY);
}

/** delete the given terrain */
void terrainDelete(t_terrain * terrain) {
	if (terrain->initialized) {
		terrain->initialized = 0;
		glhVAODelete(terrain->vao);
		glhVBODelete(terrain->vbo);
	}
	
	if (terrain->vertices != NULL) {
		free(terrain->vertices);
		terrain->vertices = NULL;
	}
	free(terrain);
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
