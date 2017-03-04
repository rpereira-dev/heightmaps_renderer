#include "glh.h"

int _glh_debug = 0;
t_glh_context * _glh_context;

void errorCallback(int error, const char* description) {
    fprintf(stderr, "GL error: %s (%d)\n", description, error);
}

/** called to init opengl */
int glhInit() {
	if (!glfwInit()) {
		return (0);
	}

	glfwSetErrorCallback(errorCallback);
	return (1);
}

/** stop opengl */
void glhStop() {
	if (_glh_context != NULL) {
		glhDestroyContext(_glh_context);
	}
	glfwTerminate();
}

/** create a new glh context */
t_glh_context * glhCreateContext(void) {
	t_glh_context * context = (t_glh_context*)malloc(sizeof(t_glh_context));
	if (context == NULL) {
		return (NULL);
	}
	context->window = glhWindowCreate();
	return (context);
}

void glhDestroyContext(t_glh_context * context) {
	if (_glh_context == context) {
		_glh_context = NULL;
	}

	glhWindowDestroy(context->window);
	context->window = NULL;
	free(context);
}

/** set the opengl context to the given window */
void glhMakeContextCurrent(t_glh_context * context) {
	// set current context
	glfwMakeContextCurrent(context->window->pointer);

	// singleton update
	_glh_context = context;

    //initialize glew if needed
    #ifdef _WIN32
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			fprintf(stderr, "glew err: %s\n", glewGetErrorString(err));
		}
    #endif
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

#ifndef GL_STACK_UNDERFLOW
# define GL_STACK_UNDERFLOW (0)
#endif

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

/** window related functions */

/** create and return a new gl window */
t_glh_window * glhWindowCreate() {
	static char * DEFAULT_WINDOW_TITLE = "Default Title";
	static int DEFAULT_WINDOW_WIDTH = 1100;
	static int DEFAULT_WINDOW_HEIGHT = 1100 / 1.6f;

	#ifdef __APPLE__
		glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  		glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
  		glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  		glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#endif

	void * pointer = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_TITLE, NULL, NULL);
	if (pointer == NULL) {
		return (NULL);
	}

	t_glh_window * window = (t_glh_window *)malloc(sizeof(t_glh_window));
	if (window == NULL) {
		return (NULL);
	}

	window->pointer = pointer;
	window->width = DEFAULT_WINDOW_WIDTH;
	window->height = DEFAULT_WINDOW_HEIGHT;

	return (window);
}

int glhWindowShouldClose(t_glh_window * window) {
	return (glfwWindowShouldClose(window->pointer));
}

void glhWindowClose(t_glh_window * window) {
	glfwSetWindowShouldClose(window->pointer, 1);
}

void glhViewPort(int x, int y, int width, int height) {
	glViewport(x, y, width, height);
}

/** destroy a window */
void glhWindowDestroy(t_glh_window * window) {
	glfwDestroyWindow(window->pointer);
}

/** set window title */
void glhWindowSetTitle(t_glh_window * window, char * title) {
	glfwSetWindowTitle(window->pointer, title);
}

void glhWindowSetSize(t_glh_window * window, int width, int height) {
	glfwSetWindowSize(window->pointer, width, height);
}

void glhWindowUpdate(t_glh_window * window) {
	window->prev_mouseX = window->mouseX;
	window->prev_mouseY = window->mouseY;
	glfwGetCursorPos(window->pointer, &(window->mouseX), &(window->mouseY));
	glfwGetWindowSize(window->pointer, &(window->width), &(window->height));
	glfwPollEvents();
}

/** swap buffers */
void glhSwapBuffer(t_glh_window * window) {
	glfwSwapBuffers(window->pointer);
	window->frames_swapped++;
}

/** clear the buffers */
void glhClear(int bufferbits) {
	glClear(bufferbits);
}

void glhClearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

/** program functions */

/** create a new program */
t_glh_program * glhProgramNew(void) {
	t_glh_program * program = (t_glh_program*)malloc(sizeof(t_glh_program));
	if (program == NULL) {
		return (NULL);
	}
	memset(program, 0, sizeof(t_glh_program));
	return (program);
}

/** add a shader to the program */
int glhProgramAddShader(t_glh_program * program, GLuint shaderID, int shaderType) {
	if (shaderType < 0 || shaderType >= 4) {
		return (0);
	}
	program->shaders[shaderType] = shaderID;
	return (1);
}

