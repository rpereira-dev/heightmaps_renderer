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

static void rendererGenerateBufferIndices(t_renderer * renderer) {

	long size = sizeof(unsigned short) * (TERRAIN_DETAIL - 1) * (TERRAIN_DETAIL - 1) * 6;
	unsigned short * indices = (unsigned short *) malloc(size);

	int x, z;
	int i00, i01, i11, i10;
	int i = 0;
	for (x = 0 ; x < TERRAIN_DETAIL - 1; x++) {
		for (z = 0 ; z < TERRAIN_DETAIL - 1; z++) {

			i00 = x * TERRAIN_DETAIL + z;
			i01 = i00 + 1;
			i10 = (x + 1) * TERRAIN_DETAIL + z;
			i11 = i10 + 1;
			indices[i++] = i00;
			indices[i++] = i11;
			indices[i++] = i10;
			indices[i++] = i00;
			indices[i++] = i01;
			indices[i++] = i11;
		}
	}

	renderer->terrain_indices = glhVBOGen();
	glhVBOBind(GL_ELEMENT_ARRAY_BUFFER, renderer->terrain_indices);
	glhVBOData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	glhVBOUnbind(GL_ELEMENT_ARRAY_BUFFER);

	free(indices);
}

static void rendererGenerateBufferVertices(t_renderer * renderer) {
	
	long size = sizeof(float) * TERRAIN_DETAIL * TERRAIN_DETAIL * 2;
	float * vertices = (float *) malloc(size);

	float unit = 1 / (float)(TERRAIN_DETAIL - 1);
	int x, z;
	int i = 0;
	for (x = 0 ; x < TERRAIN_DETAIL ; x++) {
		for (z = 0 ; z < TERRAIN_DETAIL ; z++) {
			vertices[i++] = x * unit;
			vertices[i++] = z * unit;
		}
	}

	renderer->terrain_vertices = glhVBOGen();
	glhVBOBind(GL_ARRAY_BUFFER, renderer->terrain_vertices);
	glhVBOData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	glhVBOUnbind(GL_ARRAY_BUFFER);

	free(vertices);
}

static void rendererGenerateBuffers(t_renderer * renderer) {
	rendererGenerateBufferIndices(renderer);
	rendererGenerateBufferVertices(renderer);
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

	//generate terrain indices
	rendererGenerateBuffers(renderer);

	//initialize lists
	renderer->render_list = array_list_new(256, sizeof(t_terrain *));
	renderer->delete_list = array_list_new(256, sizeof(t_terrain *));

	//enable depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);


	glhCheckError("post rendererInit()");
}

void rendererDelete(t_renderer * renderer) {
	glhProgramDelete(renderer->program);
	glhVBODelete(renderer->terrain_indices);
	glhVBODelete(renderer->terrain_vertices);
	array_list_delete(renderer->render_list);
	array_list_delete(renderer->delete_list);
	free(renderer->render_list);
	free(renderer->delete_list);
}

void rendererUpdate(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera) {
	(void)context;
	(void)world;
	(void)renderer;
	(void)camera;
}

void rendererRender(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera) {
	(void)context;

	//delete every terrains that has to
	ARRAY_LIST_ITER_START(renderer->delete_list, t_terrain **, terrain_ptr, i) {
		worldRemoveTerrain(world, *terrain_ptr);
	}
	ARRAY_LIST_ITER_END(renderer->delete_list, t_terrain *, terrain, i);

    //clear color buffer
    glhClearColor(119 / 255.0f, 181 / 255.0f, 254 / 255.0f, 1.0f);
    glhClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//bind the program
	glhProgramUse(renderer->program);

	//load uniforms
	glhProgramLoadUniformMatrix4f(u_mvp_matrix, (float*)&(camera->mviewproj));

	//debug
	if (glfwGetKey(context->window->pointer, GLFW_KEY_F) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//render every terrain that has to
	ARRAY_LIST_ITER_START(renderer->render_list, t_terrain **, terrain_ptr, i) {

		//get the terrain
		t_terrain * terrain = *terrain_ptr;

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
		glhDrawElements(GL_TRIANGLES, (TERRAIN_DETAIL - 1) * (TERRAIN_DETAIL - 1) * 6, GL_UNSIGNED_SHORT, NULL);
	}
	ARRAY_LIST_ITER_END(renderer->render_list, t_terrain *, terrain, i);

	glhVAOUnbind();
	glhProgramUse(NULL);
}
