#include "vec2i.h"

t_vec2i * vec2i_new(void) {
	return ((t_vec2i *)malloc(sizeof(t_vec2i)));
}

void vec2i_delete(t_vec2i * vec) {
	free(vec);
}

t_vec2i * vec2i_zero(t_vec2i * dst) {
	if (dst == NULL) {
		if ((dst = vec2i_new()) == NULL) {
			return (NULL);
		}
	}
	memset(dst, 0, sizeof(t_vec2i));
	return (dst);
}

t_vec2i * vec2i_set(t_vec2i * dst, int x, int y) {
	if (dst == NULL) {
		if ((dst = vec2i_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = x;
	dst->y = y;
	return (dst);
}

t_vec2i * vec2i_set2(t_vec2i * dst, t_vec2i * vec) {
	if (dst == vec) {
		return (dst);
	}
	return (vec2i_set(dst, vec->x, vec->y));
}

t_vec2i * vec2i_add(t_vec2i * dst, t_vec2i * left, t_vec2i * right) {
	if (dst == NULL) {
		if ((dst = vec2i_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = left->x + right->x;
	dst->y = left->y + right->y;
	return (dst);
}

t_vec2i * vec2i_sub(t_vec2i * dst, t_vec2i * left, t_vec2i * right) {
	if (dst == NULL) {
		if ((dst = vec2i_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = left->x - right->x;
	dst->y = left->y - right->y;
	return (dst);
}

t_vec2i * vec2i_mult(t_vec2i * dst, t_vec2i * vec, int scalar) {
	if (dst == NULL) {
		if ((dst = vec2i_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = vec->x * scalar;
	dst->y = vec->y * scalar;
	return (dst);
}

t_vec2i * vec2i_mult2(t_vec2i * dst, t_vec2i * left, t_vec2i * right) {
	if (dst == NULL) {
		if ((dst = vec2i_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = left->x * right->x;
	dst->y = left->y * right->y;
	return (dst);
}

int vec2i_dot_product(t_vec2i * left, t_vec2i * right) {
	return (left->x * right->x + left->y * right->y);
}

int vec2i_length_squared(t_vec2i * vec) {
	return (vec2i_dot_product(vec, vec));
}

int vec2i_length(t_vec2i * vec) {
	return ((int)sqrt(vec2i_length_squared(vec)));
}

t_vec2i * vec2i_normalize(t_vec2i * dst, t_vec2i * vec) {

	if (dst == NULL) {
		if ((dst = vec2i_new()) == NULL) {
			return (NULL);
		}
	}

	int norm = 1 / vec2i_length(vec);
	dst->x = vec->x * norm;
	dst->y = vec->y * norm;
	return (dst);
}

t_vec2i * vec2i_negate(t_vec2i * dst, t_vec2i * src) {
	if (dst == NULL) {
		if ((dst = vec2i_new()) == NULL) {
			return (NULL);
		}
	}
	dst->x = -src->x;
	dst->y = -src->y;
	return (dst);
}

int vec2i_angle(t_vec2i * left, t_vec2i * right) {
	int dls = vec2i_dot_product(left, right) / (vec2i_length(left) * vec2i_length(right));
	if (dls < -1.0f) {
		dls = -1.0f;
	} else if (dls > 1.0f) {
		dls = 1.0f;
	}
	return ((int)acos(dls));
}

t_vec2i * vec2i_mix(t_vec2i * dst, t_vec2i * left, t_vec2i * right, int ratio) {

	if (dst == NULL) {
		if ((dst = vec2i_new()) == NULL) {
			return (NULL);
		}
	}

	dst->x = left->x * ratio + right->x * (1 - ratio);
	dst->y = left->y * ratio + right->y * (1 - ratio);
	return (dst);
}

int vec2i_equals(t_vec2i * left, t_vec2i * right) {
	return (left == right || (left->x == right->x && left->y == right->y));
}

char * vec2i_str(t_vec2i * vec) {
	if (vec == NULL) {
		return (strdup("vec2i(NULL)"));
	}
	char buffer[128];
	sprintf(buffer, "vec2i(%d ; %d)", vec->x, vec->y);
	return (strdup(buffer));
}