/** link the program */
void glhProgramLink(t_glh_program * program, void (*fbindAttributes)(t_glh_program *), void (*fLinkUniforms)(t_glh_program *)) {

	//create a new program
	program->id = glCreateProgram();

	//for each shaders, attach it
	int i;
	for (i = 0 ; i < 4 ; i++) {
		if (program->shaders[i]) {
			glAttachShader(program->id, program->shaders[i]);
		}
	}

	//bind attributes to shaders
	fbindAttributes(program);

	//link the program
	glLinkProgram(program->id);

	{
		char message[512];
		int length;
		glGetProgramInfoLog(program->id, sizeof(message), &length, message);
		if (length > 0) {
			printf("Linking shader message: %s\n", message);
		}
	}

	//valide program
	glValidateProgram(program->id);

	//link uniforms
	fLinkUniforms(program);
}

/** delete a program */
void glhProgramDelete(t_glh_program * program) {
	glDeleteProgram(program->id);
	free(program);
}

void glhProgramUse(t_glh_program * program) {
	glUseProgram(program == NULL ? 0 : program->id);
}

void glhProgramBindAttribute(t_glh_program * program, int attribute, char * name) {
	glBindAttribLocation(program->id, attribute, name);
}

void glhProgramLoadUniformInt(int location, int value) {
	glUniform1i(location, value);
}

void glhProgramLoadUniformFloat(int location, float value) {
	glUniform1f(location, value);
}

void glhProgramLoadUniformVec2f(int location, float x, float y) {
	glUniform2f(location, x, y);
}

void glhProgramLoadUniformVec3f(int location, float x, float y, float z) {
	glUniform3f(location, x, y, z);
}

void glhProgramLoadUniformVec4f(int location, float x, float y, float z, float w) {
	glUniform4f(location, x, y, z, w);
}

void glhProgramLoadUniformMatrix4f(int location, float * mat4) {
	glUniformMatrix4fv(location, 1, GL_FALSE, mat4);
}

int glhProgramGetUniform(t_glh_program * program, char * name) {
	return (glGetUniformLocation(program->id, name));
}

// shaders
void glhShaderDelete(GLuint shaderID) {
	glDeleteShader(shaderID);
}

GLuint glhShaderLoad(char * filepath, GLenum type) {	

	//read file to string
	int fd = open(filepath, O_RDONLY);
	if (fd < 0) {
		puts("a");
		return (-1);
	}

	static int buffsize = 4096;
	char * source = (char*)malloc(sizeof(char) * buffsize);
	if (source == NULL) {
		puts("b");
		return (-1);
	}

	int capacity = buffsize;
	int length = 0;
	int r;
	while ((r = read(fd, source + length, buffsize)) > 0) {
		length += r;
		capacity += buffsize;
		source = (char*)realloc(source, capacity);
	}

	if (length == capacity) {
		source = (char*)realloc(source, length + 1);
	}
	source[length] = 0;

	close(fd);

	//create shader
	GLuint shaderID = glCreateShader(type);
	//add source
	glShaderSource(shaderID, 1, (char const **)&source, &length);
	//free the source file
	free(source);

	//compile
	glCompileShader(shaderID);

	//check compilation
	GLint compiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE) {
		GLint length = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);

		char buffer[length];
		glGetShaderInfoLog(shaderID, length, &length, buffer);

		glhShaderDelete(shaderID);

		printf("shader compilation error: %s\n%s\n", filepath, buffer);
		return (-1);
	}
	return (shaderID);
}		


// vao and vbo bindings
GLuint glhVAOGen(void) {
	GLuint dst;
	glGenVertexArrays(1, &dst);
	return (dst);
}

GLuint glhVBOGen(void) {
	GLuint dst;
	glGenBuffers(1, &dst);
	return (dst);
}

void glhVAODelete(GLuint vao) {
	glDeleteVertexArrays(1, &vao);
}

void glhVBODelete(GLuint vbo) {
	glDeleteBuffers(1, &vbo);
}

void glhVBOData(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage) {
	glBufferData(target, size, data, usage);
}

void glhVAOBind(GLuint vao) {
	glBindVertexArray(vao);
}

void glhVAOUnbind(void) {
	glhVAOBind(0);
}

void glhVAOSetAttribute(GLuint attributeID, GLint length, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * offset) {
	glVertexAttribPointer(attributeID, length, type, normalized, stride, offset);
}

void glhVAOEnableAttribute(GLuint id) {
	glEnableVertexAttribArray(id);
}

void glhVBOBind(GLuint target, GLuint vbo) {
	glBindBuffer(target, vbo);
}

void glhVBOUnbind(GLuint target) {
	glhVBOBind(target, 0);
}

void glhDraw(int dst, int begin, int vertex_count) {
	glDrawArrays(dst, begin, vertex_count);
}

void glhDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices) {
	glDrawElements(mode, count, type, indices);
}