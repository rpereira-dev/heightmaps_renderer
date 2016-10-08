#include "renderer.h"

void printUsage(char * binary, FILE * dst) {
	fprintf(dst, "usage: ./%s [FLAGS]\n", binary);
	fprintf(dst, "flags available are:\n");
	fprintf(dst, "\t-r {SEED} for random/infinite terrain generation\n");
	fprintf(dst, "\t-t [FILE] for a bmp terrain heightmap loading\n");
	fprintf(dst, "\t-h [MAX_HEIGHT] to define maximum height of meshes\n");
	fprintf(dst, "examples:\n");
	fprintf(dst, "\t./%s -r 42 -h 256\n", binary);
	fprintf(dst, "\t./%s -t \"texture.bmp\" -h 12\n", binary);
}

int main(int argc, char **argv) {

	//get binary name
	char * binary = argc == 0 ? "renderer" : argv[0];

	//parse arguments
    int optind;
    char mode = 'r';
    long seed = time(NULL);
    int maxheight = 64;
    char * bmpfile = NULL;
    for (optind = 1; optind < argc; optind++) {

    	if (argv[optind][0] != '-' || argv[optind][2] != 0) {
    		 printUsage(binary, stderr); return (EXIT_FAILURE);
    	}

    	mode = argv[optind][1];

    	switch (argv[optind][1]) {
    		case 'r': if (optind + 1 < argc) { seed = atoi(argv[++optind]); } break;
    		case 't': if (optind + 1 >= argc) { printUsage(binary, stderr); return (EXIT_FAILURE); } else { bmpfile = strdup(argv[++optind]); } break;
    		case 'h': if (optind + 1 >= argc) { printUsage(binary, stderr); return (EXIT_FAILURE); } else { maxheight = atoi(argv[++optind]); } break;
    		default: printUsage(binary, stderr); return (EXIT_FAILURE);
        }   
    }

    if (argc <= 1) {
		printUsage(binary, stdout);
		printf("\n");
    }
    printf("{mode='%c'}, {seed='%ld'}, {maxheight='%d'}, {bmpfile='%s'}\n\n", mode, seed, maxheight, bmpfile);

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

	t_world world;
	t_renderer renderer;
	t_camera camera;

	printf("Initializing camera...\n");
	cameraInit(&camera);

	printf("Initializing renderer...\n");
	rendererInit(&renderer);

	printf("Initializing inputs...\n");
	inputInit(context);

	printf("Initializing world...\n");
	worldInit(&world);

	printf("Loop started...\n");
    while (!glhWindowShouldClose(context->window)) {

    	//update the window
    	glhWindowUpdate(context->window);

    	//input
    	inputUpdate(context, &world, &renderer, &camera);
    	
    	//update the camera and the world
    	cameraUpdate(&camera);
    	worldUpdate(&world, &camera);

    	//update the renderer
    	rendererUpdate(context, &world, &renderer, &camera);

    	//render
    	rendererRender(context, &world, &renderer, &camera);
    	
    	glhCheckError("post rendererUpdate()");

    	//swap buffers
    	glhSwapBuffer(context->window);
    }

	printf("Loop ended\n");

	printf("Deleting camera...\n");
	cameraDelete(&camera);

	printf("Deleting world...\n");
	worldDelete(&world);

	printf("Deleting renderer...\n");
	rendererDelete(&renderer);

	printf("Destroying gl context..\n");
    glhDestroyContext(context);

    printf("Stopping openGL...\n");
    glhStop();

	printf("All done\n");

	return (0);
}