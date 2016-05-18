#version 330
layout(location = 0) out vec4 out_color;

uniform int tex;
uniform sampler2D textura;
uniform sampler2D textura1;
in vec2 g_texcoords;
void main(){
	vec3 tex1;
	//if (tex == 0) {
		tex1 = texture(textura, g_texcoords).xyz;
	//} else {
	//	tex1 = texture(textura1, g_texcoords).xyz;
	//}
	

	if (tex1.r<0.1 && tex1.g<0.1 && tex1.b<0.1) discard;	
	
	out_color = vec4(tex1, 1);
}