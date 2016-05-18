#version 330
layout(location = 0) out vec4 out_color;

in vec3 fs_in_position;
in vec3 fs_eye_position;
in vec3 fs_in_color;

void main(){

	vec3 tex1 = fs_in_color;
	if (tex1.r<0.1 && tex1.g<0.1 && tex1.b<0.1) discard;	

	out_color = vec4(tex1, 1);
}