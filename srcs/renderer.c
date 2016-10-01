#include "renderer.h"

GLuint u_mvp_matrix;
GLuint u_transf_matrix;

static void rendererBindAttributes(t_glh_program * program) {
	glhProgramBindAttribute(program, 0, "pos");
	//glhProgramBindAttribute(program, 1, "normal");
	//glhProgramBindAttribute(program, 2, "color");
}

static void rendererLinkUniforms(t_glh_program * program) {
	u_mvp_matrix = glhProgramGetUniform(program, "mvp_matrix");
	u_transf_matrix = glhProgramGetUniform(program, "transf_matrix");
}

void rendererInit(t_renderer * renderer) {

	//create the program
	renderer->program = glhProgramNew();

	//load shaders
	GLuint fs = glhShaderLoad("./shaders/model.fs", GL_FRAGMENT_SHADER);
	GLuint vs = glhShaderLoad("./shaders/model.vs", GL_VERTEX_SHADER);
	glhProgramAddShader(renderer->program, fs, GLH_SHADER_FRAGMENT);
	glhProgramAddShader(renderer->program, vs, GLH_SHADER_VERTEX);

	//link
	glhProgramLink(renderer->program, rendererBindAttributes, rendererLinkUniforms);

	//enable depth test
	glEnable(GL_DEPTH_TEST);
}

void rendererDelete(t_renderer * renderer) {
	glhProgramDelete(renderer->program);
}

void rendererUpdate(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera) {
	(void)context;
	(void)world;
	(void)renderer;
	(void)camera;
}

void rendererRender(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera) {
	(void)context;
	(void)world;
	(void)renderer;
	(void)camera;

	//bind the program
	glhProgramBind(renderer->program);

	//load uniforms
	glhProgramLoadUniformMatrix4f(program, u_mvp_matrix, camera->mvp_matrix);
	
	//for each terrains
	
	glhProgramLoadUniformMatrix4f(program, u_transf_matrix, camera->mvp_matrix);

	glhProgramUnbind();
}
