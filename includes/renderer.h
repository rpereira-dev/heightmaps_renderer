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

typedef struct	s_camera {
	t_vec3	pos;
	float	fov;
	float 	near_distance;
	float	far_distance;
	t_mat4	mview;
	t_mat4	mproj;
	t_mat4	mviewproj;
}				t_camera;

typedef struct 	s_terrain {

}				t_terrain;

typedef struct 	s_world {
	t_hmap	* terrains;
}				t_world;

/** the renderer part of the program */
typedef struct 	s_renderer {
	t_glh_program * program;
}				t_renderer;

void rendererUpdate(t_world * world, t_renderer * renderer, t_camera * camera);

void worldUpdate(t_world * world, t_camera * camera);

void cameraUpdate(t_camera * camera);

#endif