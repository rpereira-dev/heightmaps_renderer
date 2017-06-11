#include "mat4f.h"

t_mat4f * mat4f_new(void) {
	return ((t_mat4f*)malloc(sizeof(t_mat4f)));
}

void mat4f_delete(t_mat4f * mat) {
	free(mat);
}

t_mat4f * mat4f_copy(t_mat4f * dst, t_mat4f * src) {
	if (dst == NULL) {
		if ((dst = mat4f_new()) == NULL) {
			return (NULL);
		}
	}
	memcpy(dst, src, sizeof(t_mat4f));
	return (dst);
}

t_mat4f * mat4f_identity(t_mat4f * dst) {
	if (dst == NULL) {
		if ((dst = mat4f_new()) == NULL) {
			return (NULL);
		}
	}

	dst->m00 = 1, dst->m01 = 0, dst->m02 = 0, dst->m03 = 0;
	dst->m10 = 0, dst->m11 = 1, dst->m12 = 0, dst->m13 = 0;
	dst->m20 = 0, dst->m21 = 0, dst->m22 = 1, dst->m23 = 0;
	dst->m30 = 0, dst->m31 = 0, dst->m32 = 0, dst->m33 = 1;
	return (dst);
}

t_mat4f * mat4f_zero(t_mat4f * dst) {
	if (dst == NULL) {
		if ((dst = mat4f_new()) == NULL) {
			return (NULL);
		}
	}
	memset(dst, 0, sizeof(t_mat4f));
	return (dst);
}

t_mat4f * mat4f_transpose(t_mat4f * dst, t_mat4f * src) {

	if (dst == NULL) {
		if ((dst = mat4f_new()) == NULL) {
			return (NULL);
		}
	}

	float m00 = src->m00;
	float m01 = src->m10;
	float m02 = src->m20;
	float m03 = src->m30;
	float m10 = src->m01;
	float m11 = src->m11;
	float m12 = src->m21;
	float m13 = src->m31;
	float m20 = src->m02;
	float m21 = src->m12;
	float m22 = src->m22;
	float m23 = src->m32;
	float m30 = src->m03;
	float m31 = src->m13;
	float m32 = src->m23;
	float m33 = src->m33;

	dst->m00 = m00, dst->m01 = m01, dst->m02 = m02, dst->m03 = m03;
	dst->m10 = m10, dst->m11 = m11, dst->m12 = m12, dst->m13 = m13;
	dst->m20 = m20, dst->m21 = m21, dst->m22 = m22, dst->m23 = m23;
	dst->m30 = m30, dst->m31 = m31, dst->m32 = m32, dst->m33 = m33;

	return (dst);
}

t_mat4f * mat4f_scale(t_mat4f * dst, t_mat4f * src, float scale) {

	if (dst == NULL) {
		if ((dst = mat4f_new()) == NULL) {
			return (NULL);
		}
	}

	dst->m00 = src->m00 * scale, dst->m01 = src->m01 * scale, dst->m02 = src->m02 * scale, dst->m03 = src->m03 * scale;
	dst->m10 = src->m10 * scale, dst->m11 = src->m11 * scale, dst->m12 = src->m12 * scale, dst->m13 = src->m13 * scale;
	dst->m20 = src->m20 * scale, dst->m21 = src->m21 * scale, dst->m22 = src->m22 * scale, dst->m23 = src->m23 * scale;

	return (dst);
}

t_mat4f * mat4f_scale3(t_mat4f * dst, t_mat4f * src, t_vec3f * scale) {

	if (dst == NULL) {
		if ((dst = mat4f_new()) == NULL) {
			return (NULL);
		}
	}
	
	dst->m00 = src->m00 * scale->x, dst->m01 = src->m01 * scale->x, dst->m02 = src->m02 * scale->x, dst->m03 = src->m03 * scale->x;
	dst->m10 = src->m10 * scale->y, dst->m11 = src->m11 * scale->y, dst->m12 = src->m12 * scale->y, dst->m13 = src->m13 * scale->y;
	dst->m20 = src->m20 * scale->z, dst->m21 = src->m21 * scale->z, dst->m22 = src->m22 * scale->z, dst->m23 = src->m23 * scale->z;

	return (dst);	
}

