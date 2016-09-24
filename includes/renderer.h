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

// number of vertex per terrain
# define TERRAIN_VERTEX_COUNT (16)
//terrain width (and height)
# define TERRAIN_SIZE (256)

/** a terrain */
typedef struct 	s_terrain {
	int x, y, z;
	float heights[TERRAIN_VERTEX_COUNT * TERRAIN_VERTEX_COUNT];
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
void rendererUpdate(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera);

//world related functions
void worldInit(t_world * world);
void worldUpdate(t_world * world, t_camera * camera);

//camera related functions
void cameraInit(t_camera * camera);
void cameraUpdate(t_camera * camera);

#endif