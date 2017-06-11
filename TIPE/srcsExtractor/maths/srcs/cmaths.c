#include "cmaths.h"

float * sin_table;

float atan_f(float x) {
	float xabs = ABS(x);
	return (0.78539816339f * x - x * (xabs - 1) * (0.2447f + 0.0663f * xabs));
}

float acos_f(float x) {
	//lagrange interpolation:
	return ((-0.69813170079773212f * x * x - 0.87266462599716477f) * x + 1.5707963267948966f);
}

float asin_f(float x) {
	return (-acos_f(x) + 1.5707963267948966f);
}

float tan_f(float x) {
	return (sin_f(x) / cos_f(x));
}

float sin_f(float x) {
	unsigned int index = (unsigned int)RAD_TO_DEG(ABS(x)) % 360;
	return (index > 180 ? -sin_table[index - 180] : sin_table[index]);
}

float cos_f(float x) {
	return (sin_f(x + 1.5707963267948966f));
}

float sqrt_f(float x) {
	unsigned int i = *(unsigned int*) &x;
	i  += 127 << 23;
	i >>= 1;
	return (*(float*) &i);
}

int cmaths_init(void) {

	sin_table = (float *) malloc(sizeof(float) * 4 * 360);
	if (sin_table == NULL) {
		return (0);
	}

	int i, j;
	for (i = 0, j = 0 ; i < 180; i++) {
		sin_table[j++] = (float)sin((double)DEG_TO_RAD((float)i));
	}

	return (1);
}

int cmaths_deinit(void) {
	free(sin_table);
	sin_table = NULL;
	return (1);
}
