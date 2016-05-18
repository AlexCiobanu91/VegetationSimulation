#version 330

layout(location = 0) in vec3 in_position;

uniform mat4 model_matrix, view_matrix, projection_matrix;

out vec3 texcoord;

void main(){

	texcoord = in_position;

    gl_Position = projection_matrix * view_matrix * model_matrix*vec4(in_position, 1);
}