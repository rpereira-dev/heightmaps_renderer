#ifndef GLH_H
# define GLH_H

# ifdef __APPLE__
# 	define GLFW_INCLUDE_GLCOREARB
# endif

# if defined(_WIN32) |	defined(_WIN64)
# 	include <GL/glew.h>
# endif

# include "GLFW/glfw3.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>

# define GLH_WINDOW_EVENT_SCROLL (0)
# define GLH_WINDOW_EVENT_MOUSE_CURSOR (1)
# define GLH_WINDOW_EVENT_MOUSE_BUTTON (2)
# define GLH_WINDOW_EVENT_RESIZE (3)
# define GLH_WINDOW_EVENT_FOCUS (4)

typedef struct 	s_glh_window {
	void			* pointer;
	int				width;
	int				height;
	double 			mouseX; //current mouse coordinate X
	double 			mouseY; //current mouse coordinate Y
	double 			prev_mouseX; //previous mouse coordinate X
	double 			prev_mouseY; //previous mouse coordinate Y
	int 			frames_swapped; //number of swap buffer calls (i.e, frame put on screen)
}				t_glh_window;

typedef struct	s_glh_context {
	t_glh_window * window;
}				t_glh_context;

# define GLH_SHADER_VERTEX (0)
# define GLH_SHADER_GEOMETRY (1)
# define GLH_SHADER_FRAGMENT (2)
# define GLH_SHADER_COMPUTE (3)
# define GLH_SHADER_MAX_ID (4)

typedef struct 	s_glh_program {
	GLuint id;
	GLuint shaders[GLH_SHADER_MAX_ID];
}				t_glh_program;

/** called to init opengl */
int glhInit();

/** stop opengl */
void glhStop();

/** create a new glh context */
t_glh_context * glhCreateContext(void);

void glhDestroyContext(t_glh_context * context);

/** set the opengl context to the given window */
void glhMakeContextCurrent(t_glh_context * context);

	/** get the last set context */
t_glh_context * glhGetContext();
t_glh_window * glhGetWindow();
char * glhGetErrorString(int err);

/** call it to check openGL error after a gl call */
void glhCheckError(char * label);

/** window related functions */

/** create and return a new gl window */
t_glh_window * glhWindowCreate();
int glhWindowShouldClose(t_glh_window * window);
void glhWindowClose(t_glh_window * window);
void glhWindowDestroy(t_glh_window * window);
void glhWindowSetTitle(t_glh_window * window, char * title);
void glhWindowSetSize(t_glh_window * window, int width, int height);
void glhWindowUpdate(t_glh_window * window);
void glhViewPort(int x, int y, int width, int height);

/** swap buffers */
void glhSwapBuffer(t_glh_window * window);

/** clear the buffers */
void glhClear(int bufferbits);
void glhClearColor(float r, float g, float b, float a);

//textures
GLuint glhGenTexture(void);
void glhDeleteTexture(GLuint txID);

/** program functions */
t_glh_program *	glhProgramNew(void);
int 			glhProgramAddShader(t_glh_program * program, GLuint shaderID, int shaderType);
void 			glhProgramLink(t_glh_program * program, void (*fbindAttributes)(t_glh_program *), void (*fLinkUniforms)(t_glh_program *));
void 			glhProgramDelete(t_glh_program * program);
void			glhProgramUse(t_glh_program * program);
void			glhProgramBindAttribute(t_glh_program * program, int attribute, char * name);
void			glhProgramLoadUniformInt(int location, int value);
void 			glhProgramLoadUniformFloat(int location, float value);
void 			glhProgramLoadUniformVec2f(int location, float x, float y);
void 			glhProgramLoadUniformVec3f(int location, float x, float y, float z);
void 			glhProgramLoadUniformVec4f(int location, float x, float y, float z, float w);
void 			glhProgramLoadUniformMatrix4f(int location, float * mat4);
int 			glhProgramGetUniform(t_glh_program * program, char * name);

void 	glhShaderDelete(GLuint shaderID);
GLuint 	glhShaderLoad(char * filepath, GLenum type);

//vao / vbo
GLuint glhVAOGen(void);
GLuint glhVBOGen(void);

void glhVAODelete(GLuint vao);
void glhVBODelete(GLuint vbo);

void glhVAOBind(GLuint vao);
void glhVAOUnbind(void);

void glhVBOData(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);

void glhVAOSetAttribute(GLuint attributeID, GLint length, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * offset);
void glhVAOSetAttributeI(GLuint attributeID, GLint length, GLenum type, GLsizei stride, const GLvoid * offset);
void glhVAOEnableAttribute(GLuint id);

void glhVBOBind(GLuint target, GLuint vbo);
void glhVBOUnbind(GLenum target);

void glhDraw(int dst, int begin, int vertex_count);
void glhDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices);

#endif