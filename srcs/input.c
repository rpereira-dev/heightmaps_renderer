#include "renderer.h"

static void inputKey(GLFWwindow * winptr, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE) glhWindowClose(glhGetContext()->window);
	(void)winptr;
	(void)key;
	(void)scancode;
	(void)action;
	(void)mods;
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

void inputInit(t_glh_context * context) {
	glfwSetKeyCallback(context->window->pointer, inputKey);
	glfwSetCursorPosCallback(context->window->pointer, inputCursorPos);
	glfwSetMouseButtonCallback(context->window->pointer, inputMouseButton);
}