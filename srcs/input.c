#include "renderer.h"

static void inputKey(GLFWwindow * winptr, int key, int scancode, int action, int mods) {

	(void)scancode;
	(void)mods;

	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(winptr, 1);
	} else if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		int mod = glfwGetInputMode(winptr, GLFW_CURSOR) == GLFW_CURSOR_DISABLED ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
		glfwSetInputMode(winptr, GLFW_CURSOR, mod);
	}

}

static void inputCursorPos(GLFWwindow * winptr, double xpos, double ypos) {
	(void)winptr;
	(void)xpos;
	(void)ypos;
}

static void inputMouseButton(GLFWwindow * winptr, int button, int action, int mods) {
	(void)winptr;
	(void)button;
	(void)action;
	(void)mods;
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
	printf("%*s: %lu\n", padding, "loaded terrains", world->terrains->size);
	printf("\n");

	printf("-------------------- RENDERER --------------------------\n");
	printf("\n");
	printf("%*s: %d\n", padding, "terrain program ID", renderer->program->id);
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

	static float movespeed = 4.0f;
	static float rotspeed = 1.0f;

	t_glh_window * win = glhGetWindow();

	if (glfwGetInputMode(win->pointer, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
		return ;
	}

	//rotation
	camera->rot.pitch += ((win->mouseY - win->prev_mouseY) * 0.3f * rotspeed);
	camera->rot.yaw += -((win->mouseX - win->prev_mouseX) * 0.3f * rotspeed);

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

	if (glfwGetKey(win->pointer, GLFW_KEY_A) == GLFW_PRESS) {
		camera->pos.x += -camera->vview.z * movespeed;
		camera->pos.z += camera->vview.x * movespeed;
	}
	else if (glfwGetKey(win->pointer, GLFW_KEY_D) == GLFW_PRESS) {
		camera->pos.x += camera->vview.z * movespeed;
		camera->pos.z += -camera->vview.x * movespeed;
	}
}

static void inputUpdateWorld(t_glh_window * win, t_world * world) {
	if (glfwGetKey(win->pointer, GLFW_KEY_P) == GLFW_PRESS) {
		noise2Seed(world->noise, time(NULL));
		HMAP_ITER_START(world->terrains, t_terrain *, terrain) {
			terrainGenerate(world, terrain);
		}
		HMAP_ITER_END(world->terrains, t_terrain *, terrain);
	}
}

void inputUpdate(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera) {
	inputUpdateCamera(camera);
	inputUpdateDebug(context, world, renderer, camera);
	inputUpdateWorld(context->window, world);
}

void inputInit(t_glh_context * context) {
	glfwSetInputMode(context->window->pointer, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(context->window->pointer, inputKey);
	glfwSetCursorPosCallback(context->window->pointer, inputCursorPos);
	glfwSetMouseButtonCallback(context->window->pointer, inputMouseButton);
}
