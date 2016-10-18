#version 330 core

in vec2   pos;
in float  height;
in vec3   normal;
in vec3   color;

out vec3  pass_color;

//view and projection matrix
uniform mat4 mvp_matrix;

//transformation matrix
uniform mat4 transf_matrix;

//weather data
uniform float fog_density;
uniform float fog_gradient;

uniform vec3 sky_color;

uniform int state;

# define STATE_APPLY_FOG (1)
# define STATE_APPLY_PHONG_LIGHTNING (2)

void main(void) {
	vec4 world_pos = transf_matrix * vec4(pos.x, height, pos.y, 1.0);
  	gl_Position = mvp_matrix * world_pos;

  	//fog calculation
  	float visibility = 1.0;
  	if ((state & STATE_APPLY_FOG) != STATE_APPLY_FOG) {
  		float distance = length(gl_Position.xyz);
  		visibility = exp(-pow(distance * fog_density, fog_gradient));
  		visibility = clamp(visibility, 0, 1);
  	}

  	//lumiere diffuse
  	vec3 diffuse = vec3(1.0, 1.0, 1.0);

  	//phong ligthing model
  	if ((state & STATE_APPLY_PHONG_LIGHTNING) != STATE_APPLY_PHONG_LIGHTNING) {
  		vec3 sun = normalize(vec3(1.0, 1.0, 1.0));
	  	float n = dot(normal, sun);
	  	float intensity = max(n, 0.4);
	  	diffuse *= intensity;
	}

	pass_color = mix(sky_color, color * diffuse, visibility);
}