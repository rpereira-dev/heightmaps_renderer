#version 330 core

in vec2   pos;
in vec2   uv;
in float  height;
in vec3   normal;
in int    textureID;

out float     visibility;
out vec3      pass_normal;
out vec2      pass_uv;
flat out int  pass_textureID;

//view and projection matrix
uniform mat4 mvp_matrix;

//transformation matrix
uniform mat4 transf_matrix;

uniform int state;

uniform int time;

# define TERRAIN_DETAIL (16)
# define TERRAIN_SIZE (16)
# define TERRAIN_UNIT (TERRAIN_SIZE / TERRAIN_DETAIL)
# define TERRAIN_RENDER_DISTANCE (48)
# define TERRAIN_KEEP_LOADED_DISTANCE (TERRAIN_LOADED_DISTANCE)
# define MAX_NUMBER_OF_TERRAIN_LOADED (TERRAIN_KEEP_LOADED_DISTANCE * TERRAIN_KEEP_LOADED_DISTANCE * 2 * 2)
# define TERRAIN_FLOATS_PER_VERTEX (3 + 3 + 1)
# define RENDER_DISTANCE (TERRAIN_RENDER_DISTANCE * TERRAIN_SIZE)

# define STATE_APPLY_FOG (1)
# define STATE_APPLY_PHONG_LIGHTNING (2)

void main(void) {
	vec4 world_pos = transf_matrix * vec4(pos.x, height, pos.y, 1.0);
  	gl_Position = mvp_matrix * world_pos;

  	//fog calculation
    visibility = 0.0f;
  	if ((state & STATE_APPLY_FOG) != STATE_APPLY_FOG) {
      //visibility^8
  		visibility = length(gl_Position.xz) / float(RENDER_DISTANCE);
      visibility += sin(time * 0.1f) * 0.025f + sin(42.0f + time * 0.1f) * 0.01f + sin(42.5f + time * 0.1f) * 0.01f;

      visibility = visibility * visibility;
      visibility = visibility * visibility;
      visibility = visibility * visibility;
      visibility = clamp(visibility, 0, 1);
  	}

    pass_normal = normal;
    pass_uv = uv;
    pass_textureID = textureID;
}