t_mat4f * mat4f_translate(t_mat4f * dst, t_mat4f * src, float tx, float ty, float tz) {
	if (dst == NULL) {
		if ((dst = mat4f_new()) == NULL) {
			return (NULL);
		}
	}

	dst->m30 += src->m00 * tx + src->m10 * ty + src->m20 * tz;
	dst->m31 += src->m01 * tx + src->m11 * ty + src->m21 * tz;
	dst->m32 += src->m02 * tx + src->m12 * ty + src->m22 * tz;
	dst->m33 += src->m03 * tx + src->m13 * ty + src->m23 * tz;
	return (dst);
}

t_mat4f * mat4f_translate3(t_mat4f * dst, t_mat4f * src, t_vec3f * translate) {
	return (mat4f_translate(dst, src, translate->x, translate->y, translate->z));
}

t_mat4f * mat4f_rotate(t_mat4f * dst, t_mat4f * src, float angle, t_vec3f * axis) {

	if (dst == NULL) {
		if ((dst = mat4f_new()) == NULL) {
			return (NULL);
		}
	}

	float c = (float)cos(angle);
	float s = (float)sin(angle);
	float oneminusc = 1.0f - c;
	float xy = axis->x * axis->y;
	float yz = axis->y * axis->z;
	float xz = axis->x * axis->z;
	float xs = axis->x * s;
	float ys = axis->y * s;
	float zs = axis->z * s;

	float f00 = axis->x * axis->x * oneminusc + c;
	float f01 = xy * oneminusc + zs;
	float f02 = xz * oneminusc - ys;
	// n[3] not used
	float f10 = xy * oneminusc - zs;
	float f11 = axis->y * axis->y * oneminusc + c;
	float f12 = yz * oneminusc + xs;
	// n[7] not used
	float f20 = xz * oneminusc + ys;
	float f21 = yz * oneminusc - xs;
	float f22 = axis->z * axis->z * oneminusc + c;

	float t00 = src->m00 * f00 + src->m10 * f01 + src->m20 * f02;
	float t01 = src->m01 * f00 + src->m11 * f01 + src->m21 * f02;
	float t02 = src->m02 * f00 + src->m12 * f01 + src->m22 * f02;
	float t03 = src->m03 * f00 + src->m13 * f01 + src->m23 * f02;
	float t10 = src->m00 * f10 + src->m10 * f11 + src->m20 * f12;
	float t11 = src->m01 * f10 + src->m11 * f11 + src->m21 * f12;
	float t12 = src->m02 * f10 + src->m12 * f11 + src->m22 * f12;
	float t13 = src->m03 * f10 + src->m13 * f11 + src->m23 * f12;
	dst->m20 = src->m00 * f20 + src->m10 * f21 + src->m20 * f22;
	dst->m21 = src->m01 * f20 + src->m11 * f21 + src->m21 * f22;
	dst->m22 = src->m02 * f20 + src->m12 * f21 + src->m22 * f22;
	dst->m23 = src->m03 * f20 + src->m13 * f21 + src->m23 * f22;
	dst->m00 = t00;
	dst->m01 = t01;
	dst->m02 = t02;
	dst->m03 = t03;
	dst->m10 = t10;
	dst->m11 = t11;
	dst->m12 = t12;
	dst->m13 = t13;
	return (dst);
}

