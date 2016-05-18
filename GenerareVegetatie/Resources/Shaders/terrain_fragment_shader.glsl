#version 410 core
layout(location = 0, index = 0) out vec4 out_color;
uniform sampler2D textura;

//in vec3 fs_in_light;
in vec2 fs_in_texcoord;

void main()
{

	 vec3 tex1 = texture(textura, fs_in_texcoord).xyz;
	 if (tex1.r<0.1 && tex1.g<0.1 && tex1.b<0.1) discard;	
	out_color = vec4(tex1,1);
}