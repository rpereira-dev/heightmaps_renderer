#version 330 core

in vec2 pos;
in float height;
//in vec3 color;

out vec4 pass_color;

//view and projection matrix
uniform mat4 mvp_matrix;

//transformation matrix
uniform mat4 transf_matrix;

void main(void) {
	gl_Position = mvp_matrix * transf_matrix * vec4(pos.x, height, pos.y, 1.0);
	//gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
	pass_color = vec4(1.0, 1.0, 0.0, 1.0);
}