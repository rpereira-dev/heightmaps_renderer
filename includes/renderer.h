#ifndef RENDERER_H
# define RENDERER_H

# include "array_list.h"
# include "hmap.h"
# include "cmaths.h"
# include "vec.h"
# include "mat.h"
# include "glh.h"
# include "tinycthread.h"
# include "noise.h"
# include <string.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <time.h>
# include <fcntl.h>

/** the camera data structures */
typedef struct	s_camera {
	t_vec3f	pos; //camera world position
	t_vec3f	rot; //camera rotation toward (x, y, z) axis
	t_vec3f vview; //view vector (direction where we are currently lookin at)
	float	fov; //field of view
	float 	near_distance; //near plane distance
	float	far_distance; //far plane distance
	t_mat4f	mview; //view matrix
	t_mat4f	mproj; //projection matrix
	t_mat4f	mviewproj; //projection matrix times view matrix
	t_vec2i terrain_index; //current world terrain index of the camera
}				t_camera;

//terrain detail (number of vertex per line)
# define TERRAIN_DETAIL (16)
//terrain width (and height)
# define TERRAIN_SIZE (16)
# define TERRAIN_UNIT (TERRAIN_SIZE / TERRAIN_DETAIL)
// number of terrain to render in term of distance
# define TERRAIN_RENDER_DISTANCE (32)
// distance where terrain are kept loaded in memory
# define TERRAIN_LOADED_DISTANCE (TERRAIN_RENDER_DISTANCE + 4)
// distance where terrain are kept loaded in memory
# define TERRAIN_KEEP_LOADED_DISTANCE (TERRAIN_LOADED_DISTANCE)
# define MAX_NUMBER_OF_TERRAIN_LOADED (TERRAIN_KEEP_LOADED_DISTANCE * TERRAIN_KEEP_LOADED_DISTANCE * 2 * 2)
// number of floats per vertex
# define TERRAIN_FLOATS_PER_VERTEX (3 + 3 + 1)

# define STATE_APPLY_FOG (1)
# define STATE_APPLY_PHONG_LIGHTNING (2)

typedef struct 	s_heightmap {
	int w, h;
}				t_heightmap;

/** a terrain */
typedef struct 	s_terrain {
	t_vec2i	index;
	GLuint 	vao;
	GLuint 	vbo;
	float 	* vertices;
	int 	initialized;
}				t_terrain;

# define WORLD_OCTAVES (4)

/** the world */
typedef struct 	s_world {
	t_hmap 			* terrains;
	t_noise			* octaves[WORLD_OCTAVES];
	t_array_list	* bioms;
	int 			max_height;
	t_heightmap		* heightmap;
	int 			time;
}				t_world;

typedef struct 	s_biom {
	float (*heightGen)(t_world *, struct s_biom *, float, float);
	void (*colorGen)(t_world *, struct s_biom *, t_vec3f *, float, float, float);
	int (*canGenerateAt)(t_world *, struct s_biom *, float, float);
}				t_biom;

/** the renderer part of the program */
typedef struct 	s_renderer {
	t_glh_program	* program; //the rendering GPU program
	GLuint 			terrain_indices; //terrain indices buffer
	GLuint 			terrain_vertices; //terrain vertices buffer (static grid)
	t_array_list 	* render_list; //the list of terrain to render
	t_array_list 	* delete_list; //the list of terrain to delete
	int 			state; //the state for rendering
}				t_renderer;

typedef struct 	s_env {
	t_glh_context 	* context;
	t_world			world;
	t_renderer		renderer;
	t_camera		camera;
	thrd_t 			thrd;
	int 			is_running;
}				t_env;

//renderer related functions
void rendererInit(t_renderer * renderer);
void rendererDelete(t_renderer * renderer);
void rendererUpdate(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera);
void rendererRender(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera);

//world related functions
void 		worldInit(t_world * world, char * bmpfile, int maxheight);
void 		worldDelete(t_world * world);
void 		worldUpdate(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera);
void 		worldGetGridIndex(t_world * world, float worldX, float worldZ, int * gridX, int * gridY);
t_terrain * worldGetTerrain(t_world * world, int gridX, int gridY);
t_biom *	worldGetBiomAt(t_world * world, float wx, float wz);

/** bioms */
void	biomsInit(t_world * world);
void 	biomsDelete(t_world * world);

//terrains
t_terrain *	terrainNew(t_world * world, int gridX, int gridY);
int 		terrainHash(t_terrain * terrain);
int 		terrainCmp(t_terrain * left, t_terrain * right);
void		terrainLoadHeightMap(t_terrain * terrains, int * n, char const * bmpfile);
void 		terrainGenerate(t_world * world, t_terrain * terrain);

//camera related functions
void cameraInit(t_camera * camera);
void cameraDelete(t_camera * camera);
void cameraUpdate(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera);

//heightmaps (bmp file)
t_heightmap *	heightmapNew(char const * path);
void 			heightmapDelete(t_heightmap * map);
int 			heightmapGetHeight(t_heightmap * map, int x, int y);

//inputs
void inputInit(t_glh_context * context);
void inputUpdate(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera);

#endif