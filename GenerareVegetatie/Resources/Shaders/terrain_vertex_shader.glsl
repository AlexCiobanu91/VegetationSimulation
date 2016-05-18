#version 410 core

uniform mat4 model_matrix, view_matrix, projection_matrix;

layout(location = 0) in vec3 vs_in_position;		
layout(location = 1) in vec3 vs_in_normal;	
layout(location = 2) in vec2 vs_in_texcoord;	

out vec3 cs_in_position;
out vec2 cs_in_texcoord;
out vec3 cs_in_normal;

void main()
{
	 cs_in_position = vs_in_position;
	 cs_in_texcoord = vs_in_texcoord;
	 cs_in_normal = vs_in_normal;

	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vs_in_position, 1);

}
