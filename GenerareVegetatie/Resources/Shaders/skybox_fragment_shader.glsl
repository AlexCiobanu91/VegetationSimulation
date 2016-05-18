#version 330
layout(location = 0) out vec3 out_color;

uniform samplerCube texture_cubemap;

in vec3 texcoord;

void main(){

	out_color = texture (texture_cubemap, texcoord).xyz; 
}