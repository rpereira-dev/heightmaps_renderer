#include "renderer.h"

# define BMP_HEADER_SIZE (54)

t_image * imageNew(char const * path) {

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
	t_image * image = (t_image*)malloc(sizeof(t_image) + 3 * w * h);
	if (image == NULL) {
		close(fd);
		return (NULL);
	}
	image->w = w;
	image->h = h;

	//read useless bytes
	lseek(fd, offset - BMP_HEADER_SIZE, SEEK_CUR);

	//read raw bytes
	read(fd, image + 1, w * h * 3);
	close(fd);

	return (image);
}

void imageDelete(t_image * map) {
	free(map);
}
