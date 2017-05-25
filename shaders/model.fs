#version 330 core

in float 	visibility;
in vec3  	pass_normal;
in vec2  	pass_uv;
flat in int pass_textureID;

out	vec4 vertexColor;

uniform vec3 sky_color;
uniform int state;

uniform sampler2D textureSampler;


# define TERRAIN_DETAIL (16)
# define TERRAIN_SIZE (16)
# define TERRAIN_UNIT (TERRAIN_SIZE / TERRAIN_DETAIL)
# define TERRAIN_RENDER_DISTANCE (64)
# define TERRAIN_KEEP_LOADED_DISTANCE (TERRAIN_LOADED_DISTANCE)
# define MAX_NUMBER_OF_TERRAIN_LOADED (TERRAIN_KEEP_LOADED_DISTANCE * TERRAIN_KEEP_LOADED_DISTANCE * 2 * 2)
# define TERRAIN_FLOATS_PER_VERTEX (3 + 3 + 1)

# define STATE_APPLY_FOG (1)
# define STATE_APPLY_PHONG_LIGHTNING (2)

# define TX_UNIT (1 / 5.0)

void main(void) {

  	//lumiere diffuse
  	vec3 diffuse = vec3(1.0, 1.0, 1.0);

  	//phong ligthing model
  	if ((state & STATE_APPLY_PHONG_LIGHTNING) != STATE_APPLY_PHONG_LIGHTNING) {
  		vec3 sun = normalize(vec3(1.0, 1.0, 0.2));
	  	float n = dot(pass_normal, sun);
	  	float intensity = max(n, 0.3);
	  	diffuse *= intensity;
	}

	float uvx = (pass_uv.x * TX_UNIT + pass_textureID * TX_UNIT);
	float uvy = pass_uv.y;
	vec3 txcolor = texture(textureSampler, vec2(uvx, uvy)).rgb;
	vec3 color = mix(txcolor * diffuse, sky_color, visibility);
	vertexColor = vec4(color, 1.0);
}
