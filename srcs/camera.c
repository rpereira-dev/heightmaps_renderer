#include "renderer.h"

void cameraInit(t_camera * camera) {

	camera->pos.x = 0, camera->pos.y = 0, camera->pos.z = 0;
	camera->rot.pitch = 0, camera->rot.yaw = 0, camera->rot.roll = 0;
	camera->fov = 70.0f;
	camera->near_distance = 0.01f;
	camera->far_distance = TERRAIN_RENDER_DISTANCE * TERRAIN_RENDER_DISTANCE * TERRAIN_SIZE;
}

void cameraDelete(t_camera * camera) {
	(void)camera;
}

static void cameraUpdateMatrices(t_camera * camera) {

	//matrices
	t_vec3f * viewvec = &(camera->vview);
	t_mat4f * view = &(camera->mview);
	t_mat4f * proj = &(camera->mproj);
	t_mat4f * viewproj = &(camera->mviewproj);

	//view vector
	float pitch = DEG_TO_RAD(camera->rot.pitch);
	float yaw = DEG_TO_RAD(camera->rot.yaw);
	float roll = DEG_TO_RAD(camera->rot.roll);
	float cospitch = cos(pitch);
	viewvec->x = cospitch * sin(yaw);
	viewvec->y = -sin(pitch);
	viewvec->z = -cospitch * cos(yaw);
	vec3f_normalize(viewvec, viewvec);

	//view matrix
	mat4f_identity(view);
	mat4f_rotateX(view, view, pitch);
	mat4f_rotateY(view, view, yaw);
	mat4f_rotateZ(view, view, roll);
	mat4f_translate(view, view, -camera->pos.x, -camera->pos.y, -camera->pos.z);

	//projection matrix
	float aspect = 1.6f;
	mat4f_perspective(proj, aspect, DEG_TO_RAD(camera->fov), camera->near_distance, camera->far_distance);

	//combine view and projection
	mat4f_mult(viewproj, proj, view);
}

void cameraUpdate(t_glh_context * context, t_world * world, t_renderer * renderer, t_camera * camera) {
	(void)context;
	(void)world;
	(void)renderer;
	(void)camera;

	//update camera matrices
	cameraUpdateMatrices(camera);

	//update camera world index
	worldGetGridIndex(world, camera->pos.x, camera->pos.y, &(camera->terrain_index.x), &(camera->terrain_index.y));
}