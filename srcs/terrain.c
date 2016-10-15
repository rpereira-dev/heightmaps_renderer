#include "renderer.h"

/*
float clamp(float val, float min, float max) {
	if (val > max) {
		return (max);
	}
    return (val < min ? min : val);
}
*/

static float terrainGenerateHeightAt(t_world * world, int gridX, int gridY, int x, int y) {

	static float height_ratio = 1.0f;
	static int layers = 1;
	static int layerf = 3.5f;
	static int weightf = 0.9f;

	float layer = 0.009f;
	float weight = 1.0f;
	float height = 0;
	int i;
	for (i = 0 ; i < layers ; i++) {
		float posx = gridX * (TERRAIN_DETAIL - 1) + x;
		float posy = gridY * (TERRAIN_DETAIL - 1) + y;
		height += noise2(world->noise, posx * layer, posy * layer) * weight;
	    layer *= layerf;
	    weight *= weightf;
	}
	return (height * height_ratio);
}


static void terrainGenerateVertices(t_world * world, float vertices[], int gridX, int gridY,
										float (*heightGen)(t_world *, int, int, int, int)) {
	int x, z;
	int i = 0;
	for (x = 0 ; x < TERRAIN_DETAIL ; x++) {
		for (z = 0 ; z < TERRAIN_DETAIL; z++) {
			vertices[i++] =  heightGen(world, gridX, gridY, x, z);
		}
	}
}

//update the vbo
static void terrainUpdateVBO(t_terrain * terrain, float vertices[]) {

	//bind it to gpu
	glhVBOBind(GL_ARRAY_BUFFER, terrain->vbo);
	glhVBOData(GL_ARRAY_BUFFER, TERRAIN_DETAIL * TERRAIN_DETAIL * sizeof(float), vertices, GL_STATIC_DRAW);
	glhVBOUnbind(GL_ARRAY_BUFFER);
}

/** generate n terrain from the given bmp map file, and return them */
void terrainLoadHeightMap(t_terrain * terrains, int * n, char const * bmpfile) {
	(void)terrains;
	(void)n;
	(void)bmpfile;

	//TODO
}

void terrainGenerate(t_world * world, t_terrain * terrain) {

	//the world's terrain index
	int gridX = terrain->index.x;
	int gridY = terrain->index.y;

	float vertices[TERRAIN_DETAIL * TERRAIN_DETAIL];

	//generate heightmap
	terrainGenerateVertices(world, vertices, gridX, gridY, terrainGenerateHeightAt);

	//update the vbo
	terrainUpdateVBO(terrain, vertices);
}


/** allocate a new terrain on heap + gpu */
t_terrain * terrainNew(t_renderer * renderer, int gridX, int gridY) {

	//allocate the terrain
	t_terrain * terrain = (t_terrain*)malloc(sizeof(t_terrain));
	if (terrain == NULL) {
		return (NULL);
	}

	terrain->index.x = gridX;
	terrain->index.y = gridY;

	//allocate terrain model on GPU
	terrain->vao = glhVAOGen();
	terrain->vbo = glhVBOGen();

	//bind vao
	glhVAOBind(terrain->vao);

	//bind indices
	glhVBOBind(GL_ELEMENT_ARRAY_BUFFER, renderer->terrain_indices);

	//bind static grid
	glhVBOBind(GL_ARRAY_BUFFER, renderer->terrain_vertices);
	glhVAOSetAttribute(0, 2, GL_FLOAT, 0, 2 * sizeof(float), NULL);
	glhVBOUnbind(GL_ARRAY_BUFFER);
	glhVAOEnableAttribute(0);

	//bind heightmap
	glhVBOBind(GL_ARRAY_BUFFER, terrain->vbo);
	glhVAOSetAttribute(1, 1, GL_FLOAT, 0, 1 * sizeof(float), NULL);
	glhVBOUnbind(GL_ARRAY_BUFFER);
	glhVAOEnableAttribute(1);

	//unbind vao
	glhVAOUnbind();

	return (terrain);
}

/** delete a terrain from memory (heap + gpu)*/
void terrainDelete(t_terrain * terrain) {
	glhVAODelete(terrain->vao);
	glhVBODelete(terrain->vbo);
	free(terrain);
}
