#include "renderer.h"

typedef struct 	s_bmp_header {
	char magic[2];
	int size;
	int reserved;
	int offset;
}				t_bmp_header;

t_heightmap * heightmapNew(char const * path) {

	int fd = open(path, O_RDONLY);
	if (fd == -1) {
		return (NULL);
	}

	float w, h;

	t_heightmap * map = (t_heightmap*)malloc(sizeof(t_heightmap) + sizeof(float) * w * h);
	if (map == NULL) {
		return (NULL);
	}
	map->w = w;
	map->h = h;




	close(fd);
	return (map);
}

float heightMapGetHeight(t_heightmap * map, int x, int y) {
	float * heights = (float*)(map + sizeof(t_heightmap));
	return (heights[x * map->w + y]);
}

void heightMapDelete(t_heightmap * map) {
	free(map);
}