t_mat4f * mat4f_rotateX(t_mat4f * dst, t_mat4f * src, float angle) {

	if (dst == NULL) {
		if ((dst = mat4f_new()) == NULL) {
			return (NULL);
		}
	}

	float c = (float)cos(angle);
	float s = (float)sin(angle);
	float t00 = src->m00;
	float t01 = src->m01;
	float t02 = src->m02;
	float t03 = src->m03;
	float t10 = src->m10 * c + src->m20 * s;
	float t11 = src->m11 * c + src->m21 * s;
	float t12 = src->m12 * c + src->m22 * s;
	float t13 = src->m13 * c + src->m23 * s;
	dst->m20 = src->m10 * -s + src->m20 * c;
	dst->m21 = src->m11 * -s + src->m21 * c;
	dst->m22 = src->m12 * -s + src->m22 * c;
	dst->m23 = src->m13 * -s + src->m23 * c;
	dst->m00 = t00;
	dst->m01 = t01;
	dst->m02 = t02;
	dst->m03 = t03;
	dst->m10 = t10;
	dst->m11 = t11;
	dst->m12 = t12;
	dst->m13 = t13;

	return (dst);
}

t_mat4f * mat4f_rotateY(t_mat4f * dst, t_mat4f * src, float angle) {

	if (dst == NULL) {
		if ((dst = mat4f_new()) == NULL) {
			return (NULL);
		}
	}

	float c = (float)cos(angle);
	float s = (float)sin(angle);
	float t00 = src->m00 * c + src->m20 * -s;
	float t01 = src->m01 * c + src->m21 * -s;
	float t02 = src->m02 * c + src->m22 * -s;
	float t03 = src->m03 * c + src->m23 * -s;
	float t10 = src->m10;
	float t11 = src->m11;
	float t12 = src->m12;
	float t13 = src->m13;
	dst->m20 = src->m00 * s + src->m20 * c;
	dst->m21 = src->m01 * s + src->m21 * c;
	dst->m22 = src->m02 * s + src->m22 * c;
	dst->m23 = src->m03 * s + src->m23 * c;
	dst->m00 = t00;
	dst->m01 = t01;
	dst->m02 = t02;
	dst->m03 = t03;
	dst->m10 = t10;
	dst->m11 = t11;
	dst->m12 = t12;
	dst->m13 = t13;
	return (dst);
}

t_mat4f * mat4f_rotateZ(t_mat4f * dst, t_mat4f * src, float angle) {
	if (dst == NULL) {
		if ((dst = mat4f_new()) == NULL) {
			return (NULL);
		}
	}

	float c = (float)cos(angle);
	float s = (float)sin(angle);
	float t00 = src->m00 * c + src->m10 * s;
	float t01 = src->m01 * c + src->m11 * s;
	float t02 = src->m02 * c + src->m12 * s;
	float t03 = src->m03 * c + src->m13 * s;
	float t10 = src->m00 * -s + src->m10 * c;
	float t11 = src->m01 * -s + src->m11 * c;
	float t12 = src->m02 * -s + src->m12 * c;
	float t13 = src->m03 * -s + src->m13 * c;
	dst->m20 = src->m20;
	dst->m21 = src->m21;
	dst->m22 = src->m22;
	dst->m23 = src->m23;
	dst->m00 = t00;
	dst->m01 = t01;
	dst->m02 = t02;
	dst->m03 = t03;
	dst->m10 = t10;
	dst->m11 = t11;
	dst->m12 = t12;
	dst->m13 = t13;
	return (dst);
}

t_mat4f * mat4f_rotateXYZ(t_mat4f * dst, t_mat4f * src, t_vec3f * rot) {
	if (dst == NULL) {
		if ((dst = mat4f_new()) == NULL) {
			return (NULL);
		}
	}

	mat4f_rotateX(dst, src, rot->x);
	mat4f_rotateY(dst, src, rot->y);
	mat4f_rotateZ(dst, src, rot->z);
	return (dst);
}

t_mat4f * mat4f_transformation(t_mat4f * dst, t_vec3f * translate, t_vec3f * rot, t_vec3f * scale) {

	if ((dst = mat4f_identity(dst)) == NULL) {
		return (NULL);
	}

	mat4f_translate3(dst, dst, translate);
	mat4f_rotateXYZ(dst, dst, rot);
	mat4f_scale3(dst, dst, scale);
	return (dst);
}

