#version 410 core

// define the number of CPs in the output patch
layout (vertices = 3) out;

uniform vec3 gEyeWorldPos;

// attributes of the input CPs
 in vec3 cs_in_position[];
 in vec2 cs_in_texcoord[];
 in vec3 cs_in_normal[];

// attributes of the output CPs
 out vec3 es_in_position[];
 out vec2 es_in_texcoord[];
 out vec3 es_in_normal[];

float GetTessLevel(float Distance0, float Distance1)
{
    float AvgDistance = (Distance0 + Distance1) / 2.0;

    if (AvgDistance <= 10.0) {
        return 10.0;
    }
    else if (AvgDistance <= 20.0) {
        return 7.0;
    }
    else {
        return 1.0;
    }
}

float level (vec4 poz1, vec4 poz2){
    float lod=1;
	float d=distance(poz1, poz2);
	if(d<10) lod=10;
	if(10<=d && d<30) lod=5;
	if(30<=d && d<50) lod=2;
	if(50<=d) lod=1;

	return lod;
}



#define ID gl_InvocationID

void main()
{
    // Set the control points of the output patch
     es_in_position[ID] = cs_in_position[ID];
     es_in_normal[ID]	= cs_in_normal[ID];
     es_in_texcoord[ID] = cs_in_texcoord[ID];
	
	if ( ID == 0) {
		vec3 d1=gl_in[1].gl_Position.xyz+(gl_in[2].gl_Position.xyz-gl_in[1].gl_Position.xyz)/2;
		vec3 d2=gl_in[0].gl_Position.xyz+(gl_in[2].gl_Position.xyz-gl_in[0].gl_Position.xyz)/2;
		vec3 d3=gl_in[0].gl_Position.xyz+(gl_in[1].gl_Position.xyz-gl_in[0].gl_Position.xyz)/2;

		float e0=level(vec4(d1,1.0),vec4(gEyeWorldPos,1.0));
		float e1=level(vec4(d2,1.0),vec4(gEyeWorldPos,1.0));
		float e2=level(vec4(d3,1.0),vec4(gEyeWorldPos,1.0));
		float m=min(e0,min(e1,e2));

		gl_TessLevelInner[0] = floor((min(e0,min(e1,e2))+max(e0,max(e1,e2)))/2);
		gl_TessLevelOuter[0] = e0;
		gl_TessLevelOuter[1] = e1;
		gl_TessLevelOuter[2] = e2;
	}

	/*
    // Calculate the distance from the camera to the three control points
    float EyeToVertexDistance0 = distance(gEyeWorldPos, es_in_position[0]);
    float EyeToVertexDistance1 = distance(gEyeWorldPos, es_in_position[1]);
    float EyeToVertexDistance2 = distance(gEyeWorldPos, es_in_position[2]);

    // Calculate the tessellation levels
	gl_TessLevelOuter[0] = GetTessLevel(EyeToVertexDistance1, EyeToVertexDistance2);
	gl_TessLevelOuter[1] = GetTessLevel(EyeToVertexDistance2, EyeToVertexDistance0);
	gl_TessLevelOuter[2] = GetTessLevel(EyeToVertexDistance0, EyeToVertexDistance1);
	gl_TessLevelInner[0] = gl_TessLevelOuter[0];
	*/

	gl_out[ID].gl_Position = gl_in[ID].gl_Position;

}