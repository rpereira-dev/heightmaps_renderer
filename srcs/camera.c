#include "renderer.h"

void cameraInit(t_camera * camera) {

	camera->pos.x = 0, camera->pos.y = 0, camera->pos.z = 0;
	camera->rot.pitch = 0, camera->rot.yaw = 0, camera->rot.roll = 0;
	camera->fov = 70.0f;
	camera->near_distance = 0.01f;
	camera->far_distance = 1000.0f;
	camera->render_distance = 1000.0f;
}

void cameraDelete(t_camera * camera) {
	(void)camera;
}

static void cameraUpdateMatrices(t_camera * camera) {

	//matrices
	static float deg_to_rad = 3.14159f * 2.0f / 360.0f;
	static float aspect = 1 / 1.6f;
	t_vec3f * viewvec = &(camera->vview);
	t_mat4f * view = &(camera->mview);
	t_mat4f * proj = &(camera->mproj);
	t_mat4f * viewproj = &(camera->mviewproj);
	t_vec3f vecbuffer;

	//view vector
	float f = cos(camera->rot.pitch * deg_to_rad);
	viewvec->x = f * sin(camera->rot.yaw * deg_to_rad);
	viewvec->y = sin(camera->rot.pitch);
	viewvec->z = -f * cos(camera->rot.yaw);
	vec3f_normalize(viewvec, viewvec);

	//view matrix
	vec3f_negate(&(vecbuffer), &(camera->pos));
	mat4f_identity(view);
	mat4f_rotateX(view, view, camera->rot.x * deg_to_rad);
	mat4f_rotateY(view, view, camera->rot.y * deg_to_rad);
	mat4f_rotateZ(view, view, camera->rot.z * deg_to_rad);
	mat4f_translate3(view, view, &(vecbuffer));

	//projection matrix
	mat4f_perspective(proj, aspect, camera->fov * deg_to_rad, camera->near_distance, camera->far_distance);

	//combine view and projection
	mat4f_mult(viewproj, view, proj);
}

void cameraUpdate(t_camera * camera) {
	cameraUpdateMatrices(camera);
}