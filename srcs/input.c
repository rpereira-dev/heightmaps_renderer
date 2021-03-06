#include "renderer.h"

static void inputKey(GLFWwindow * winptr, int key, int scancode, int action, int mods) {
	t_world * world = &(getEnv()->world);
	t_renderer * renderer = &(getEnv()->renderer);
	t_camera * camera = &(getEnv()->camera);

		//reset terrain
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		
		int i;
		long long unsigned int seed = time(NULL);
		for (i = 0 ; i < WORLD_OCTAVES ; i++) {
			noiseNextInt(&seed);
			noiseSeed(world->octaves[i], seed);
		}

		HMAP_ITER_START(world->terrains, t_terrain *, terrain) {
			terrainGenerate(world, terrain);
		}
		HMAP_ITER_END(world->terrains, t_terrain *, terrain);
	}


	//triangles
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		renderer->state ^= STATE_RENDER_TRIANGLES;
	}

	//fog
	if (key == GLFW_KEY_V && action == GLFW_PRESS) {
		renderer->state ^= STATE_APPLY_FOG;
	}

	//lighting
	if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		renderer->state ^= STATE_APPLY_PHONG_LIGHTNING;
	}

	//lighting
	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		renderer->state ^= STATE_SPECULAR;
	}

	//culling
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		renderer->state ^= STATE_LOCK_CULLING;
	}

	//culling
	if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		renderer->state ^= STATE_CULLING;
	}

	//sun
	if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		renderer->sunray.x = camera->vview.x;
		renderer->sunray.y = -camera->vview.y;
		renderer->sunray.z = camera->vview.z;
	}

	//close
	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(winptr, 1);
	} else if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		int mod = glfwGetInputMode(winptr, GLFW_CURSOR) == GLFW_CURSOR_DISABLED ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
		glfwSetInputMode(winptr, GLFW_CURSOR, mod);
	}

}

static void inputCursorPos(GLFWwindow * winptr, double xpos, double ypos) {
}

static void inputMouseButton(GLFWwindow * winptr, int button, int action, int mods) {
}

static void inputUpdateDebug(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera) {

	static int padding = 20;
	static char * str;

    //debug
	if (glfwGetKey(context->window->pointer, GLFW_KEY_H) != GLFW_PRESS) {
		return ;
	}

	printf("\n");

	printf("********************************************************\n");
	printf("***************** DEBUG STARTS *************************\n");
	printf("********************************************************\n");

	long time;
	MICROSEC(time);	
	printf("\nCurrent timestamp: %ld\n\n", time);

	printf("\n");
	printf("-----------------  GLH CONTEXT -------------------------\n");
	printf("\n");

	printf("%*s: %p\n", padding, "window pointer", context->window->pointer);
	printf("%*s: %d\n", padding, "window width", context->window->width);
	printf("%*s: %d\n", padding, "window height", context->window->height);
	printf("%*s: %.4f\n", padding, "mouse X", context->window->mouseX);
	printf("%*s: %.4f\n", padding, "mouse Y", context->window->mouseY);
	printf("%*s: %.4f\n", padding, "prev mouse X", context->window->prev_mouseX);
	printf("%*s: %.4f\n", padding, "prev mouse Y", context->window->prev_mouseY);
	printf("%*s: %d\n", padding, "frames swapped", context->window->frames_swapped);


	printf("\n");
	printf("--------------------- WORLD ----------------------------\n");
	printf("\n");
	printf("%*s: %lu/%d\n", padding, "loaded terrains", world->terrains->size, TERRAIN_KEEP_LOADED_DISTANCE * TERRAIN_KEEP_LOADED_DISTANCE * 4);
	printf("\n");

	printf("-------------------- RENDERER --------------------------\n");
	printf("\n");
	printf("%*s: %d\n", padding, "terrain program ID", renderer->program->id);
	printf("%*s: %d\n", padding, "triangle drawn on last frame", renderer->vertexCount / 3);
	printf("%*s: %d\n", padding, "terrains drawn on last frame", renderer->vertexCount / (6 * (TERRAIN_DETAIL - 1) * (TERRAIN_DETAIL - 1)));
	printf("\n");


	printf("-------------------- CAMERA ----------------------------\n");
	printf("\n");
	printf("position: vec3f(x:%.2f ; y:%.2f ; z:%.2f)\n", camera->pos.x, camera->pos.y, camera->pos.z);
	printf("index   : vec2i(x:%d ; y:%d)\n", camera->terrain_index.x, camera->terrain_index.y);
	printf("rotation: vec3f(x:%.2f ; y:%.2f ; z:%.2f)\n", camera->rot.x, camera->rot.y, camera->rot.z);
	printf("viewvec : vec3f(x:%.2f ; y:%.2f ; z:%.2f)\n", camera->vview.x, camera->vview.y, camera->vview.z);
	printf("\n");

	str = mat4f_str(&(camera->mview));
	printf("view matrix:\n%s\n\n", str);
	free(str);

	str = mat4f_str(&(camera->mproj));
	printf("projection matrix:\n%s\n\n", str);
	free(str);

	str = mat4f_str(&(camera->mviewproj));
	printf("viewproj matrix:\n%s\n\n", str);
	free(str);

	printf("\n");

	printf("********************************************************\n");
	printf("*****************  DEBUG ENDS  *************************\n");
	printf("********************************************************\n");
}

static void inputUpdateCamera(t_camera * camera) {

	float movespeed = camera->movespeed;
	static float rotspeed = 0.3f;

	t_glh_window * win = glhGetWindow();

	if (glfwGetInputMode(win->pointer, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
		return ;
	}


	//camera speed
	if (glfwGetKey(win->pointer, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		camera->movespeed *= 1.2f;
	} else if (glfwGetKey(win->pointer, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
		camera->movespeed *= 0.833f;
	}

	//rotation
	camera->rot.pitch += ((win->mouseY - win->prev_mouseY) * rotspeed);
	camera->rot.yaw += ((win->mouseX - win->prev_mouseX) * rotspeed);

	//move
	if (glfwGetKey(win->pointer, GLFW_KEY_W) == GLFW_PRESS) {
		camera->pos.x += camera->vview.x * movespeed;
		camera->pos.y += camera->vview.y * movespeed;
		camera->pos.z += camera->vview.z * movespeed;
	} else if (glfwGetKey(win->pointer, GLFW_KEY_S) == GLFW_PRESS) {
		camera->pos.x += -camera->vview.x * movespeed;
		camera->pos.y += -camera->vview.y * movespeed;
		camera->pos.z += -camera->vview.z * movespeed;
	}

	if (glfwGetKey(win->pointer, GLFW_KEY_D) == GLFW_PRESS) {
		camera->pos.x += -camera->vview.z * movespeed;
		camera->pos.z += camera->vview.x * movespeed;
	}
	else if (glfwGetKey(win->pointer, GLFW_KEY_A) == GLFW_PRESS) {
		camera->pos.x += camera->vview.z * movespeed;
		camera->pos.z += -camera->vview.x * movespeed;
	}
}

void inputUpdate(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera) {
	inputUpdateCamera(camera);
	inputUpdateDebug(context, world, renderer, camera);
}

void inputInit(t_glh_context * context) {
	glfwSetInputMode(context->window->pointer, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(context->window->pointer, inputKey);
	glfwSetCursorPosCallback(context->window->pointer, inputCursorPos);
	glfwSetMouseButtonCallback(context->window->pointer, inputMouseButton);
}
