#ifndef GLH_H
# define GLH_H

# ifdef __APPLE__
# 	define GLFW_INCLUDE_GLCOREARB
# endif

# ifdef _WIN32
# 	include <GL/glew.h>
# endif

# include "GLFW/glfw3.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct 	s_glh_window {
	void	* pointer;
	int		width;
	int		height;
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
	int id;
	int shaders[GLH_SHADER_MAX_ID];
}				t_glh_program;

extern int _glh_debug;
extern t_glh_context * _glh_context;

/** called to init opengl */
int glhInit();

/** stop opengl */
void glhStop();

/** create a new glh context */
t_glh_context * glhCreateContext(void);

/** set the opengl context to the given window */
void glhMakeContextCurrent(t_glh_context * context);

/** window should close */
int glhWindowShouldClose(t_glh_window * window);

void glhDestroyWindow(t_glh_window * window);

/** gl clear */
void glhClear(int bufferbits);

void glhPollEvents();

/** swap buffers */
void glhSwapBuffer(t_glh_window * window);

/** get the last set context */
t_glh_context * glhGetContext();

t_glh_window * glhGetWindow();

char * glhGetErrorString(int err);

/** call it to check openGL error after a gl call */
void glhCheckError(char * label);

/** create and return a new gl window */
t_glh_window * glhCreateWindow();

void glhDestroyContext(t_glh_context * context);

#endif