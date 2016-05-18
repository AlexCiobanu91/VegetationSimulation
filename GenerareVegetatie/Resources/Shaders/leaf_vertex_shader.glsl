#version 330

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoords;
layout(location = 3) in vec3 in_start_circle_position;
layout(location = 4) in vec3 in_branch_details;
layout(location = 5) in vec2 in_branch_details2;

uniform mat4 model_matrix, view_matrix, projection_matrix;

uniform float time;   
uniform vec3 wind;
uniform float circlesPerBranch;
uniform float branchLevels;
out vec2 g_texcoords;

vec3 computeBranchOffset() {

	int branchCircle = int(in_branch_details.x);

	float intensityStepX = wind.x / circlesPerBranch;
	float intensityStepY = wind.y / circlesPerBranch;
	float intensityStepZ = wind.z / circlesPerBranch;

	float previousOffsetX = intensityStepX * (circlesPerBranch - 1) * time;
	float previousOffsetY = intensityStepY * (circlesPerBranch - 1) * time;
	float previousOffsetZ = intensityStepZ * (circlesPerBranch - 1) * time;

	previousOffsetX *= (branchLevels - 1);
	previousOffsetY *= (branchLevels - 1);
	previousOffsetZ *= (branchLevels - 1);

	return vec3(intensityStepX * branchCircle * time + previousOffsetX,
				intensityStepY * branchCircle * time + previousOffsetY,
				intensityStepY * branchCircle * time + previousOffsetZ);
}

void main() {

	vec3 out_position = in_position + computeBranchOffset();
    
	g_texcoords = in_texcoords.xy;
		
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(out_position, 1);
}
