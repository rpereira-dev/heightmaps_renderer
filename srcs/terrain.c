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

/*
float clamp(float val, float min, float max) {
	if (val > max) {
		return (max);
	}
    return (val < min ? min : val);
}
*/

static float terrainGenerateHeightAt(t_world * world, int gridX, int gridY, int x, int y) {

	static float height_ratio = TERRAIN_SIZE / 128.0f;
	static int layers = 8;
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

static void terrainGenerateVertices(t_world * world, float vertices[TERRAIN_DETAIL * TERRAIN_FLOAT_PER_VERTEX],
										int gridX, int gridY, float (*heightGen)(t_world *, int, int, int, int)) {

	//generate the model
	static float unit = 1 / (float)(TERRAIN_DETAIL - 1);

	float y00, y10, y01, y11;
	float px00, py00, pz00;
	float px10, py10, pz10;
	float px01, py01, pz01;
	float px11, py11, pz11;

	int x, z;

	int i = 0;
	for (x = 0 ; x < TERRAIN_DETAIL - 1; x++) {
		for (z = 0 ; z < TERRAIN_DETAIL - 1; z++) {

			//height generation
			y00 = heightGen(world, gridX, gridY, x + 0, z + 0);
			y10 = heightGen(world, gridX, gridY, x + 1, z + 0);
			y01 = heightGen(world, gridX, gridY, x + 0, z + 1);
			y11 = heightGen(world, gridX, gridY, x + 1, z + 1);

			//position vertices
			px00 = (float)x * unit;
			py00 = y00;
			pz00 = (float)z * unit;

			px10 = ((float)x + 1.0f) * unit;
			py10 = y10;
			pz10 = pz00;

			px01 = px10;
			py01 = y01;
			pz01 = ((float)z + 1.0f) * unit;

			px11 = px10;
			py11 = y11;
			pz11 = pz01;

			//first triangle
			vertices[i++] = px00;
			vertices[i++] = py00;
			vertices[i++] = pz00;

			vertices[i++] = px10;
			vertices[i++] = py10;
			vertices[i++] = pz10;

			vertices[i++] = px11;
			vertices[i++] = py11;
			vertices[i++] = pz11;

			//second triangle
			vertices[i++] = px00;
			vertices[i++] = py00;
			vertices[i++] = pz00;

			vertices[i++] = px11;
			vertices[i++] = py11;
			vertices[i++] = pz11;

			vertices[i++] = px01;
			vertices[i++] = py01;
			vertices[i++] = pz01;
		}
	}
}

//update the vbo
static void terrainUpdateVBO(t_terrain * terrain, float vertices[TERRAIN_VERTEX_COUNT * TERRAIN_FLOAT_PER_VERTEX]) {

	//bind it to gpu
	glhVBOBind(GL_ARRAY_BUFFER, terrain->vbo);
	glhVBOData(GL_ARRAY_BUFFER, TERRAIN_VERTEX_COUNT * TERRAIN_FLOAT_PER_VERTEX * sizeof(float), vertices, GL_STATIC_DRAW);
	//glhVBOData(GL_ARRAY_BUFFER, 18 * sizeof(float), vertices, GL_STATIC_DRAW);
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

	int gridX = terrain->index.x;
	int gridY = terrain->index.y;

	//generate model vertices
	float vertices[TERRAIN_VERTEX_COUNT * TERRAIN_FLOAT_PER_VERTEX];

	terrainGenerateVertices(world, vertices, gridX, gridY, terrainGenerateHeightAt);

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

	terrain->index.x = gridX;
	terrain->index.y = gridY;

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
