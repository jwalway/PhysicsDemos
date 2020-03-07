#version 330
 
uniform mat4 u_pm;
layout (location = 0) in vec2 a_pos;
layout (location = 1) in float a_size;
layout (location = 2) in vec4 a_color;
layout (location = 3) in int a_txtnum;

out vec2 TexCoords;
out vec4 ParticleColor;

const vec2 pos[] = vec2[4](
  vec2(-0.5,  0.5),
  vec2(-0.5, -0.5),
  vec2(0.5,   0.5),
  vec2(0.5,  -0.5)
);

const vec2 tcoords0[] = vec2[4](
  vec2(0.0, 0.5),
  vec2(0.0, 0.0),
  vec2(0.5, 0.5),
  vec2(0.5, 0.0)
);

const vec2 tcoords1[] = vec2[4](
  vec2(0.0, 1.0),
  vec2(0.0, 0.5),
  vec2(0.5, 1.0),
  vec2(0.5, 0.5)
);

const vec2 tcoords2[] = vec2[4](
  vec2(0.5, 1.0),
  vec2(0.5, 0.5),
  vec2(1.0, 1.0),
  vec2(1.0, 0.5)
);

const vec2 tcoords3[] = vec2[4](
  vec2(0.5, 0.5),
  vec2(0.5, 0.0),
  vec2(1.0, 0.5),
  vec2(1.0, 0.0)
);
 
void main() {
  vec2 offset = pos[gl_VertexID];
  if(a_txtnum == 0)
  {
	TexCoords = tcoords0[gl_VertexID];
  }else if(a_txtnum == 1)
  {
	TexCoords = tcoords1[gl_VertexID];
  }else if(a_txtnum == 2)
  {
	TexCoords = tcoords2[gl_VertexID];
  }else 
  {
	TexCoords = tcoords3[gl_VertexID];
  }
  ParticleColor = a_color; //vec4(a_color,1.0f);
  gl_Position = u_pm * vec4(a_pos.x + (offset.x * a_size) ,
							a_pos.y + (offset.y * a_size) ,
							0.0,
							1.0);
	
}