#version 330 core

in vec3 pass_color;

out	vec4 vertexColor;

void main(void) {
  vertexColor = vec4(pass_color, 1);
}
