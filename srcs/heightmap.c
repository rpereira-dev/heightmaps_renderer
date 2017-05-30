#include "renderer.h"


int heightmapGetHeight(t_image * image, float x, float y) {
	int px = (int)x;
	int py = image->h - (int)y - 1;
	int idx = (py * image->w + px) * 3;
	if (idx >= image->w * image->h * 3) {
		return (0);
	}
	unsigned char * rgb = (unsigned char*)(image + 1);
	unsigned char b = rgb[idx + 0];
	unsigned char g = rgb[idx + 1];
	unsigned char r = rgb[idx + 2];
	return (r + g + b);
}
