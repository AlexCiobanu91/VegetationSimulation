/* Alexandru-Catalin Ciobanu 341C4*/
#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>

#include "VertexFormat.h"
#include "Terrain.h"
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

typedef std::vector<std::vector<unsigned int> > matrice;

typedef enum LODLEVELS {
	FULLLOD = 0,
	PARTIALLOD = 1,
	LOWLOD = 2,
	VERYLOWLOD = 3,
	INVISIBLE = 4
};

class Tree
{
public:
	Tree();

	Tree(Terrain* terrain);

	~Tree();

	void drawTree(int lodLevel);

	void drawLeaves(int lodLevel);

	void createTree();

	glm::vec3 getStartPosition();

	static int selectTreeLod(glm::vec3 eyePosition, glm::vec3 treePosition);

private:
	int _detailLevel;
	int _noLeavesPerBranchCircle = 2;
	int _leafSize = 4.5f;

	int _noTrunkCircles = 6;
	int _trunkLevelHeight = 40;
	int _trunkRadius = 20;

	float _smoothCircles = 5;

	int _maxBranchLevel = 5;

	float _noBranchCircles = 6;

	float _maxBranchLength = 150;
	float _minBranchLength = 10;

	float _maxBranchRadius = 15;
	float _minBranchRadius = 0.2f;

	float _branchRandomness = 4;

	int _lodLevels = 4;


	Terrain * _terrain;

	float _resolution;

	glm::vec3 vecStartPosition;

	std::vector<unsigned int> _tree_vao, _tree_vbo, _tree_ibo, _tree_indices;
	matrice _branch_vao, _branch_vbo, _branch_ibo, _branch_indices;
	std::vector<unsigned int> _leaves_vao, _leaves_vbo, _leaves_ibo, _leaves_indices;

	std::vector<unsigned int> _noBranches;
	unsigned int _noLeaves;

	void createBranch(glm::vec3 startCirclePosition, glm::vec3 parentRotation, float branchLength, float branchSize, int level);
	void createLeaves(glm::vec3 branchStartCirclePosition, float branchLength, float branchSize, float rotateAngle, float branchCircle);

	void selectResolution(int lodLevel);

	glm::vec3 getRotationVector(glm::vec3 vecRotation);
	glm::vec3 getOppositeRotation(glm::vec3 vecRotation);

	

};

