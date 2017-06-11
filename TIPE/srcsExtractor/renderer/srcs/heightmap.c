#include "renderer.h"

int heightmapGetHeight(t_image * image, float x, float y) {
	int idx = ((int)x * image->h + (int)y) * 3;
	unsigned char * rgb = (unsigned char*)(image + 1);
	unsigned char b = rgb[idx + 0];
	unsigned char g = rgb[idx + 1];
	unsigned char r = rgb[idx + 2];
	return (r + g + b);
}
