#version 410 core

layout(points) in;
layout(triangle_strip, max_vertices = 7) out;



out vec3 fs_in_position;
out vec3 fs_eye_position;
out vec3 fs_in_color;
out vec2 fs_in_texcoords;

vec4 quat_from_axis_angle(vec3 axis, float angle)
{ 
  vec4 qr;
  float half_angle = (angle * 0.5) * 3.14159 / 180.0;
  qr.x = axis.x * sin(half_angle);
  qr.y = axis.y * sin(half_angle);
  qr.z = axis.z * sin(half_angle);
  qr.w = cos(half_angle);
  return qr;
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
 
vec4 quat_conj(vec4 q)
{ 
  return vec4(-q.x, -q.y, -q.z, q.w); 
}
  
vec4 quat_mult(vec4 q1, vec4 q2)
{ 
  vec4 qr;
  qr.x = (q1.w * q2.x) + (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y);
  qr.y = (q1.w * q2.y) - (q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x);
  qr.z = (q1.w * q2.z) + (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w);
  qr.w = (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z);
  return qr;
}
 
vec3 rotate_vertex_position(vec3 position, vec3 axis, float angle)
{ 
  vec4 qr = quat_from_axis_angle(axis, angle);
  vec4 qr_conj = quat_conj(qr);
  vec4 q_pos = vec4(position.x, position.y, position.z, 0);
  
  vec4 q_tmp = quat_mult(qr, q_pos);
  qr = quat_mult(q_tmp, qr_conj);
  
  return vec3(qr.x, qr.y, qr.z);
}



void createPartialBlade()
{
	vec3 svPosition = gl_in[0].gl_Position.xyz;

	float fOffsetX1 = grassWidth * 3.0f / 5.0f;
	float fOffsetX2 = grassWidth * 3.0f / 2.0f;
	float fOffsetX3 = 3.5f * grassWidth;

	float fHDist = grassWidth;

	float fHeight1 = grassHeight / 3.0f;
	float fHeight2 = grassHeight * 2.0f / 3.0f;
	float fHeight3 = grassHeight;

	vec3 vertexPosition;
    vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 offset = computeOffset(svPosition);

	float rotateAngle = int(svPosition.x * svPosition.z) % 180 + 10;

	if (distance (eye_position, svPosition) < 300) {
		// 0
		vertexPosition = svPosition + vec3(-fHDist, fHeight1, 0) + offset - svPosition;
		vertexPosition = rotate_vertex_position(vertexPosition, up, rotateAngle);
		vertexPosition += svPosition;
		gl_Position = projection_matrix * view_matrix * vec4(vertexPosition, 1);
	
		fs_in_position = vertexPosition;
		fs_eye_position = eye_position;
		fs_in_color = vec3(0.211, 0.458, 0.121);
		EmitVertex();

		// 1 
		vertexPosition = svPosition + vec3(0, fHeight1 * 2.0f / 3.0f, 0) + offset - svPosition;
		vertexPosition = rotate_vertex_position(vertexPosition, up, rotateAngle);
		vertexPosition += svPosition;
		gl_Position = projection_matrix * view_matrix * vec4(vertexPosition, 1);

		fs_in_position = vertexPosition;
		fs_eye_position = eye_position;
		fs_in_color = vec3(0.211, 0.458, 0.121);
		EmitVertex();
	}
	if (distance (eye_position, svPosition) < 600) {
		
		// 2
		vertexPosition = svPosition - svPosition;
		vertexPosition = rotate_vertex_position(vertexPosition, up, rotateAngle);
		vertexPosition += svPosition;
		gl_Position = projection_matrix * view_matrix * vec4(vertexPosition, 1);
	
		fs_in_position = vertexPosition;
		fs_eye_position = eye_position;
		fs_in_color = vec3(0.070, 0.258, 0);
		EmitVertex();

		// 3 
		vertexPosition = svPosition + vec3(fOffsetX1, 0, 0) - svPosition;
		vertexPosition = rotate_vertex_position(vertexPosition, up, rotateAngle);
		vertexPosition += svPosition;
		gl_Position = projection_matrix * view_matrix * vec4(vertexPosition, 1);

		fs_in_position = vertexPosition;
		fs_eye_position = eye_position;
		fs_in_color = vec3(0.070, 0.258, 0);
		EmitVertex();
	}
	if (distance (eye_position, svPosition) < 1500) {
		// 4 
		vertexPosition = svPosition + vec3(fOffsetX1, fHeight1, 0) + offset - svPosition;
		vertexPosition = rotate_vertex_position(vertexPosition, up, rotateAngle);
		vertexPosition += svPosition;
		gl_Position = projection_matrix * view_matrix * vec4(vertexPosition, 1);

		fs_in_position = vertexPosition;
		fs_eye_position = eye_position;
		fs_in_color = vec3(0.141, 0.360, 0.062);
		EmitVertex();
	
		// 5 
		vertexPosition = svPosition + vec3(fOffsetX1 + fHDist, fHeight1, 0) + offset - svPosition;
		vertexPosition = rotate_vertex_position(vertexPosition, up, rotateAngle);
		vertexPosition += svPosition;
		gl_Position = projection_matrix * view_matrix * vec4(vertexPosition, 1);

		fs_in_position = vertexPosition;
		fs_eye_position = eye_position;
		fs_in_color = vec3(0.141, 0.360, 0.062);
		EmitVertex();
	
		// 6 
		vertexPosition = svPosition + vec3(fOffsetX2 + fHDist, fHeight2, 0) + offset - svPosition;
		vertexPosition = rotate_vertex_position(vertexPosition, up, rotateAngle);
		vertexPosition += svPosition;
		gl_Position = projection_matrix * view_matrix * vec4(vertexPosition, 1);

		fs_in_position = vertexPosition;
		fs_eye_position = eye_position;
		fs_in_color = vec3(0.211, 0.458, 0.121);
		EmitVertex();
	}
	EndPrimitive();
}

void createFullBlade()
{
	vec3 svPosition = gl_in[0].gl_Position.xyz;

	float fOffsetX1 = grassWidth * 3.0f / 5.0f;
	float fOffsetX2 = grassWidth * 3.0f / 2.0f;
	float fOffsetX3 = 3.5f * grassWidth;

	float fHDist = grassWidth;

	float fHeight1 = grassHeight / 3.0f;
	float fHeight2 = grassHeight * 2.0f / 3.0f;
	float fHeight3 = grassHeight;

	vec3 vertexPosition;
    vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 offset = computeOffset(svPosition);

	float rotateAngle = int(svPosition.x * svPosition.z) % 180 + 10;
	if (distance (eye_position, svPosition) < 300) {
		// 0
		vertexPosition = svPosition - svPosition;
		vertexPosition = rotate_vertex_position(vertexPosition, up, rotateAngle);
		vertexPosition += svPosition;
		gl_Position = projection_matrix * view_matrix * vec4(vertexPosition, 1);
	
		fs_in_position = vertexPosition;
		fs_eye_position = eye_position;
		fs_in_color = vec3(0.070, 0.258, 0);
		fs_in_texcoords = vec2(1,1);
		EmitVertex();

		// 1 
		vertexPosition = svPosition + vec3(fHDist, 0, 0) - svPosition;
		vertexPosition = rotate_vertex_position(vertexPosition, up, rotateAngle);
		vertexPosition += svPosition;
		gl_Position = projection_matrix * view_matrix * vec4(vertexPosition, 1);

		fs_in_position = vertexPosition;
		fs_eye_position = eye_position;
		fs_in_color = vec3(0.070, 0.258, 0);
		EmitVertex();

	}

	if (distance (eye_position, svPosition) < 600) {
		// 2
		vertexPosition = svPosition + vec3(fOffsetX1, fHeight1, 0) + offset - svPosition;
		vertexPosition = rotate_vertex_position(vertexPosition, up, rotateAngle);
		vertexPosition += svPosition;
		gl_Position = projection_matrix * view_matrix * vec4(vertexPosition, 1);
	
		fs_in_position = vertexPosition;
		fs_eye_position = eye_position;
		fs_in_color = vec3(0.094, 0.329, 0.011);
		EmitVertex();


		//3 
		vertexPosition = svPosition + vec3(fOffsetX1 + fHDist, fHeight1, 0) + offset - svPosition; 
		vertexPosition = rotate_vertex_position(vertexPosition, up, rotateAngle);
		vertexPosition += svPosition;
		gl_Position = projection_matrix * view_matrix * vec4(vertexPosition, 1);

		fs_in_position = vertexPosition;
		fs_eye_position = eye_position;
		fs_in_color = vec3(0.141, 0.360, 0.062);
		EmitVertex();
	}
	if (distance (eye_position, svPosition) < 1500) {
		// 4 
		vertexPosition = svPosition + vec3(fOffsetX2, fHeight2, 0) + offset - svPosition;
		vertexPosition = rotate_vertex_position(vertexPosition, up, rotateAngle);
		vertexPosition += svPosition;
		gl_Position = projection_matrix * view_matrix * vec4(vertexPosition, 1);

		fs_in_position = vertexPosition;
		fs_eye_position = eye_position;
		fs_in_color = vec3(0.160, 0.411, 0.066);
		EmitVertex();

		// 5
		vertexPosition = svPosition + vec3(fOffsetX2 + fHDist, fHeight2, 0) + offset - svPosition;
		vertexPosition = rotate_vertex_position(vertexPosition, up, rotateAngle);
		vertexPosition += svPosition;
		gl_Position = projection_matrix * view_matrix * vec4(vertexPosition, 1);

		fs_in_position = vertexPosition;
		fs_eye_position = eye_position;
		fs_in_color = vec3(0.203, 0.411, 0.129);
		EmitVertex();

		// 6
		vertexPosition = svPosition + vec3(fOffsetX3, fHeight3, 0) + offset - svPosition;
		vertexPosition = rotate_vertex_position(vertexPosition, vec3(0,1,0), rotateAngle);
		vertexPosition += svPosition;
		gl_Position = projection_matrix * view_matrix * vec4(vertexPosition, 1);

		fs_in_position = vertexPosition;
		fs_eye_position = eye_position;
		fs_in_color = vec3(0.211, 0.458, 0.121);
		EmitVertex();
	}
	EndPrimitive();

}


void main(void)
{
	if (gl_PrimitiveIDIn % 2 == 0) {
		createFullBlade();
	} else {
		createPartialBlade();
	}
}