float mat4f_determinant(t_mat4f * mat) {

	if (mat == NULL) {
		return (0);
	}

	float d = 0;
	d += mat->m00 * ((mat->m11 * mat->m22 * mat->m33 + mat->m12 * mat->m23 * mat->m31 + mat->m13 * mat->m21 * mat->m32)
		- mat->m13 * mat->m22 * mat->m31 - mat->m11 * mat->m23 * mat->m32 - mat->m12 * mat->m21 * mat->m33);
	d -= mat->m01 * ((mat->m10 * mat->m22 * mat->m33 + mat->m12 * mat->m23 * mat->m30 + mat->m13 * mat->m20 * mat->m32)
		- mat->m13 * mat->m22 * mat->m30 - mat->m10 * mat->m23 * mat->m32 - mat->m12 * mat->m20 * mat->m33);
	d += mat->m02 * ((mat->m10 * mat->m21 * mat->m33 + mat->m11 * mat->m23 * mat->m30 + mat->m13 * mat->m20 * mat->m31)
		- mat->m13 * mat->m21 * mat->m30 - mat->m10 * mat->m23 * mat->m31 - mat->m11 * mat->m20 * mat->m33);
	d -= mat->m03 * ((mat->m10 * mat->m21 * mat->m32 + mat->m11 * mat->m22 * mat->m30 + mat->m12 * mat->m20 * mat->m31)
		- mat->m12 * mat->m21 * mat->m30 - mat->m10 * mat->m22 * mat->m31 - mat->m11 * mat->m20 * mat->m32);
	return (d);
}

static float _mat4f_determinant3x3(float t00, float t01, float t02, float t10, float t11, float t12, float t20, float t21, float t22) {
	return (t00 * (t11 * t22 - t12 * t21) + t01 * (t12 * t20 - t10 * t22) + t02 * (t10 * t21 - t11 * t20));
}

t_mat4f * mat4f_invert(t_mat4f * dst, t_mat4f * src) {

	float determinant = mat4f_determinant(src);

	if (determinant != 0) {
		if (dst == NULL) {
			if ((dst = mat4f_new()) == NULL) {
				return (NULL);
			}
		}

		float determinant_inv = 1.0f / determinant;

		float t00 = _mat4f_determinant3x3(src->m11, src->m12, src->m13, src->m21, src->m22, src->m23, src->m31, src->m32, src->m33);
		float t01 = -_mat4f_determinant3x3(src->m10, src->m12, src->m13, src->m20, src->m22, src->m23, src->m30, src->m32, src->m33);
		float t02 = _mat4f_determinant3x3(src->m10, src->m11, src->m13, src->m20, src->m21, src->m23, src->m30, src->m31, src->m33);
		float t03 = -_mat4f_determinant3x3(src->m10, src->m11, src->m12, src->m20, src->m21, src->m22, src->m30, src->m31, src->m32);

		float t10 = -_mat4f_determinant3x3(src->m01, src->m02, src->m03, src->m21, src->m22, src->m23, src->m31, src->m32, src->m33);
		float t11 = _mat4f_determinant3x3(src->m00, src->m02, src->m03, src->m20, src->m22, src->m23, src->m30, src->m32, src->m33);
		float t12 = -_mat4f_determinant3x3(src->m00, src->m01, src->m03, src->m20, src->m21, src->m23, src->m30, src->m31, src->m33);
		float t13 = _mat4f_determinant3x3(src->m00, src->m01, src->m02, src->m20, src->m21, src->m22, src->m30, src->m31, src->m32);

		float t20 = _mat4f_determinant3x3(src->m01, src->m02, src->m03, src->m11, src->m12, src->m13, src->m31, src->m32, src->m33);
		float t21 = -_mat4f_determinant3x3(src->m00, src->m02, src->m03, src->m10, src->m12, src->m13, src->m30, src->m32, src->m33);
		float t22 = _mat4f_determinant3x3(src->m00, src->m01, src->m03, src->m10, src->m11, src->m13, src->m30, src->m31, src->m33);
		float t23 = -_mat4f_determinant3x3(src->m00, src->m01, src->m02, src->m10, src->m11, src->m12, src->m30, src->m31, src->m32);

		float t30 = -_mat4f_determinant3x3(src->m01, src->m02, src->m03, src->m11, src->m12, src->m13, src->m21, src->m22, src->m23);
		float t31 = _mat4f_determinant3x3(src->m00, src->m02, src->m03, src->m10, src->m12, src->m13, src->m20, src->m22, src->m23);
		float t32 = -_mat4f_determinant3x3(src->m00, src->m01, src->m03, src->m10, src->m11, src->m13, src->m20, src->m21, src->m23);
		float t33 = _mat4f_determinant3x3(src->m00, src->m01, src->m02, src->m10, src->m11, src->m12, src->m20, src->m21, src->m22);

		// transpose and divide by the determinant
		dst->m00 = t00 * determinant_inv;
		dst->m11 = t11 * determinant_inv;
		dst->m22 = t22 * determinant_inv;
		dst->m33 = t33 * determinant_inv;
		dst->m01 = t10 * determinant_inv;
		dst->m10 = t01 * determinant_inv;
		dst->m20 = t02 * determinant_inv;
		dst->m02 = t20 * determinant_inv;
		dst->m12 = t21 * determinant_inv;
		dst->m21 = t12 * determinant_inv;
		dst->m03 = t30 * determinant_inv;
		dst->m30 = t03 * determinant_inv;
		dst->m13 = t31 * determinant_inv;
		dst->m31 = t13 * determinant_inv;
		dst->m32 = t23 * determinant_inv;
		dst->m23 = t32 * determinant_inv;
		return (dst);
	}

	return (NULL);
}

