#version 330 core

in vec3 pass_color;
in vec3 pass_normal;
in float visibility;

out	vec4 vertexColor;

uniform vec3 sky_color;

void main(void) {

  //phong ligthing model
  vec3 sun = normalize(vec3(1.0, 1.0, 1.0));
  float n = dot(pass_normal, sun);
  float intensity = max(n, 0.4);
  vec3 diffuse = intensity * vec3(1.0, 1.0, 1.0);

  vertexColor = vec4(mix(sky_color, pass_color * diffuse, visibility), 1);

}
