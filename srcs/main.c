#include "renderer.h"

void printUsage(char * binary, FILE * dst) {
	fprintf(dst, "usage: ./%s [FLAGS]\n", binary);
	fprintf(dst, "flags available are:\n");
	fprintf(dst, "\t-r {SEED} for random/infinite terrain generation\n");
	fprintf(dst, "\t-f [FILE] for a bmp terrain heightmap loading\n");
	fprintf(dst, "\t-h [MAX_HEIGHT] to define maximum height of meshes\n");
	fprintf(dst, "examples:\n");
	fprintf(dst, "\t./%s -r 42 -h 256\n", binary);
	fprintf(dst, "\t./%s -t \"texture.bmp\" -h 12\n", binary);
}
/*
static int threadLoop(void * e) {
	t_env 			* env 		= (t_env *) e;
	t_glh_context 	* context 	= env->context;
	t_renderer 		* renderer 	= &(env->renderer);
	t_world 		* world 	= &(env->world);
	t_camera 		* camera 	= &(env->camera);

	(void)env;
	(void)world;
	(void)camera;

	printf("Thread loop started!\n");

	while (env->is_running) {
		//update the camera and the world
    	worldUpdate(context, world, renderer, camera);

    	usleep(20 * 1000);

	}

	printf("Thread loop stopped!\n");

	return (0);
}*/

int main(int argc, char **argv) {

	//get binary name
	char * binary = argc == 0 ? "renderer" : argv[0];

	//parse arguments
    int optind;
    char mode = 'r';
    long seed = time(NULL);
    float maxheight = 1.0f;
    char * bmpfile = NULL;
    for (optind = 1; optind < argc; optind++) {

    	if (argv[optind][0] != '-' || argv[optind][2] != 0) {
    		 printUsage(binary, stderr); return (EXIT_FAILURE);
    	}

    	mode = argv[optind][1];

    	switch (argv[optind][1]) {
    		case 'r': if (optind + 1 < argc) { seed = atoi(argv[++optind]); } break;
    		case 'f': if (optind + 1 >= argc) { printUsage(binary, stderr); return (EXIT_FAILURE); } else { bmpfile = strdup(argv[++optind]); } break;
    		case 'h': if (optind + 1 >= argc) { printUsage(binary, stderr); return (EXIT_FAILURE); } else { maxheight = atof(argv[++optind]); } break;
    		default: printUsage(binary, stderr); return (EXIT_FAILURE);
        }   
    }

    if (argc <= 1) {
		printUsage(binary, stdout);
		printf("\n");
    }
    printf("{mode='%c'}, {seed='%ld'}, {maxheight='%f'}, {bmpfile='%s'}\n\n", mode, seed, maxheight, bmpfile);

	printf("Initializing openGL...\n");

	glhInit();

	printf("Creating gl context...\n");
    t_env env;

	env.context = glhCreateContext();

	t_glh_context * context	= env.context;

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

	t_world * world 		= &(env.world);
	t_renderer * renderer 	= &(env.renderer);
	t_camera * camera 		= &(env.camera);
    //thrd_t * thrd 			= &(env.thrd);

	printf("Initializing camera...\n");
	cameraInit(camera);

	printf("Initializing renderer...\n");
	rendererInit(renderer);

	printf("Initializing inputs...\n");
	inputInit(context);

	printf("Initializing world...\n");
	worldInit(world, bmpfile, maxheight);

	printf("Creating calculator thread...\n");
  	//thrd_create(thrd, threadLoop, &env);

	printf("Rendering started...\n");

	long int total = 0;
	long int count = 0;

	env.is_running = 1;

    while (!glhWindowShouldClose(context->window) && env.is_running) {

    	long t1;
    	MICROSEC(t1);

    	//update the window
    	glhWindowUpdate(context->window);

    	//input
    	inputUpdate(context, world, renderer, camera);
    	
    	//camera
 	    cameraUpdate(context, world, renderer, camera);

 	    //world
 	    worldUpdate(context, world, renderer, camera);

    	//update the renderer
    	rendererUpdate(context, world, renderer, camera);

    	//render
    	rendererRender(context, world, renderer, camera);
    	
    	//glhCheckError("post rendererUpdate()");

    	long t2;
    	MICROSEC(t2);

    	total += (t2 - t1);
    	count++;

    	//swap buffers
    	glhSwapBuffer(context->window);
    }

	env.is_running = 0;

    //wait for calculator thread to finish
    printf("Waiting for thread to finish...\n");
   // thrd_join(env.thrd, NULL);

	printf("Loop ended\n");

	printf("Deleting camera...\n");
	cameraDelete(camera);

	printf("Deleting world...\n");
	worldDelete(world);

	printf("Deleting renderer...\n");
	rendererDelete(renderer);

	printf("Destroying gl context..\n");
    glhDestroyContext(context);

    printf("Stopping openGL...\n");
    glhStop();

	printf("All done\n");

	printf("Moyenne: %ld\n", total / 1000 / count);

	return (0);
}