t_mat4f * mat4f_mult(t_mat4f * dst, t_mat4f * left, t_mat4f * right) {

	if (dst == NULL) {
		if ((dst = mat4f_new()) == NULL) {
			return (NULL);
		}
	}

	float m00 = left->m00 * right->m00 + left->m10 * right->m01 + left->m20 * right->m02 + left->m30 * right->m03;
	float m01 = left->m01 * right->m00 + left->m11 * right->m01 + left->m21 * right->m02 + left->m31 * right->m03;
	float m02 = left->m02 * right->m00 + left->m12 * right->m01 + left->m22 * right->m02 + left->m32 * right->m03;
	float m03 = left->m03 * right->m00 + left->m13 * right->m01 + left->m23 * right->m02 + left->m33 * right->m03;
	float m10 = left->m00 * right->m10 + left->m10 * right->m11 + left->m20 * right->m12 + left->m30 * right->m13;
	float m11 = left->m01 * right->m10 + left->m11 * right->m11 + left->m21 * right->m12 + left->m31 * right->m13;
	float m12 = left->m02 * right->m10 + left->m12 * right->m11 + left->m22 * right->m12 + left->m32 * right->m13;
	float m13 = left->m03 * right->m10 + left->m13 * right->m11 + left->m23 * right->m12 + left->m33 * right->m13;
	float m20 = left->m00 * right->m20 + left->m10 * right->m21 + left->m20 * right->m22 + left->m30 * right->m23;
	float m21 = left->m01 * right->m20 + left->m11 * right->m21 + left->m21 * right->m22 + left->m31 * right->m23;
	float m22 = left->m02 * right->m20 + left->m12 * right->m21 + left->m22 * right->m22 + left->m32 * right->m23;
	float m23 = left->m03 * right->m20 + left->m13 * right->m21 + left->m23 * right->m22 + left->m33 * right->m23;
	float m30 = left->m00 * right->m30 + left->m10 * right->m31 + left->m20 * right->m32 + left->m30 * right->m33;
	float m31 = left->m01 * right->m30 + left->m11 * right->m31 + left->m21 * right->m32 + left->m31 * right->m33;
	float m32 = left->m02 * right->m30 + left->m12 * right->m31 + left->m22 * right->m32 + left->m32 * right->m33;
	float m33 = left->m03 * right->m30 + left->m13 * right->m31 + left->m23 * right->m32 + left->m33 * right->m33;

	dst->m00 = m00;
	dst->m01 = m01;
	dst->m02 = m02;
	dst->m03 = m03;
	dst->m10 = m10;
	dst->m11 = m11;
	dst->m12 = m12;
	dst->m13 = m13;
	dst->m20 = m20;
	dst->m21 = m21;
	dst->m22 = m22;
	dst->m23 = m23;
	dst->m30 = m30;
	dst->m31 = m31;
	dst->m32 = m32;
	dst->m33 = m33;

	return (dst);
}

