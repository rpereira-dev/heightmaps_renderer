#include "renderer.h"

# define BMP_HEADER_SIZE 54

t_heightmap * heightmapNew(char const * path) {

	int fd = open(path, O_RDONLY);
	if (fd == -1) {
		return (NULL);
	}

	char header[BMP_HEADER_SIZE];
	read(fd, &header, sizeof(header));

	//magic
	if (header[0] != 'B' || header[1] != 'M') {
		close(fd);
		return (NULL);
	}

	int offset = *((int*)(header + 0x0A));
	int w = *((int*)(header + 0x12));
	int h = *((int*)(header + 0x16));
	t_heightmap * map = (t_heightmap*)malloc(sizeof(t_heightmap) + 3 * w * h);
	if (map == NULL) {
		close(fd);
		return (NULL);
	}
	map->w = w;
	map->h = h;

	//read useless bytes
	lseek(fd, offset - BMP_HEADER_SIZE, SEEK_CUR);

	//read raw bytes
	read(fd, map + 1, w * h * 3);
	close(fd);

	return (map);
}

int heightmapGetHeight(t_heightmap * map, int x, int y) {
	unsigned char * rgb = (unsigned char*)(map + 1);
	int idx = (x * map->w + y) * 3;
	unsigned char b = rgb[idx + 0];
	unsigned char g = rgb[idx + 1];
	unsigned char r = rgb[idx + 2];
	return (r + g + b);
}

void heightmapDelete(t_heightmap * map) {
	free(map);
}