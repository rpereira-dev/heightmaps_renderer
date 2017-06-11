#version 330 core

in vec2   pos;
in vec2   uv;
in float  height;
in vec2   normal;
in int    textureID;

out float     visibility;
out vec3      pass_normal;
out vec2      pass_uv;
flat out int  pass_textureID;
out vec3      viewVec;

//view and projection matrix
uniform mat4 mvp_matrix;

//transformation matrix
uniform mat4 transf_matrix;

# define TERRAIN_SIZE (16.0)
# define TERRAIN_RENDER_DISTANCE (64)
# define RENDER_DISTANCE (TERRAIN_RENDER_DISTANCE * TERRAIN_SIZE)

void main(void) {

    vec4 world_pos = transf_matrix * vec4(pos.x, height, pos.y, 1.0);
    viewVec = normalize(-world_pos.xyz);
  	gl_Position = mvp_matrix * world_pos;

  	//fog calculation, visibility = pow(distance, 8)
    visibility = 0.0f;
	visibility = length(gl_Position.xz) / float(RENDER_DISTANCE);
    visibility = visibility * visibility;
    visibility = visibility * visibility;
    visibility = visibility * visibility;
    visibility = clamp(visibility, 0, 1);

    pass_normal = normalize(vec3(normal.x, 1.0, normal.y));
    pass_uv = uv;
    pass_textureID = textureID;
}