t_vec4f * mat4f_transform_vec4f(t_vec4f * dst, t_mat4f * left, t_vec4f * right) {
	if (dst == NULL) {
		if ((dst = vec4f_new()) == NULL) {
			return (NULL);
		}
	}

	float x = left->m00 * right->x + left->m10 * right->y + left->m20 * right->z + left->m30 * right->w;
	float y = left->m01 * right->x + left->m11 * right->y + left->m21 * right->z + left->m31 * right->w;
	float z = left->m02 * right->x + left->m12 * right->y + left->m22 * right->z + left->m32 * right->w;
	float w = left->m03 * right->x + left->m13 * right->y + left->m23 * right->z + left->m33 * right->w;

	dst->x = x;
	dst->y = y;
	dst->z = z;
	dst->w = w;

	return (dst);
}

t_mat4f * mat4f_orthographic(t_mat4f * dst, float left, float right, float bot, float top, float near, float far) {
	if (dst == NULL) {
		if ((dst = mat4f_new()) == NULL) {
			return (NULL);
		}
	}

	dst->m00 = 2.0f / (right - left);
	dst->m01 = 0;
	dst->m02 = 0;
	dst->m03 = (right + left) / (left - right);

	dst->m10 = 0;
	dst->m11 = 2.0f / (top - bot);
	dst->m12 = 0;
	dst->m13 = (top + bot) / (bot - top);

	dst->m20 = 0;
	dst->m21 = 0;
	dst->m22 = 2 / (near - far);
	dst->m23 = (far + near) / (near - far);

	dst->m30 = 0.0f;
	dst->m31 = 0.0f;
	dst->m32 = 0.0f;
	dst->m33 = 1.0f;

	return (dst);
}

t_mat4f * mat4f_perspective(t_mat4f * dst, float aspect, float fov, float near, float far) {
	if (dst == NULL) {
		if ((dst = mat4f_new()) == NULL) {
			return (NULL);
		}
	}

	float y_scale = (float) (1.0f / tan(fov / 2.0f) * aspect);
	float x_scale = y_scale / aspect;
	float frustrum_length = far - near;

	dst->m00 = x_scale;
	dst->m01 = 0.0f;
	dst->m02 = 0.0f;
	dst->m03 = 0.0f;

	dst->m10 = 0.0f;
	dst->m11 = y_scale;
	dst->m12 = 0.0f;
	dst->m13 = 0.0f;

	dst->m20 = 0.0f;
	dst->m21 = 0.0f;
	dst->m22 = -((far + near) / frustrum_length);
	dst->m23 = -1.0f;

	dst->m30 = 0.0f;
	dst->m31 = 0.0f;
	dst->m32 = -((2.0f * near * far) / frustrum_length);
	dst->m33 = 0.0f;

	return (dst);
}

char * mat4f_str(t_mat4f * mat) {
	if (mat == NULL) {
		return (strdup("mat4f(NULL)"));
	}
	char buffer[1024];
	sprintf(buffer, "mat4f(%.4f ; %.4f ; %.4f ; %.4f\n      %.4f ; %.4f ; %.4f ; %.4f\n      %.4f ; %.4f ; %.4f ; %.4f\n      %.4f ; %.4f ; %.4f ; %.4f)",
		mat->m00, mat->m10, mat->m20, mat->m30, mat->m01, mat->m11, mat->m21, mat->m31, mat->m02, mat->m12, mat->m22, mat->m32, mat->m03, mat->m13, mat->m23, mat->m33);
	return (strdup(buffer));
}
