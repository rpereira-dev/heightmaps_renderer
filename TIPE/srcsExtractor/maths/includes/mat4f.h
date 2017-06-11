#ifndef MAT4F_H
# define MAT4F_H

# include "cmaths.h"
# include "vec.h"

typedef struct	s_mat4f {
	float m00;
	float m01;
	float m02;
	float m03;

	float m10;
	float m11;
	float m12;
	float m13;

	float m20;
	float m21;
	float m22;
	float m23;

	float m30;
	float m31;
	float m32;
	float m33;
}				t_mat4f;

/** create a new matrix */
t_mat4f * mat4f_new(void);

/** delete the given matrix */
void mat4f_delete(t_mat4f * mat);

/** copy */
t_mat4f * mat4f_copy(t_mat4f * dst, t_mat4f * src);

/** set identity */
t_mat4f * mat4f_identity(t_mat4f * dst);

/** set to zero */
t_mat4f * mat4f_zero(t_mat4f * dst);

/** transpose */
t_mat4f * mat4f_transpose(t_mat4f * dst, t_mat4f * src);

/** scale */
t_mat4f * mat4f_scale(t_mat4f * dst, t_mat4f * mat, float f);

/** translate */
t_mat4f * mat4f_translate(t_mat4f * dst, t_mat4f * src, float tx, float ty, float tz);
t_mat4f * mat4f_translate3(t_mat4f * dst, t_mat4f * src, t_vec3f * translate);

/** rotate */
t_mat4f * mat4f_rotate(t_mat4f * dst, t_mat4f * src, float angle, t_vec3f * axis);
t_mat4f * mat4f_rotateX(t_mat4f * dst, t_mat4f * src, float angle);
t_mat4f * mat4f_rotateY(t_mat4f * dst, t_mat4f * src, float angle);
t_mat4f * mat4f_rotateZ(t_mat4f * dst, t_mat4f * src, float angle);
t_mat4f * mat4f_rotateXYZ(t_mat4f * dst, t_mat4f * src, t_vec3f * rot);

/** transformation matrix */
t_mat4f * mat4f_transformation(t_mat4f * dst, t_vec3f * translate, t_vec3f * rot, t_vec3f * scale);

/** determinant */
float mat4f_determinant(t_mat4f * mat);

/** invert */
t_mat4f * mat4f_invert(t_mat4f * dst, t_mat4f * src);

/** mult */
t_mat4f * mat4f_mult(t_mat4f * dst, t_mat4f * left, t_mat4f * right);

/** transform vec4f */
t_vec4f * mat4f_transform_vec4f(t_vec4f * dst, t_mat4f * left, t_vec4f * right);

/** projections matrix bellow: */

/** orthographic matrix */
t_mat4f * mat4f_orthographic(t_mat4f * dst, float left, float right, float bot, float top, float near, float far);

/** perspective matrix */
t_mat4f * mat4f_perspective(t_mat4f * dst, float aspect, float fov, float near, float far);

/** to string: return a string allocated with malloc() */
char * mat4f_str(t_mat4f * mat);

#endif