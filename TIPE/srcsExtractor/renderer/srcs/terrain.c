#include "renderer.h"

static void terrainCalculateNormal(t_world * world, t_generator * generator, float * nx, float * nz,
									float wx, float wy, float wz) {
	float dx = generator->heightGenStep;
	float dz = generator->heightGenStep;
	*nx = (generator->heightGen(world, generator, wx + dx, wz) - wy) / dx;
	*nz = (generator->heightGen(world, generator, wx, wz + dz) - wy) / dz;
}

static void terrainGenerateVertices(t_world * world, float vertices[], int gridX, int gridY) {
	float nx, nz;
	int i = 0;
	for (int x = 0 ; x < TERRAIN_DETAIL ; x++) {
		for (int y = 0 ; y < TERRAIN_DETAIL; y++) {

			float wx = (gridX * (TERRAIN_DETAIL - 1) + x) * TERRAIN_UNIT;
			float wz = (gridY * (TERRAIN_DETAIL - 1) + y) * TERRAIN_UNIT;
			t_generator * generator = worldGetGenerator(world, wx, wz);
			float wy = generator->heightGen(world, generator, wx, wz);
			int textureID = generator->colorGen(world, generator, wx, wy, wz);

			terrainCalculateNormal(world, generator, &nx, &nz, wx, wy, wz);

			*(vertices + i++) = wy;
			*(vertices + i++) = nx;
			*(vertices + i++) = nz;
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

	//generate the transformation matrix for this terrain
	mat4f_identity(&(terrain->mat));
	mat4f_translate(&(terrain->mat), &(terrain->mat), terrain->index.x * TERRAIN_SIZE, 0, terrain->index.y * TERRAIN_SIZE);
	mat4f_scale(&(terrain->mat), &(terrain->mat), TERRAIN_SIZE);

	terrainGenerate(world, terrain);

	return (terrain);
}
