/* Alexandru-Catalin Ciobanu 341C4*/
#pragma once

#include<iostream>
#include<stdio.h>;
#include<stdlib.h>;
#include<fstream>;
#include<vector>;

#include "..\Models\VertexFormat.h"
#include "..\Models\Terrain.h"
#include "..\Models\Tree.h"
#include "GL\glew.h"
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define M_PI 3.1415926535897932384626433832795

class Grass
{
private:
		std::vector<glm::vec3> _vecPositions;
		Tree* _tree;
		float _grassWidth, _grassHeight;
		std::vector<unsigned int> _grass_vao, _grass_vbo, _grass_ibo, _grass_indices;
		int _indicesCount;
		int _noGrassCount;
		float _nGrassSize;
		float _grassRadius;
		GLuint _shaderProgram;
		Terrain * _terrain;
		int _noLODLevels = 3;
		int _coordX = 0;
		int _coordZ = 0;
		int _coordY = 0;
		int _terrainScaleFactor = 0;
		int _noGrass;

		glm::vec3 _startPosition;

		std::vector<int> _noGrassIndices;
		void createFullBlade(glm::vec3 bladePosition, std::vector<VertexFormat> &vertices, int lod);
		void createPartialBlade(glm::vec3 bladePosition, std::vector<VertexFormat> &vertices, int lod);

		void computePartialBladeIndices(std::vector<unsigned int> &indices, int lodLevel);
		void computeFullBladeIndices(std::vector<unsigned int> &indices, int lodLevel);

		


public:
	Grass();

	Grass(int noGrassCount, int grassWidth, int grassHeight, float radius, Tree* tree);

	Grass(int noGrassCount, float grassWidth, float grassHeight, int coordX, int coordZ,int grassRadius, Terrain * terrain);

	void createGrass();

	void createGrassBlade();

	void createHighDetailGrass();

	void drawGrass(int lodLevel);

	glm::vec3 getStartPosition();

	~Grass();

	static int selectGrassLod(glm::vec3 eyePosition, glm::vec3 treePosition, float grassRadius);
};

