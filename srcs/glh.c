#include "glh.h"

int _glh_debug = 0;
t_glh_context * _glh_context;

/** called to init opengl */
int glhInit() {
	if (!glfwInit()) {
		return (0);
	}
	return (1);
}

/** stop opengl */
void glhStop() {
	glfwTerminate();
}

/** create a new glh context */
t_glh_context * glhCreateContext(void) {
	t_glh_context * context = (t_glh_context*)malloc(sizeof(t_glh_context));
	if (context == NULL) {
		return (NULL);
	}
	context->window = glhCreateWindow();
	return (context);
}

void glhDestroyWindow(t_glh_window * window) {
	glfwDestroyWindow(window->pointer);
}

void glhDestroyContext(t_glh_context * context) {
	if (_glh_context == context) {
		_glh_context = NULL;
	}

	glhDestroyWindow(context->window);
	context->window = NULL;
	free(context);
}

/** set the opengl context to the given window */
void glhMakeContextCurrent(t_glh_context * context) {
	// set current context
	glfwMakeContextCurrent(context->window->pointer);

	// singleton update
	_glh_context = context;
}

void glhPollEvents() {
	glfwPollEvents();
}

	/** get the last set context */
t_glh_context * glhGetContext() {
	return (_glh_context);
}

t_glh_window * glhGetWindow() {
	if (glhGetContext() == NULL) {
		return (NULL);
	}
	return (glhGetContext()->window);
}

char * glhGetErrorString(int err) {
	static char * str[] = { "GL_INVALID_ENUM", "GL_INVALID_VALUE", "GL_INVALID_OPERATION", "GL_STACK_OVERFLOW",
			"GL_STACK_UNDERFLOW", "GL_OUT_OF_MEMORY" };
	int errs[] = { GL_INVALID_ENUM, GL_INVALID_VALUE, GL_INVALID_OPERATION, GL_STACK_OVERFLOW,
			GL_STACK_UNDERFLOW, GL_OUT_OF_MEMORY, };

	if (err != GL_NO_ERROR) {
		for (int i = 0; i < 6; i++) {
			if (errs[i] == err) {
				return (str[i]);
			}
		}
	}
	return (NULL);
}

/** call it to check openGL error after a gl call */
void glhCheckError(char * label) {
	int err = glGetError();

	char * str = glhGetErrorString(err);
	if (str == NULL) {
		return;
	}
	printf("%s : GLH ERROR CHECK : %s\n", label, str);
}

/** create and return a new gl window */
t_glh_window * glhCreateWindow() {
	static char * DEFAULT_WINDOW_TITLE = "Default Title";
	static int DEFAULT_WINDOW_WIDTH = 1100;
	static int DEFAULT_WINDOW_HEIGHT = 1100 / 1.6f;

	t_glh_window * window = (t_glh_window*)malloc(sizeof(t_glh_window));
	if (window == NULL) {
		return (NULL);
	}

	window->pointer = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_TITLE, NULL, NULL);
	window->width = DEFAULT_WINDOW_WIDTH;
	window->height = DEFAULT_WINDOW_HEIGHT;

	return (window);
}

int glhWindowShouldClose(t_glh_window * window) {
	return (glfwWindowShouldClose(window->pointer));
}

void glhSwapBuffer(t_glh_window * window) {
	glfwSwapBuffers(window->pointer);
}

void glhClear(int bufferbits) {
	glClear(bufferbits);
}

/** compile issues....*/
void _wassert() {
	//printf("_wassert called\n");
}

void __ms_vsnprintf() {
	//printf("__ms_vsnprintf called\n");
}
