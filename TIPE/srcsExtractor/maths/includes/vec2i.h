#ifndef VEC2I_H
# define VEC2I_H

# include "cmaths.h"

typedef struct	s_vec2i {
	union {
		int x;
		int uvx;
	};

	union {
		int y;
		int uvy;
	};
}				t_vec2i;

/** create a new vec2i */
t_vec2i * vec2i_new(void);

/** delete the vec2i */
void vec2i_delete(t_vec2i * vec);

/** set the vec2i to 0 */
t_vec2i * vec2i_zero(t_vec2i * dst);

/** set the vec2i values */
t_vec2i * vec2i_set(t_vec2i * dst, int x, int y);
t_vec2i * vec2i_set2(t_vec2i * dst, t_vec2i * vec);

/** add two vec2i */
t_vec2i * vec2i_add(t_vec2i * dst, t_vec2i * left, t_vec2i * right);

/** sub two vec2i */
t_vec2i * vec2i_sub(t_vec2i * dst, t_vec2i * left, t_vec2i * right);

/** mult the vec2i by the given scalar */
t_vec2i * vec2i_mult(t_vec2i * dst, t_vec2i * vec, int scalar);
t_vec2i * vec2i_mult2(t_vec2i * dst, t_vec2i * left, t_vec2i * right);

/** scale product */
int vec2i_dot_product(t_vec2i * left, t_vec2i * right);

/** length */
int vec2i_length_squared(t_vec2i * vec);
int vec2i_length(t_vec2i * vec);

/** normalize */
t_vec2i * vec2i_normalize(t_vec2i * dst, t_vec2i * vec);

/** negate */
t_vec2i * vec2i_negate(t_vec2i * dst, t_vec2i * src);

/** angle between two vec */
int vec2i_angle(t_vec2i * left, t_vec2i * right);

/** mix the two vectors */
t_vec2i * vec2i_mix(t_vec2i * dst, t_vec2i * left, t_vec2i * right, int ratio);

/** comparison */
int vec2i_equals(t_vec2i * left, t_vec2i * right);

/** to string: return a string allocated with malloc() */
char * vec2i_str(t_vec2i * vec);

#endif