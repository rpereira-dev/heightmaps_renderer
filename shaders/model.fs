#version 330 core

in float visibility;
in vec3  pass_normal;
in vec2  pass_uv;

out	vec4 vertexColor;

uniform vec3 sky_color;
uniform int state;

uniform sampler2D textureSampler;


# define TERRAIN_DETAIL (16)
# define TERRAIN_SIZE (16)
# define TERRAIN_UNIT (TERRAIN_SIZE / TERRAIN_DETAIL)
# define TERRAIN_RENDER_DISTANCE (48)
# define TERRAIN_KEEP_LOADED_DISTANCE (TERRAIN_LOADED_DISTANCE)
# define MAX_NUMBER_OF_TERRAIN_LOADED (TERRAIN_KEEP_LOADED_DISTANCE * TERRAIN_KEEP_LOADED_DISTANCE * 2 * 2)
# define TERRAIN_FLOATS_PER_VERTEX (3 + 3 + 1)

# define STATE_APPLY_FOG (1)
# define STATE_APPLY_PHONG_LIGHTNING (2)

void main(void) {

  	//lumiere diffuse
  	vec3 diffuse = vec3(1.0, 1.0, 1.0);

  	//phong ligthing model
  	if ((state & STATE_APPLY_PHONG_LIGHTNING) != STATE_APPLY_PHONG_LIGHTNING) {
  		vec3 sun = normalize(vec3(1.0, 0.8, 1.0));
	  	float n = dot(pass_normal, sun);
	  	float intensity = max(n, 0.2);
	  	diffuse *= intensity;
	}

	vec3 color = mix(texture(textureSampler, pass_uv * 16.0) * diffuse, sky_color, visibility);
	vertexColor = vec4(color, 1.0);
}
