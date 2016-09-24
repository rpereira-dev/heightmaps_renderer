#include "renderer.h"

int main(void) {


	printf("Initializing openGL...\n");

	glhInit();

	printf("Creating gl context...\n");

	t_glh_context * context = glhCreateContext();
	if (context == NULL) {
		fprintf(stderr, "Failed to create gl context.\n");
		return (EXIT_FAILURE);
	}

	if (context->window == NULL) {
		fprintf(stderr, "Failed to create gl window.\n");
		return (EXIT_FAILURE);
	}

	printf("Making gl context current...\n");
    glhMakeContextCurrent(context);

	printf("Loop started...\n");

	t_world world;
	t_renderer renderer;
	t_camera camera;


	worldInit(&world);
	rendererInit(&renderer);
	cameraInit(&camera);

    while (!glhWindowShouldClose(context->window)) {

    	glhClear(GL_COLOR_BUFFER_BIT);
    	
    	glhWindowUpdate(context->window);
    	cameraUpdate(&camera);
    	worldUpdate(&world, &camera);
    	rendererUpdate(context, &world, &renderer, &camera);

    	glhSwapBuffer(context->window);
    }

	printf("Loop ended.\n");

	printf("Destroying gl context..\n");
    glhDestroyContext(context);

    printf("Stop openGL...\n");
    glhStop();

	printf("All done.\n");

	return (0);
}