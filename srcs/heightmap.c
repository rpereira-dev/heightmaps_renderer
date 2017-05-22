#include "renderer.h"


int heightmapGetHeight(t_image * image, int x, int y) {
	unsigned char * rgb = (unsigned char*)(image + 1);
	int idx = (x * image->w + y) * 3;
	unsigned char b = rgb[idx + 0];
	unsigned char g = rgb[idx + 1];
	unsigned char r = rgb[idx + 2];
	return (r + g + b);
}
