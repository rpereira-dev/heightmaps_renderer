#include "renderer.h"

int terrainHash(t_terrain * terrain) {
	(void)terrain;
	return (0);
}

int terrainCmp(t_terrain * left, t_terrain * right) {
	(void)left;
	(void)right;
	return (1);
}

static int terrainGenerateHeightAt(int gridX, int gridY, int x, int y) {
	//TODO PERLIN NOISE
	(void)gridX;
	(void)gridY;
	(void)x;
	(void)y;
	return (0);
}

static void terrainGenerateVertices(float vertices[TERRAIN_DETAIL * TERRAIN_FLOAT_PER_VERTEX], int gridX, int gridY, int (*heightGen)(int, int, int, int)) {
		//generate the model
	int x, z;

	int i = 0;
	for (x = 0 ; x < TERRAIN_DETAIL - 1; x++) {
		for (z = 0 ; z < TERRAIN_DETAIL - 1; z++) {

			//height generation
			int y00, y10, y01, y11;

			y00 = heightGen(gridX, gridY, x + 0, z + 0);
			y10 = heightGen(gridX, gridY, x + 1, z + 0);
			y01 = heightGen(gridX, gridY, x + 0, z + 1);
			y11 = heightGen(gridX, gridY, x + 1, z + 1);

			//first triangle
			vertices[i++] = x;
			vertices[i++] = y00;
			vertices[i++] = z;

			vertices[i++] = x + 1;
			vertices[i++] = y10;
			vertices[i++] = z;

			vertices[i++] = x;
			vertices[i++] = y01;
			vertices[i++] = z + 1;

			//second triangle
			vertices[i++] = x + 1;
			vertices[i++] = y10;
			vertices[i++] = z;

			vertices[i++] = x + 1;
			vertices[i++] = y11;
			vertices[i++] = z + 1;

			vertices[i++] = x;
			vertices[i++] = y01;
			vertices[i++] = z + 1;
		}
	}
}

//update the vbo
static void terrainUpdateVBO(t_terrain * terrain, float vertices[TERRAIN_VERTEX_COUNT * TERRAIN_FLOAT_PER_VERTEX]) {

	//bind it to gpu
	glhVBOBind(GL_ARRAY_BUFFER, terrain->vbo);
	glhVBOData(GL_ARRAY_BUFFER, TERRAIN_VERTEX_COUNT * TERRAIN_FLOAT_PER_VERTEX, vertices, GL_STATIC_DRAW);
	glhVBOUnbind(GL_ARRAY_BUFFER);
}

/** generate n terrain from the given bmp map file, and return them */
void terrainLoadHeightMap(t_terrain * terrains, int * n, char const * bmpfile) {
	(void)terrains;
	(void)n;
	(void)bmpfile;

	//TODO
}

void terrainGenerate(t_terrain * terrain) {

	int gridX = terrain->index.x;
	int gridY = terrain->index.y;

	//generate model vertices
	float vertices[TERRAIN_VERTEX_COUNT * TERRAIN_FLOAT_PER_VERTEX];

	terrainGenerateVertices(vertices, gridX, gridY, terrainGenerateHeightAt);

	//update the vbo
	terrainUpdateVBO(terrain, vertices);
}


/** allocate a new terrain on heap + gpu */
t_terrain * terrainNew(int gridX, int gridY) {

	//allocate the terrain
	t_terrain * terrain = (t_terrain*)malloc(sizeof(t_terrain));
	if (terrain == NULL) {
		return (NULL);
	}

	vec2i_set(&(terrain->index), gridX, gridY);

	//allocate terrain model on GPU
	terrain->vao = glhVAOGen();
	terrain->vbo = glhVBOGen();

	//bind it to gpu
	glhVAOBind(terrain->vao);
	glhVBOBind(GL_ARRAY_BUFFER, terrain->vbo);
	glhVAOSetAttribute(0, 3, GL_FLOAT, 0, 3 * sizeof(float), NULL);
	glhVAOEnableAttribute(0);
	glhVBOUnbind(GL_ARRAY_BUFFER);
	glhVAOUnbind();

	return (terrain);
}

/** delete a terrain from memory (heap + gpu)*/
void terrainDelete(t_terrain * terrain) {
	glhVAODelete(terrain->vao);
	glhVBODelete(terrain->vbo);
	free(terrain);
}
