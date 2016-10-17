#version 330 core

in vec2   pos;
in float  height;
in vec3   normal;
in vec3   color;

out vec3  pass_color;
out vec3  pass_normal;
out float visibility;

//view and projection matrix
uniform mat4 mvp_matrix;

//transformation matrix
uniform mat4 transf_matrix;

//weather data
uniform float fog_density;
uniform float fog_gradient;

void main(void) {
  vec4 world_pos = transf_matrix * vec4(pos.x, height, pos.y, 1.0);
  gl_Position = mvp_matrix * world_pos;

  //fog calculation
  float distance = length(gl_Position.xyz);
  visibility = exp(-pow(distance * fog_density, fog_gradient));
  visibility = clamp(visibility, 0, 1);

  pass_color = color;
  pass_normal = normal;
}