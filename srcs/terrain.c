#include "renderer.h"

/*
float clamp(float val, float min, float max) {
	if (val > max) {
		return (max);
	}
    return (val < min ? min : val);
}
*/

static float terrainGetHeight(float vertices[], int x, int y) {
	return (vertices[(x * TERRAIN_DETAIL + y) * 4]);
}

static void terrainCalculateNormal(float vertices[], t_vec3f * normal, int x, int z) {

	float xy_height = terrainGetHeight(vertices, x, z);
	float left	= (x - 1 >= 0) 				? terrainGetHeight(vertices, x - 1, z) : xy_height + xy_height - terrainGetHeight(vertices, x + 1, z);
	float right	= (x + 1 < TERRAIN_DETAIL) 	? terrainGetHeight(vertices, x + 1, z) : xy_height + xy_height - terrainGetHeight(vertices, x - 1, z);
	float down	= (z - 1 >= 0) 				? terrainGetHeight(vertices, x, z - 1) : xy_height + xy_height - terrainGetHeight(vertices, x, z + 1);
	float up 	= (z + 1 < TERRAIN_DETAIL) 	? terrainGetHeight(vertices, x, z + 1) : xy_height + xy_height - terrainGetHeight(vertices, x, z - 1);
	vec3f_set(normal, left - right, 2.0f, down - up);
	vec3f_normalize(normal, normal);
}

static void terrainGenerateNormals(float vertices[]) {
	t_vec3f normal;

	int x, z;
	int i = 0;
	for (x = 0 ; x < TERRAIN_DETAIL ; x++) {
		for (z = 0 ; z < TERRAIN_DETAIL; z++) {

			//calculate the normal
			terrainCalculateNormal(vertices, &normal, x, z);
			++i; //skip height
			vertices[i++] = normal.x;
			vertices[i++] = normal.y;
			vertices[i++] = normal.z;
		}
	}
}

static float terrainGenerateHeightAt(t_world * world, int gridX, int gridY, int x, int y) {

/*
	float posx = gridX * (TERRAIN_DETAIL - 1) + x;
	float posy = gridY * (TERRAIN_DETAIL - 1) + y;
	return (noise2(world->noise, posx * 0.009f, posy * 0.009f) * 3.37f);
*/


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
			vertices[i++] = heightGen(world, gridX, gridY, x, z);
			++i; //skip normalx
			++i; //skip normaly
			++i; //skip normalz
		}
	}
}

//update the vbo
static void terrainUpdateVBO(t_terrain * terrain, float vertices[]) {

	//bind it to gpu
	glhVBOBind(GL_ARRAY_BUFFER, terrain->vbo);
	glhVBOData(GL_ARRAY_BUFFER, TERRAIN_DETAIL * TERRAIN_DETAIL * 4 * sizeof(float), vertices, GL_STATIC_DRAW);
	glhVBOUnbind(GL_ARRAY_BUFFER);
}

/** generate n terrain from the given bmp map file, and return them */
void terrainLoadvertices(t_terrain * terrains, int * n, char const * bmpfile) {
	(void)terrains;
	(void)n;
	(void)bmpfile;

	//TODO
}

void terrainGenerate(t_world * world, t_terrain * terrain) {

	//the world's terrain index
	int gridX = terrain->index.x;
	int gridY = terrain->index.y;

	float vertices[TERRAIN_DETAIL * TERRAIN_DETAIL * 4];

	//generate vertices
	terrainGenerateVertices(world, vertices, gridX, gridY, terrainGenerateHeightAt);
	terrainGenerateNormals(vertices);
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
	glhVAOSetAttribute(0, 2, GL_FLOAT, 0, 2 * sizeof(float), NULL); //default vertices
	glhVBOUnbind(GL_ARRAY_BUFFER);
	glhVAOEnableAttribute(0);

	//bind vertices
	glhVBOBind(GL_ARRAY_BUFFER, terrain->vbo);
	glhVAOSetAttribute(1, 4, GL_FLOAT, 0, 4 * sizeof(float), NULL); //height + normal
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
