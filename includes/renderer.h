#ifndef RENDERER_H
# define RENDERER_H

# include "array_list.h"
# include "hmap.h"
# include "vec.h"
# include "glh.h"
# include "noise.h"
# include <string.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <time.h>

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
# define TERRAIN_SIZE (256)
// number of terrain to render in term of distance
# define TERRAIN_RENDER_DISTANCE (10)
// distance where terrain are kept loaded in memory
# define TERRAIN_LOADED_DISTANCE (TERRAIN_RENDER_DISTANCE)
// distance where terrain are kept loaded in memory
# define TERRAIN_KEEP_LOADED_DISTANCE (TERRAIN_RENDER_DISTANCE)

/** a terrain */
typedef struct 	s_terrain {
	t_vec2i index;
	GLuint vao;
	GLuint vbo;
}				t_terrain;

/** the world */
typedef struct 	s_world {
	t_hmap *	terrains;
	t_noise2 *	noise;
}				t_world;

/** the renderer part of the program */
typedef struct 	s_renderer {
	t_glh_program	* program; //the rendering GPU program
	GLuint 			terrain_indices; //terrain indices buffer
	GLuint 			terrain_vertices; //terrain vertices buffer (static grid)
	t_array_list 	* render_list; //the list of terrain to render
	t_array_list 	* delete_list; //the list of terrain to delete
}				t_renderer;

//renderer related functions
void rendererInit(t_renderer * renderer);
void rendererDelete(t_renderer * renderer);
void rendererUpdate(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera);
void rendererRender(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera);

//world related functions
void 		worldInit(t_world * world);
void 		worldDelete(t_world * world);
void 		worldUpdate(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera);
void 		worldGetGridIndex(t_world * world, float worldX, float worldZ, int * gridX, int * gridY);
t_terrain * worldGetTerrain(t_world * world, int gridX, int gridY);
void		worldRemoveTerrain(t_world * world, t_terrain * terrain);
//terrains
t_terrain *	terrainNew(t_renderer * renderer, int gridX, int gridY);
void 		terrainDelete(t_terrain * terrain);
int 		terrainHash(t_terrain * terrain);
int 		terrainCmp(t_terrain * left, t_terrain * right);
void 		terrainGenerate(t_world * world, t_terrain * terrain);
void		terrainLoadHeightMap(t_terrain * terrains, int * n, char const * bmpfile);

//camera related functions
void cameraInit(t_camera * camera);
void cameraDelete(t_camera * camera);
void cameraUpdate(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera);

//inputs
void inputInit(t_glh_context * context);
void inputUpdate(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera);

#endif