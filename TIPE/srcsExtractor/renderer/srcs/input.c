#include "renderer.h"

static void inputKey(GLFWwindow * winptr, int key, int scancode, int action, int mods) {
	//close
	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(winptr, 1);
	}
}

static void inputUpdateCamera(t_camera * camera) {

	float movespeed = camera->movespeed;
	static float rotspeed = 0.3f;

	t_glh_window * win = glhGetWindow();

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
}
