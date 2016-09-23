#include "renderer.h"

void render(t_world * world, t_camera * camera) {
	(void)world;
	(void)camera;
}

int main(void) {

	t_world world;
	t_camera camera;

	printf("Creating gl context...\n");
	t_glh_context * context = glhCreateContext();

	printf("Make gl context current...\n");
    glhMakeContextCurrent(context);

	printf("Loop started...\n");
    while (!glhWindowShouldClose(context->window)) {

    	glhClear(GL_COLOR_BUFFER_BIT);
    	render(&world, &camera);
    	glhSwapBuffer(context->window);
    	glhPollEvents();
    }

	printf("Loop ended.\n");

	printf("Destroying gl context..\n");
    glhDestroyContext(context);

    printf("Stop openGL...\n");
    glhStop();

	printf("All done.\n");

	return (0);
}