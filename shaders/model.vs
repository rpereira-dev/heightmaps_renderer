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

uniform int time;

# define STATE_APPLY_FOG (1)
# define STATE_APPLY_PHONG_LIGHTNING (2)

# define TERRAIN_SIZE (16)
# define TERRAIN_RENDER_DISTANCE (32)
# define RENDER_DISTANCE (TERRAIN_RENDER_DISTANCE * TERRAIN_SIZE)

void main(void) {
	vec4 world_pos = transf_matrix * vec4(pos.x, height, pos.y, 1.0);
  	gl_Position = mvp_matrix * world_pos;

  	//fog calculation
    float visibility = 0.0f;
  	if ((state & STATE_APPLY_FOG) != STATE_APPLY_FOG) {
      //visibility^8
  		visibility = length(gl_Position.xyz) / float(RENDER_DISTANCE);
      visibility += sin(time * 0.5f) * 0.025f + sin(42.0f + time * 0.5f) * 0.01f + sin(42.5f + time * 0.4f) * 0.01f;

      visibility = visibility * visibility;
      visibility = visibility * visibility;
      visibility = visibility * visibility;
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

	pass_color = mix(color * diffuse, sky_color, visibility);
}