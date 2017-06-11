#ifndef VEC3F_H
# define VEC3F_H

# include "cmaths.h"

typedef struct	s_vec3f {
	union {
		float x;
		float r;
		float pitch;
	};

	union {
		float y;
		float g;
		float yaw;
	};

	union {
		float z;
		float b;
		float roll;
	};
}				t_vec3f;

/** create a new vec3f */
t_vec3f * vec3f_new(void);

/** delete the vec3f */
void vec3f_delete(t_vec3f * vec);

/** set the vec3f to 0 */
t_vec3f * vec3f_zero(t_vec3f * dst);

/** set the vec3f values */
t_vec3f * vec3f_set(t_vec3f * dst, float x, float y, float z);
t_vec3f * vec3f_set3(t_vec3f * dst, t_vec3f * vec);

/** add two vec3f */
t_vec3f * vec3f_add(t_vec3f * dst, t_vec3f * left, t_vec3f * right);

/** sub two vec3f */
t_vec3f * vec3f_sub(t_vec3f * dst, t_vec3f * left, t_vec3f * right);

/** mult the vec3f by the given scalar */
t_vec3f * vec3f_mult(t_vec3f * dst, t_vec3f * vec, float scalar);
t_vec3f * vec3f_mult3(t_vec3f * dst, t_vec3f * left, t_vec3f * right);

/** cross product */
t_vec3f * vec3f_cross(t_vec3f * dst, t_vec3f * left, t_vec3f * right);

/** scale product */
float vec3f_dot_product(t_vec3f * left, t_vec3f * right);

/** length */
float vec3f_length_squared(t_vec3f * vec);
float vec3f_length(t_vec3f * vec);

/** normalize */
t_vec3f * vec3f_normalize(t_vec3f * dst, t_vec3f * vec);

/** negate */
t_vec3f * vec3f_negate(t_vec3f * dst, t_vec3f * src);

/** angle between two vec */
float vec3f_angle(t_vec3f * left, t_vec3f * right);

/** mix the two vectors */
t_vec3f * vec3f_mix(t_vec3f * dst, t_vec3f * left, t_vec3f * right, float ratio);

/** comparison */
int vec3f_equals(t_vec3f * left, t_vec3f * right);

/** hash */
int vec3f_hash(t_vec3f * vec);

/** round vec3f */
t_vec3f * vec3f_round(t_vec3f * dst, t_vec3f * vec, int decimals);

/** to string: return a string allocated with malloc() */
char * vec3f_str(t_vec3f * vec);

#endif