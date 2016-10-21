#include "renderer.h"

static float terrainGetHeight(float vertices[], int x, int y) {
	return (vertices[(x * TERRAIN_DETAIL + y) * TERRAIN_FLOATS_PER_VERTEX]);
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

	int x, y;
	int i = 0;
	for (x = 0 ; x < TERRAIN_DETAIL ; x++) {
		for (y = 0 ; y < TERRAIN_DETAIL; y++) {

			//calculate the normal
			terrainCalculateNormal(vertices, &normal, x, y);
			++i; //skip height
			vertices[i++] = normal.x;
			vertices[i++] = normal.y;
			vertices[i++] = normal.z;
			++i; //skip color r
			++i; //skip color g
			++i; //skip color b
		}
	}
}

float clamp(float val, float min, float max) {
	if (val > max) {
		return (max);
	}
    return (val < min ? min : val);
}

static void terrainGenerateColorAt(t_world * world, t_vec3f * color, float wx, float wy, float wz) {

	static t_vec3f blue		= {{0.1f}, {0.2f}, {0.9f}};
	static t_vec3f green 	= {{0.2f}, {0.9f}, {0.14f}};
	static t_vec3f brown	= {{0.4f}, {0.35f}, {0.25f}};
	static t_vec3f gray		= {{0.6f}, {0.6f}, {0.6f}};
	static t_vec3f white	= {{0.95f}, {0.95f}, {0.95f}};

	float gradient = noise3(world->octaves[1], wx * 16, wy * 16, wz * 16) * 0.05f;

	if (wy <= -10) {
		vec3f_set3(color, &blue);
	} else if (wy <= -6.0f) {
		vec3f_mix(color, &blue, &green, 1 - (wy + 10.0f) / 4.0f);
	} else if (wy <= 3) {
		vec3f_set3(color, &green);
	} else if (wy <= 7) {
		vec3f_mix(color, &green, &brown, 1 - (wy - 3.0f) / 4.0f);
	} else if (wy <= 9) {
		vec3f_set3(color, &brown);
	} else if (wy <= 11) {
		vec3f_mix(color, &brown, &gray, 1 - (wy - 9.0f) / 2.0f);
	} else if (wy <= 13) {
		vec3f_set3(color, &gray);
	} else if(wy <= 15) {
		vec3f_mix(color, &gray, &white, 1 - (wy - 13.0f) / 2.0f);
	} else {
		vec3f_set3(color, &white);
	}

	vec3f_mult(color, color, 1 - gradient);
}

static float terrainGenerateHeightAt(t_world * world, float wx, float wz) {
	float height = 0;
	height += noise2(world->octaves[0], wx * 0.001f, wz * 0.001f) * 20.0f;
	height += noise2(world->octaves[1], wx * 0.01f, wz * 0.01f) * 1.0f;
	height += noise2(world->octaves[2], wx * 0.05f, wz * 0.05f) * 0.08f;
	height += noise2(world->octaves[3], wx * 16.0f, wz * 16.0f) * 0.01f;
	height = clamp(height, -10, 512);
	return (height);
}

static void terrainGenerateVertices(t_world * world, float vertices[], int gridX, int gridY,
										float (*heightGen)(t_world *, float, float),
										void (*colorGen)(t_world *, t_vec3f *, float, float, float)) {
	t_vec3f color;
	int x, y;
	int i = 0;
	for (x = 0 ; x < TERRAIN_DETAIL ; x++) {
		for (y = 0 ; y < TERRAIN_DETAIL; y++) {

			float wx = gridX * (TERRAIN_DETAIL - 1) + x;
			float wz = gridY * (TERRAIN_DETAIL - 1) + y;
			float wy = heightGen(world, wx, wz);
			colorGen(world, &color, wx, wy, wz);

			vertices[i++] = wy;

			++i; //skip normalx
			++i; //skip normaly
			++i; //skip normalz

			//generate color
			vertices[i++] = color.r;
			vertices[i++] = color.g;
			vertices[i++] = color.b;
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
	terrain->vertices = (float *) malloc(sizeof(float) * TERRAIN_DETAIL * TERRAIN_DETAIL * TERRAIN_FLOATS_PER_VERTEX);
	terrainGenerateVertices(world, terrain->vertices, gridX, gridY, terrainGenerateHeightAt, terrainGenerateColorAt);
	terrainGenerateNormals(terrain->vertices);
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

/** delete a terrain from memory (heap + gpu)*/
void terrainDelete(t_terrain * terrain) {
	free(terrain);
}
