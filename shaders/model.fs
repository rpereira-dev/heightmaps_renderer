#version 330 core

in float 	visibility;
in vec3  	pass_normal;
in vec2  	pass_uv;
flat in int pass_textureID;
in vec3		viewVec;

out	vec4 vertexColor;

uniform vec3 sky_color;
uniform int state;
uniform vec3 sunray;

uniform sampler2D textureSampler;

# define STATE_APPLY_PHONG_LIGHTNING (2)
# define STATE_SPECULAR (32)
# define TX_UNIT (1 / 5.0)

float getDampingFactor() {
	if (pass_textureID == 0) {
		return (8.0);
	}
	if (pass_textureID == 4) {
		return (4.0);
	}
	return 16.0;
}

void main(void) {
	//texture
	float uvx = (pass_uv.x * TX_UNIT + pass_textureID * TX_UNIT);
	float uvy = pass_uv.y;
	vec3 txcolor = texture(textureSampler, vec2(uvx, uvy)).rgb;
	vec3 color = txcolor;
  	//phong ligthing model
  	if ((state & STATE_APPLY_PHONG_LIGHTNING) != STATE_APPLY_PHONG_LIGHTNING) {
	  	float intensity = max(dot(pass_normal, sunray), 0.2);
		color *= intensity;
		if ((state & STATE_SPECULAR) != STATE_SPECULAR) {
		  	//specular lighting
		  	vec3 reflectVec = reflect(-sunray, pass_normal);
			float specAngle = max(dot(reflectVec, viewVec), 0.0);
			float specular = pow(specAngle, getDampingFactor());
			color += specular * vec3(1.0, 1.0, 1.0);
		}
	}
	//apply fog
	vertexColor = vec4(mix(color, sky_color, visibility), 1.0);
}
