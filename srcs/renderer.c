#include "renderer.h"

GLuint u_mvp_matrix;
GLuint u_transf_matrix;
GLuint u_sky_color;
GLuint u_state;
GLuint u_time;

static void rendererBindAttributes(t_glh_program * program) {
	glhProgramBindAttribute(program, 0, "pos");
	glhProgramBindAttribute(program, 1, "uv");
	glhProgramBindAttribute(program, 2, "height");
	glhProgramBindAttribute(program, 3, "normal");
	glhProgramBindAttribute(program, 4, "textureID");
}

static void rendererLinkUniforms(t_glh_program * program) {
	u_mvp_matrix = glhProgramGetUniform(program, "mvp_matrix");
	u_transf_matrix = glhProgramGetUniform(program, "transf_matrix");
	u_sky_color = glhProgramGetUniform(program, "sky_color");
	u_state = glhProgramGetUniform(program, "state");
	u_time = glhProgramGetUniform(program, "time");
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
	
	long size = sizeof(float) * 4 * TERRAIN_DETAIL * TERRAIN_DETAIL;
	float * vertices = (float *) malloc(size);

	float unit = 1 / (float)(TERRAIN_DETAIL - 1);
	int x, z;
	int i = 0;
	for (x = 0 ; x < TERRAIN_DETAIL ; x++) {
		for (z = 0 ; z < TERRAIN_DETAIL ; z++) {
			vertices[i++] = x * unit;
			vertices[i++] = z * unit;
			vertices[i++] = z / (float)(TERRAIN_DETAIL - 1) / (float)TX_MAX;
			vertices[i++] = x / (float)(TERRAIN_DETAIL - 1);
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

	renderer->state = 0;

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

	//image
	renderer->texture.txID = glhGenTexture();
	renderer->texture.image = imageNew("./res/textures.bmp");
	unsigned char * pixels = (unsigned char*)(renderer->texture.image + 1);
	glBindTexture(GL_TEXTURE_2D, renderer->texture.txID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, renderer->texture.image->w, renderer->texture.image->h, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);

	//enable depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);


	glhCheckError("post rendererInit()");
}

static void rendererDeinitTerrain(t_renderer * renderer, t_terrain * terrain) {
	(void)renderer;

	if (terrain->initialized) {
		terrain->initialized = 0;
		glhVAODelete(terrain->vao);
		glhVBODelete(terrain->vbo);
	}
	
	if (terrain->vertices != NULL) {
		free(terrain->vertices);
		terrain->vertices = NULL;
	}
	free(terrain);
}

static void rendererInitTerrain(t_renderer * renderer, t_terrain * terrain) {

	(void)renderer;

	terrain->initialized = 1;

	//allocate terrain model on GPU
	terrain->vao = glhVAOGen();
	terrain->vbo = glhVBOGen();

	//bind vao
	glhVAOBind(terrain->vao);

	//bind indices
	glhVBOBind(GL_ELEMENT_ARRAY_BUFFER, renderer->terrain_indices);

	//bind static grid
	glhVBOBind(GL_ARRAY_BUFFER, renderer->terrain_vertices);
	glhVAOSetAttribute(0, 2, GL_FLOAT, 0, 4 * sizeof(float), NULL); //default vertices pos
	glhVAOSetAttribute(1, 2, GL_FLOAT, 0, 4 * sizeof(float), (void*)(2 * sizeof(float))); //default vertices uv
	glhVBOUnbind(GL_ARRAY_BUFFER);
	glhVAOEnableAttribute(0);
	glhVAOEnableAttribute(1);

	//bind buffer
	glhVBOBind(GL_ARRAY_BUFFER, terrain->vbo);
	//set attruibutes
	glhVAOSetAttribute(2, 1, GL_FLOAT, 0, TERRAIN_VERTEX_SIZE, NULL); //height
	glhVAOSetAttribute(3, 3, GL_FLOAT, 0, TERRAIN_VERTEX_SIZE, (void*)(1 * sizeof(float))); //normal
	glhVAOSetAttributeI(4, 3, GL_INT, TERRAIN_VERTEX_SIZE, (void*)((3 + 1) * sizeof(float))); //texture ID
	glhVBOUnbind(GL_ARRAY_BUFFER);
	//enable attributes
	glhVAOEnableAttribute(2);
	glhVAOEnableAttribute(3);
	glhVAOEnableAttribute(4);

	//unbind vao
	glhVAOUnbind();
}

static void rendererUpdateLists(t_world * world, t_renderer * renderer, t_camera * camera) {

	(void)camera;

	//clear list
	array_list_clear(renderer->render_list);
	array_list_clear(renderer->delete_list);

	//update listst
	HMAP_ITER_START(world->terrains, t_terrain *, terrain) {
		
		t_vec3f diff;
		diff.x = terrain->index.x - camera->terrain_index.x;
		diff.y = 0;
		diff.z = terrain->index.y - camera->terrain_index.y;

			//if to far, delete this terrain
		if (diff.x > TERRAIN_KEEP_LOADED_DISTANCE || diff.z > TERRAIN_KEEP_LOADED_DISTANCE) {
			array_list_add(renderer->delete_list, &terrain);
		} else {

			float distance = vec3f_length(&diff);
			float normalizer = 1 / distance;
			diff.x *= normalizer;
			diff.z *= normalizer;
			if (distance < TERRAIN_RENDER_DISTANCE) {
				//float dot = vec3f_dot_product(&(camera->vview), &diff);
				//float angle = acos_f(dot);
				//if (distance <= 2 || angle < camera->fov + 0.01f) {
					array_list_add(renderer->render_list, &terrain);
			//	}
			}
		}
	}
	HMAP_ITER_END(world->terrains, t_terrain *, terrain);


	//remove terrains
	ARRAY_LIST_ITER_START(renderer->delete_list, t_terrain **, terrain_ptr, i) {
		t_terrain * terrain = *terrain_ptr;
		hmap_remove_key(world->terrains, &(terrain->index));
		rendererDeinitTerrain(renderer, terrain);
	}
	ARRAY_LIST_ITER_END(renderer->delete_list, t_terrain **, terrain_ptr, i);

	array_list_clear(renderer->delete_list);
}

void rendererUpdate(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera) {
	(void)context;
	rendererUpdateLists(world, renderer, camera);
}

void rendererRender(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera) {
	(void)context;
	(void)world;

	//viewport
	glhViewPort(0, 0, context->window->width, context->window->height);

	//set the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderer->texture.txID);

    //clear color buffer
    glhClearColor(0.46f, 0.70f, 0.99f, 1.0f);
    glhClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//bind the program
	glhProgramUse(renderer->program);

	//load uniforms
	glhProgramLoadUniformMatrix4f(u_mvp_matrix, (float*)&(camera->mviewproj));

	//weather
	glhProgramLoadUniformVec3f(u_sky_color, 0.46f, 0.70f, 0.99f);

	//load state
	glhProgramLoadUniformInt(u_state, renderer->state);
	glhProgramLoadUniformInt(u_time, world->time);

	//debug
	if (glfwGetKey(context->window->pointer, GLFW_KEY_F) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	int vertexCount = (TERRAIN_DETAIL - 1) * (TERRAIN_DETAIL - 1) * 6;

	//render every terrain that has to
	ARRAY_LIST_ITER_START(renderer->render_list, t_terrain **, terrain_ptr, i) {

		//get the terrain
		t_terrain * terrain = *terrain_ptr;

		//check terrain vertices
		if (!terrain->initialized) {
			rendererInitTerrain(renderer, terrain);
		}

		if (terrain->vertices != NULL) {
			glhVBOBind(GL_ARRAY_BUFFER, terrain->vbo);
			//set vertices
			glhVBOData(GL_ARRAY_BUFFER, TERRAIN_DETAIL * TERRAIN_DETAIL * TERRAIN_VERTEX_SIZE, terrain->vertices, GL_STATIC_DRAW);
			//release data
			free(terrain->vertices);
			terrain->vertices = NULL;
			glhVBOUnbind(GL_ARRAY_BUFFER);
		}

		//generate the transformation matrix
		t_mat4f mat;
		mat4f_identity(&mat);
		mat4f_translate(&mat, &mat, terrain->index.x * TERRAIN_SIZE, 0, terrain->index.y * TERRAIN_SIZE);
		mat4f_scale(&mat, &mat, TERRAIN_SIZE);

		//load the matrix as a uniform variable
		glhProgramLoadUniformMatrix4f(u_transf_matrix, (float*)(&mat));

		//bind the model
		glhVAOBind(terrain->vao);

		//draw it
		glhDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_SHORT, NULL);
	}
	ARRAY_LIST_ITER_END(renderer->render_list, t_terrain *, terrain, i);

	glhVAOUnbind();
	glhProgramUse(NULL);
}


void rendererDelete(t_renderer * renderer) {
	glhProgramDelete(renderer->program);
	glhVBODelete(renderer->terrain_indices);
	glhVBODelete(renderer->terrain_vertices);
	array_list_delete(renderer->render_list);
	array_list_delete(renderer->delete_list);
	free(renderer->render_list);
	imageDelete(renderer->texture.image);
	glhDeleteTexture(renderer->texture.txID);
}
