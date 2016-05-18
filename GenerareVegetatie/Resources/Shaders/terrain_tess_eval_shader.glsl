#version 410 core

layout(triangles, equal_spacing, ccw) in;

// uniform vec3 light_position;
// uniform vec3 eye_position;
// uniform int material_shininess;
// uniform float material_kd;
// uniform float material_ks;

 in vec3 es_in_position[];
 in vec2 es_in_texcoord[];
 in vec3 es_in_normal[];

out vec2 fs_in_texcoord;
out vec3 fs_in_normal;

// float ambientalGlobalLight = 1.0;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
   	return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
   	return normalize(vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2);
}

void main()
{

   	// Interpolate the attributes of the output vertex using the barycentric coordinates
     fs_in_texcoord = interpolate2D(es_in_texcoord[0], es_in_texcoord[1], es_in_texcoord[2]);
	 fs_in_normal = interpolate3D(es_in_normal[0], es_in_normal[1], es_in_normal[2]);

   	 vec3 fs_in_position = interpolate3D(es_in_position[0], es_in_position[1], es_in_position[2]);

	// Normal_FS_in = interpolate3D(Normal_ES_in[0], Normal_ES_in[1], Normal_ES_in[2]);
   	// Normal_FS_in = normalize(Normal_FS_in);
   	// WorldPos_FS_in = interpolate3D(WorldPos_ES_in[0], WorldPos_ES_in[1], WorldPos_ES_in[2]);

   	// Displace the vertex along the normal
   	// float Displacement = texture(gDisplacementMap, TexCoord_FS_in.xy).x;
   	// WorldPos_FS_in += Normal_FS_in * Displacement * gDispFactor;

	gl_Position =	gl_TessCoord.x * gl_in[0].gl_Position + 
					gl_TessCoord.y * gl_in[1].gl_Position + 
					gl_TessCoord.z * gl_in[2].gl_Position;
   	
}