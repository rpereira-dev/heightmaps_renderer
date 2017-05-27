#version 330 core

in float 	visibility;
in vec3  	pass_normal;
in vec2  	pass_uv;
flat in int pass_textureID;

out	vec4 vertexColor;

uniform vec3 sky_color;
uniform int state;
uniform vec3 sunray;

uniform sampler2D textureSampler;

# define STATE_APPLY_PHONG_LIGHTNING (2)
# define TX_UNIT (1 / 5.0)

void main(void) {

  	//lumiere diffuse
  	vec3 diffuse = vec3(1.0, 1.0, 1.0);

  	//phong ligthing model
  	if ((state & STATE_APPLY_PHONG_LIGHTNING) != STATE_APPLY_PHONG_LIGHTNING) {
	  	float n = dot(pass_normal, sunray);
	  	float intensity = max(n, 0.3);
	  	diffuse *= intensity;
	}

	//texture
	float uvx = (pass_uv.x * TX_UNIT + pass_textureID * TX_UNIT);
	float uvy = pass_uv.y;
	vec3 txcolor = texture(textureSampler, vec2(uvx, uvy)).rgb;
	vec3 color = mix(txcolor * diffuse, sky_color, visibility);
	vertexColor = vec4(color, 1.0);
}
