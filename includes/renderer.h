#ifndef RENDERER_H
# define RENDERER_H

# include "array_list.h"
# include "hmap.h"
# include "vec2.h"
# include "vec3.h"
# include "vec4.h"
# include "mat4.h"
# include "glh.h"
# include <string.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <time.h>

/** the camera data structures */
typedef struct	s_camera {
	t_vec3	pos;
	float	fov;
	float 	near_distance;
	float	far_distance;
	t_mat4	mview;
	t_mat4	mproj;
	t_mat4	mviewproj;
	float	render_distance;
}				t_camera;

//terrain detail (number of vertex per line)
# define TERRAIN_DETAIL (16)
// number of vertex per terrain
# define TERRAIN_VERTEX_COUNT (TERRAIN_DETAIL * TERRAIN_DETAIL * 6)
//float per vertices, i.e, (x, y, z)
# define TERRAIN_FLOAT_PER_VERTEX (3)
//terrain width (and height)
# define TERRAIN_SIZE (256)

/** a terrain */
typedef struct 	s_terrain {
	int gridX, gridY;
	GLuint vao;
	GLuint vbo;
}				t_terrain;

/** the world */
typedef struct 	s_world {
	t_hmap	* terrains;
}				t_world;

/** the renderer part of the program */
typedef struct 	s_renderer {
	t_glh_program * program;
}				t_renderer;

//renderer related functions
void rendererInit(t_renderer * renderer);
void rendererDelete(t_renderer * renderer);
void rendererUpdate(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera);
void rendererRender(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera);

//world related functions
void worldInit(t_world * world);
void worldDelete(t_world * world);
void worldUpdate(t_world * world, t_camera * camera);

//terrains
t_terrain *	terrainNew(int gridX, int gridY);
void 		terrainDelete(t_terrain * terrain);
void 		terrainGenerate(t_terrain * terrain);
void		terrainLoadHeightMap(t_terrain * terrains, int * n, char const * bmpfile);

//camera related functions
void cameraInit(t_camera * camera);
void cameraDelete(t_camera * camera);
void cameraUpdate(t_camera * camera);

//inputs
void inputInit(t_glh_context * context);

#endif