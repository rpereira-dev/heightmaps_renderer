#version 330 core

in vec4 pass_color;

out	vec4 vertexColor;

void main(void) {
	vertexColor = pass_color;
}
