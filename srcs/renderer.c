#include "renderer.h"

GLuint u_mvp_matrix;
GLuint u_transf_matrix;

static void rendererBindAttributes(t_glh_program * program) {
	glhProgramBindAttribute(program, 0, "pos");
	glhProgramBindAttribute(program, 1, "color");
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

	glhCheckError("post rendererInit()");
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

    //clear color buffer
    glhClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//bind the program
	glhProgramUse(renderer->program);

	//load uniforms
	glhProgramLoadUniformMatrix4f(u_mvp_matrix, (float*)&(camera->mviewproj));

	//debug
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	HMAP_ITER_START(world->terrains, t_terrain *, terrain) {
		//generate the transformation matrix
		static t_mat4f mat;
		mat4f_identity(&mat);
		mat4f_translate(&mat, &mat, terrain->index.x * TERRAIN_SIZE, 0, terrain->index.y * TERRAIN_SIZE);
		mat4f_scale(&mat, &mat, TERRAIN_SIZE);

		//load the matrix as a uniform variable
		glhProgramLoadUniformMatrix4f(u_transf_matrix, (float*)(&mat));

		//bind the model
		glhVAOBind(terrain->vao);

		//draw it
		glhDraw(GL_TRIANGLES, 0, TERRAIN_VERTEX_COUNT);
	}
	HMAP_ITER_END(world->terrains, t_terrain *, terrain);

	glhVAOUnbind();
	glhProgramUse(NULL);
}
