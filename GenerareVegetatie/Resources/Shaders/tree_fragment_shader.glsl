#version 330
layout(location = 0) out vec4 out_color;

uniform sampler2D textura;
in vec2 g_texcoords;
void main(){
	vec3 tex1 = texture(textura, g_texcoords).xyz;

	if (tex1.r<0.1 && tex1.g<0.1 && tex1.b<0.1) discard;	
	
	out_color = vec4(tex1, 1);
}