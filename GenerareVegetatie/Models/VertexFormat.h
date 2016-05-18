/* Alexandru-Catalin Ciobanu 341C4*/
#pragma once
#include "glm\glm.hpp"

typedef struct VertexFormat{
public:
	float coordX, coordY, coordZ;	
	float normalX, normalY, normalZ;						
	float texcoord_x, texcoord_y;							
	float branch_startX, branch_startY, branch_startZ;
	float branchCircle, branchRadius, branchLevel;
	float branchVertex, branchLength;
	VertexFormat(){
		coordX = coordY = coordZ = 0;
		normalX = normalY = normalZ = 0;
		texcoord_x = texcoord_y = 0;
	}

	VertexFormat(float px, float py, float pz){
		coordX = px;		coordY = py;		coordZ = pz;
		normalX = normalY = normalZ = 0;
		texcoord_x = texcoord_y = 0;
	}
	VertexFormat(float px, float py, float pz, float nx, float ny, float nz){
		coordX = px;		coordY = py;		coordZ = pz;
		normalX = nx;		normalY = ny;		normalZ = nz;
		texcoord_x = texcoord_y = 0;
	}

	VertexFormat(float px, float py, float pz, float tx, float ty){
		coordX = px;		coordY = py;		coordZ = pz;
		texcoord_x = tx;		texcoord_y = ty;
		normalX = normalY = normalZ = 0;
	}

	VertexFormat(float px, float py, float pz, float nx, float ny, float nz, float tx, float ty){
		coordX = px;		coordY = py;		coordZ = pz;
		normalX = nx;		normalY = ny;		normalZ = nz;
		texcoord_x = tx;		texcoord_y = ty;
		branch_startX = branch_startY = branch_startZ = 0;
		branchCircle = branchRadius = branchLevel = 0;
		branchVertex = branchLength = 0;
	}
} VertexFormat;