#version 330 core

in float 	visibility;
in vec3  	pass_normal;
in vec2  	pass_uv;
flat in int pass_textureID;
in vec3		viewVec;

out	vec4 vertexColor;

uniform vec3 sky_color;
uniform vec3 sunray;

uniform sampler2D textureSampler;

# define TX_UNIT (1 / 5.0)
# define MIN_INTENSITY (0.2)
# define SPECULAR_DAMPING (4.0)

void main(void) {
	//texture
	float uvx = (pass_uv.x * TX_UNIT + pass_textureID * TX_UNIT);
	float uvy = pass_uv.y;
	vec3 txcolor = texture(textureSampler, vec2(uvx, uvy)).rgb;
	vec3 color = txcolor;
  	//phong ligthing model
  	float intensity = max(dot(pass_normal, sunray), MIN_INTENSITY);
	color *= intensity;

  	//specular lighting
  	vec3 reflectVec = reflect(-sunray, pass_normal);
	float specAngle = max(dot(reflectVec, viewVec), 0.0);
	float specular = pow(specAngle, SPECULAR_DAMPING);
	color += specular * vec3(1.0, 1.0, 1.0);

	//apply fog
	vertexColor = vec4(mix(color, sky_color, visibility), 1.0);
}
