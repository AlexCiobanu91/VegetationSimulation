/* Alexandru-Catalin Ciobanu 341C4*/
#pragma once

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

#include "VertexFormat.h"

class Terrain
{
private:
	std::vector<VertexFormat> _vecVertices;
	std::vector<unsigned int> _vecIndices;
	std::vector<float> _vecHeights;
	int _nHeight;
	int _nWidth;

	unsigned int _nVao;
	unsigned int _nVbo;
	unsigned int _nIbo;
	unsigned int _nIndices;
	unsigned int _terrainScale = 40;

public:

	Terrain();

	Terrain(const std::string &filename, const std::string &filename2, int height);

	~Terrain();

	void computeNormals(float level);

	void readTerrain(const std::string &filename, const std::string &filename2, int height);

	void loadTerrain(const std::string &filename, const std::string &filename2, int height);

	float getCoordHeight(int i, int j);

	void render();

	int getWidth();

	int getHeight();
};



