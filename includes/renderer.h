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

#endif