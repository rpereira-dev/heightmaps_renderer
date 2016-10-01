#version 330 core

in vec3 pos;
in vec3 color;

out vec3 pass_color;

//view and projection matrix
uniform mat4 mvp_matrix;

//transformation matrix
uniform mat4 transf_matrix;

void main(void) {
	gl_Position = mvp_matrix * transf_matrix * vec4(pos, 1.0);
	pass_color = color;
}