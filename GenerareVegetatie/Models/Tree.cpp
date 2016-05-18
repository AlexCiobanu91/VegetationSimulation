/* Alexandru-Catalin Ciobanu 341C4*/
#include "Tree.h"

Tree::Tree()
{
}

Tree::Tree(Terrain* terrain)
{
	_terrain = terrain;

	std::vector<unsigned int> tempVector = std::vector<unsigned int>(4);

	_tree_vao = tempVector;
	_tree_vbo = tempVector;
	_tree_ibo = tempVector;
	_tree_indices = tempVector;

	for (size_t i = 0; i < _lodLevels; i++)
	{
		_branch_vao.push_back(std::vector<unsigned int>());
		_branch_vbo.push_back(std::vector<unsigned int>());
		_branch_ibo.push_back(std::vector<unsigned int>());
		_branch_indices.push_back(std::vector<unsigned int>());

		

		_noBranches.push_back(0);
		
	}

	_noLeaves = 0;
}

Tree::~Tree()
{
}

void Tree::selectResolution(int lodLevel)
{
	switch (lodLevel) {
		case FULLLOD:
			_resolution = 0.2f;
			break;

		case PARTIALLOD:
			_resolution = 0.6f;
			break;

		case LOWLOD:
			_resolution = 0.8f;
			break;

		case VERYLOWLOD:
			_resolution = 1.0f;
			break;
	}
}

void Tree::drawTree(int lodLevel)
{
	if (lodLevel != INVISIBLE) {
		glBindVertexArray(_tree_vao[lodLevel]);
		glDrawElements(GL_TRIANGLES, _tree_indices[lodLevel], GL_UNSIGNED_INT, 0);

		for (int i = 0; i < _noBranches[lodLevel]; i++) {
			glBindVertexArray(_branch_vao[lodLevel][i]);
			glDrawElements(GL_TRIANGLES, _branch_indices[lodLevel][i], GL_UNSIGNED_INT, 0);
		}
	}

}

void Tree::drawLeaves(int lodLevel)
{
	if (lodLevel != INVISIBLE) {
		for (int i = 0; i < _noLeaves / (lodLevel + 1); i++) {
			glBindVertexArray(_leaves_vao[i]);
			glDrawElements(GL_TRIANGLES, _leaves_indices[i], GL_UNSIGNED_INT, 0);
		}
	}
}

glm::vec3 Tree::getOppositeRotation(glm::vec3 vecRotation) {
	int coordX = 0, coordY = 0, coordZ = 0;
	if (vecRotation.x != 0) {
		coordX = vecRotation.x == -1 ? 1 : -1;
	}

	if (vecRotation.y != 0) {
		coordY = vecRotation.y == -1 ? 1 : -1;
	}

	if (vecRotation.z != 0) {
		coordZ = vecRotation.z == -1 ? 1 : -1;
	}

	return glm::vec3(coordX, coordY, coordZ);
}

glm::vec3 Tree::getRotationVector(glm::vec3 vecRotation) {
	glm::vec3 vecNewRotation;

	int nRotation = rand() % 6;
	int nCoordX = rand() % 3 - 1;
	int nCoordY = rand() % 3 - 1;
	int nCoordZ = rand() % 3 - 1;
	vecNewRotation = glm::vec3(nCoordX, nCoordY, nCoordZ);
	while (	vecNewRotation == vecRotation || 
			vecNewRotation == getOppositeRotation(vecRotation) || 
			vecNewRotation == glm::vec3(0, 0, 0) ||
			vecNewRotation == glm::vec3(1, 1, 1) 
			) {
		int nCoordX = rand() % 3 - 1;
		int nCoordY = rand() % 3 - 1;
		int nCoordZ = rand() % 3 - 1;
		vecNewRotation = glm::vec3(nCoordX, nCoordY, nCoordZ);
	}
	
	return vecNewRotation;
}

void Tree::createLeaves(glm::vec3 branchStartCirclePosition, float branchLength, float branchSize, float rotateAngle, float branchCircle) {

	//for (int l = 0; l < _lodLevels; l++) {
		std::vector<VertexFormat> vecLeavesVertices;

		float fStartX = branchStartCirclePosition.x;
		float fStartY = branchStartCirclePosition.y;
		float fStartZ = branchStartCirclePosition.z;
		float fLeavesDistances = branchLength / _noLeavesPerBranchCircle;

		std::vector<unsigned int> indices;
		float rotateStep = 0;
		for (int i = 0; i < _noLeavesPerBranchCircle * 4; i += 4)
		{
			float randX = rand(); //% 360;
			float randZ = rand(); //% 360;
			float fPozX = fStartX + branchSize * 3 * cos(randX);
			float fPozZ = fStartZ + branchSize * 3 * sin(randZ);

			glm::mat4 trans1, trans2, trans3, trans4;
			//trans1 = trans;
			glm::vec3 vecRotation;

			rotateStep = rand() % 360;
			vecRotation = glm::vec3(1, 1, 1);
			trans1 = glm::rotate(trans1, glm::radians(rotateStep), vecRotation);
			glm::vec4 vRotation1 = trans1 * glm::vec4(0, 0, 0, 1);

			rotateStep = rand() % 360;
			vecRotation = getRotationVector(glm::vec3(0, 0, 0));
			trans2 = glm::rotate(trans1, glm::radians(rotateStep), vecRotation);
			glm::vec4 vRotation2 = trans1 * glm::vec4(_leafSize, 0, 0, 1);

			rotateStep = rand() % 360;
			vecRotation = getRotationVector(glm::vec3(0, 0, 0));
			trans3 = glm::rotate(trans1, glm::radians(rotateStep), vecRotation);
			glm::vec4 vRotation3 = trans1 * glm::vec4(_leafSize, _leafSize, 0, 1);

			rotateStep = rand() % 360;
			vecRotation = getRotationVector(glm::vec3(0, 0, 0));
			trans4 = glm::rotate(trans1, glm::radians(rotateStep), vecRotation);
			glm::vec4 vRotation4 = trans1 * glm::vec4(0, _leafSize, 0, 1);

			VertexFormat vertex1 = VertexFormat(fPozX + vRotation1.x, fStartY + vRotation1.y, fPozZ + vRotation1.z, 0, 1, 0, 1, 1);
			VertexFormat vertex2 = VertexFormat(fPozX + vRotation2.x + _leafSize, fStartY + vRotation2.y, fPozZ + vRotation2.z, 0, 1, 0, 0, 1);
			VertexFormat vertex3 = VertexFormat(fPozX + vRotation3.x + _leafSize, fStartY + vRotation3.y + _leafSize, fPozZ + vRotation3.z, 0, 1, 0, 0, 0);
			VertexFormat vertex4 = VertexFormat(fPozX + vRotation4.x, fStartY + vRotation4.y + _leafSize, fPozZ + vRotation4.z, 0, 1, 0, 1, 0);

			vertex1.branch_startX = fStartX;
			vertex1.branch_startY = fStartY;
			vertex1.branch_startZ = fStartZ;			

			vertex2.branch_startX = fStartX;
			vertex2.branch_startY = fStartY;
			vertex2.branch_startZ = fStartZ;

			vertex3.branch_startX = fStartX;
			vertex3.branch_startY = fStartY;
			vertex3.branch_startZ = fStartZ;

			vertex4.branch_startX = fStartX;
			vertex4.branch_startY = fStartY;
			vertex4.branch_startZ = fStartZ;

			vertex1.branchCircle = branchCircle;
			vertex2.branchCircle = branchCircle;
			vertex3.branchCircle = branchCircle;
			vertex4.branchCircle = branchCircle;

			vecLeavesVertices.push_back(vertex1);
			vecLeavesVertices.push_back(vertex2);
			vecLeavesVertices.push_back(vertex3);
			vecLeavesVertices.push_back(vertex4);

			indices.push_back(i + 0); indices.push_back(i + 1); indices.push_back(i + 2);
			indices.push_back(i + 2); indices.push_back(i + 3);  indices.push_back(i + 0);
		}

		unsigned int vao, vbo, ibo;
		//vertex array object -> un obiect ce reprezinta un container pentru starea de desenare
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		//vertex buffer object -> un obiect in care tinem vertecsii
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vecLeavesVertices.size()*sizeof(VertexFormat), &vecLeavesVertices[0], GL_STATIC_DRAW);

		//index buffer object -> un obiect in care tinem indecsii
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		//legatura intre atributele vertecsilor si pipeline, datele noastre sunt INTERLEAVED.
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0);						//trimite pozitii pe pipe 0
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(float) * 3));		//trimite normale pe pipe 1
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(float) * 3));	//trimite texcoords pe pipe 2
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(float) * 8));	// trimite pozitia de start a crengii
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(float) * 11));	// trimite detalii creanga
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(float) * 14));	// trimite detalii creanga
		_leaves_vao.push_back(vao);
		_leaves_vbo.push_back(vbo);
		_leaves_ibo.push_back(ibo);

		_leaves_indices.push_back(indices.size());

		_noLeaves ++;
	//}
}

void Tree::createBranch(glm::vec3 startCirclePosition, glm::vec3 parentRotation, float branchLength, float branchSize, int level) {

	std::vector<glm::vec3> vecCirclePositions;
	
	glm::vec3 middleCirclePosition;
	float posX = startCirclePosition.x;
	float posZ = startCirclePosition.z;
	float posY = startCirclePosition.y;

	float rotateStep = 50 + rand() % 16;
	float fRotateAngle = rotateStep;
	bool changeRotation = true;
	
	glm::mat4 trans;
	trans = glm::rotate(trans, glm::radians(rotateStep), parentRotation);
	
	for (int l = 0; l < _lodLevels; l++) {
		std::vector<VertexFormat> vecLevelVertices;
		std::vector<int> vecLevelIndices;

		selectResolution(l);
		int verticesPerLevel = (2 * M_PI) / _resolution;

		float crtLevelDistance = branchLength / _noBranchCircles;
		float crtLevelRadius = branchSize;
		float nextLevelRadius = branchSize / 2;

		float smoothRadiusStep = (crtLevelRadius - nextLevelRadius) / _smoothCircles;
		float branchRadiusStep = (_maxBranchRadius - _minBranchRadius) / _maxBranchLevel;

		for (int i = 0; i < _noBranchCircles; i++) {

			if (i >= _noBranchCircles - _smoothCircles) {
				crtLevelRadius -= smoothRadiusStep;
			}

			glm::vec4 vecVertexCenter = trans * glm::vec4(0, 0 + crtLevelDistance * i, 0, 1);

			VertexFormat centerLevelVertex = VertexFormat(posX + vecVertexCenter.x, posY + vecVertexCenter.y, posZ + vecVertexCenter.z, 0, 1, 0, 0, 0);
			centerLevelVertex.branch_startX = posX;
			centerLevelVertex.branch_startY = posY;
			centerLevelVertex.branch_startZ = posZ;

			centerLevelVertex.branchCircle = i;
			centerLevelVertex.branchRadius = crtLevelRadius;
			centerLevelVertex.branchLevel = level;

			centerLevelVertex.branchVertex = 1.0f;
			centerLevelVertex.branchLength = branchLength;


			glm::vec3 vecCenterLevel = glm::vec3(centerLevelVertex.coordX, centerLevelVertex.coordY, centerLevelVertex.coordZ);
			vecCirclePositions.push_back(vecCenterLevel);

			if (level == _maxBranchLevel && i < _noBranchCircles - 1) {
				createLeaves(vecCenterLevel, branchLength, branchSize, rotateStep, i);
			}
			vecLevelVertices.push_back(centerLevelVertex);


			for (int j = 0; j < verticesPerLevel; j++) {
				float randOffsetX = 0, randOffsetY = 0, randOffsetZ = 0;
				int nLevelRadius = (int)crtLevelRadius;
				if (nLevelRadius < 1) nLevelRadius++;

				randOffsetX = (1 + (rand() % (int)nLevelRadius)) / _branchRandomness;
				randOffsetY = 0;
				randOffsetZ = (1 + (rand() % (int)nLevelRadius)) / _branchRandomness;

				float coordX = crtLevelRadius * cos(j) + randOffsetX;
				float coordZ = crtLevelRadius * sin(j) + randOffsetZ;
				float coordY = crtLevelDistance * i + randOffsetY;

				glm::vec4 vecVertex = trans * glm::vec4(coordX, coordY, coordZ, 1);

				VertexFormat tempVertex = VertexFormat(posX + vecVertex.x, posY + vecVertex.y, posZ + vecVertex.z, 0, 1, 0, 0, 0);

				tempVertex.branch_startX = posX; 
				tempVertex.branch_startY = posY;
				tempVertex.branch_startZ = posZ;

				tempVertex.branchCircle = i;
				tempVertex.branchRadius = crtLevelRadius;
				tempVertex.branchLevel = level;

				tempVertex.branchVertex = 1.0f;
				tempVertex.branchLength = branchLength;


				vecLevelVertices.push_back(tempVertex);
			}
		}

		for (int i = 0; i < _noBranchCircles; i++) {
			int crtCenterIndex = i * verticesPerLevel + i;
			for (int j = 1; j < verticesPerLevel - 1; j++) {
				vecLevelIndices.push_back(crtCenterIndex);
				vecLevelIndices.push_back(crtCenterIndex + j);
				vecLevelIndices.push_back(crtCenterIndex + j + 1);
			}

		}

		for (int i = 0; i < _noBranchCircles - 1; i++) {
			int level1CenterIndex = i * verticesPerLevel + i;
			int level2CenterIndex = (i + 1) * verticesPerLevel + (i + 1);
			for (int j = 1; j < verticesPerLevel; j += 2) {

				vecLevelIndices.push_back(level1CenterIndex + j); //11
				vecLevelIndices.push_back(level1CenterIndex + j + 1); //12
				vecLevelIndices.push_back(level2CenterIndex + j + 1); //22

				vecLevelIndices.push_back(level2CenterIndex + j + 1); //22
				vecLevelIndices.push_back(level2CenterIndex + j); //21
				vecLevelIndices.push_back(level1CenterIndex + j); //11
			}
		}

		for (int i = 0; i < _noBranchCircles; i += 2) {
			int level1CenterIndex = i * verticesPerLevel + i;
			int level2CenterIndex = (i + 1) * verticesPerLevel + (i + 1);
			for (int j = 1; j < verticesPerLevel; j += 2) {

				vecLevelVertices[level1CenterIndex + j].texcoord_x = 0;
				vecLevelVertices[level1CenterIndex + j].texcoord_y = 0;

				vecLevelVertices[level1CenterIndex + j + 1].texcoord_x = 1;
				vecLevelVertices[level1CenterIndex + j + 1].texcoord_y = 0;

				vecLevelVertices[level2CenterIndex + j + 1].texcoord_x = 1;
				vecLevelVertices[level2CenterIndex + j + 1].texcoord_y = 1;

				vecLevelVertices[level2CenterIndex + j].texcoord_x = 0;
				vecLevelVertices[level2CenterIndex + j].texcoord_y = 1;
			}
		}

		//vertex array object -> un obiect ce reprezinta un container pentru starea de desenare
		unsigned int vao, vbo, ibo;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		//vertex buffer object -> un obiect in care tinem vertecsii
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vecLevelVertices.size()*sizeof(VertexFormat), &vecLevelVertices[0], GL_STATIC_DRAW);

		//index buffer object -> un obiect in care tinem indecsii
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vecLevelIndices.size()*sizeof(unsigned int), &vecLevelIndices[0], GL_STATIC_DRAW);

		//legatura intre atributele vertecsilor si pipeline, datele noastre sunt INTERLEAVED.
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0);					// trimite pozitii pe pipe 0
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(float) * 3));	// trimite normale pe pipe 1
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(float) * 6));	// trimite texcoord pe pipe 2
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(float) * 8));	// trimite pozitia de start a crengii
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(float) * 11));	// trimite detalii creanga
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(float) * 14));	// trimite detalii creanga
		
		_branch_vao[l].push_back(vao);
		_branch_vbo[l].push_back(vbo);
		_branch_ibo[l].push_back(ibo);

		_branch_indices[l].push_back(vecLevelIndices.size());

		_noBranches[l] ++;
	}

	float nCircleLevel, newBranchSize, newBranchLength;
	float branchLengthStep = (_maxBranchLength - _minBranchLength) / (float) _maxBranchLevel;

	int nNewLevel;
	glm::vec3 vecNewRotation;

	if (level < _maxBranchLevel) {
		int nNewBranches = abs(rand() % (level + 2));
		if (level == _maxBranchLevel - 1) {
			nNewBranches = 2;
		}
		for (int i = 0; i < nNewBranches; i++) {
			nCircleLevel = abs(rand() % ((int)_noBranchCircles - 4)) + 1;
			vecNewRotation = getRotationVector(parentRotation);

			newBranchLength = branchLength - branchLengthStep;
			createBranch(vecCirclePositions[nCircleLevel], vecNewRotation, newBranchLength, branchSize / 2, level + 1);
		}
		
		newBranchLength = branchLength - branchLengthStep;
		vecNewRotation = getRotationVector(getOppositeRotation(parentRotation));

		createBranch(vecCirclePositions[_noBranchCircles - 1], vecNewRotation, newBranchLength, branchSize / 2, level + 1);

		vecNewRotation = getOppositeRotation(vecNewRotation);

		createBranch(vecCirclePositions[_noBranchCircles - 1], vecNewRotation, newBranchLength, branchSize / 2, level + 1);
	}
}

void Tree::createTree()
{
	std::vector<VertexFormat> vecLevelVertices;
	std::vector<int> vecLevelIndices;
	glm::vec3 topCircleCenter;

	int fTerrainWidth = _terrain->getWidth();
	int fTerrainHeight = _terrain->getHeight();

	int posX = rand() % (fTerrainWidth - 40) + 20;
	int posZ = rand() % (fTerrainHeight - 40) + 20;
	float posY = _terrain->getCoordHeight(posX, posZ) - 10;

	posX *= 40;
	posZ *= 40;

	posX += abs(rand() % (40));
	posZ += abs(rand() % (40));

	for (int l = 0; l < _lodLevels; l++) {
		vecLevelVertices.clear();
		vecLevelIndices.clear();

		selectResolution(0);

		int verticesPerLevel = (2 * M_PI) / _resolution;

		vecStartPosition = glm::vec3(posX, posY, posZ);
		float trunkRadius = _trunkRadius;
		float smoothTrunkStep = (_trunkRadius - _maxBranchRadius) / _smoothCircles;
		for (int i = 0; i < _noTrunkCircles; i++) {
			VertexFormat centerLevelVertex = VertexFormat(posX, posY + _trunkLevelHeight * i, posZ, 0, 1, 0, 0, 0);

			if (i == _noTrunkCircles - 1) {
				topCircleCenter = glm::vec3(posX, posY + _trunkLevelHeight *i, posZ);
			}

			if (i >= _noTrunkCircles - _smoothCircles) {
				trunkRadius -= smoothTrunkStep;
			}

			vecLevelVertices.push_back(centerLevelVertex);

			for (int j = 0; j < verticesPerLevel; j++) {
				int randOffsetX = 0, randOffsetY = 0, randOffsetZ = 0;

				randOffsetX = (rand() % (_noTrunkCircles - i)) * ((_noTrunkCircles - i) / 2);
				randOffsetY = rand() % (_trunkLevelHeight / 2);
				randOffsetZ = (rand() % (_noTrunkCircles - i)) * ((_noTrunkCircles - i) / 2);

				VertexFormat tempVertex = VertexFormat(posX + trunkRadius * cos(j) + randOffsetX,
					posY + _trunkLevelHeight * i + randOffsetY,
					posZ + trunkRadius * sin(j) + randOffsetZ,
					0, 1, 0, 0, 0);
				vecLevelVertices.push_back(tempVertex);
			}
		}

		for (int i = 0; i < _noTrunkCircles; i++) {
			int crtCenterIndex = i * verticesPerLevel + i;
			for (int j = 1; j < verticesPerLevel - 1; j++) {
				vecLevelIndices.push_back(crtCenterIndex);
				vecLevelIndices.push_back(crtCenterIndex + j);
				vecLevelIndices.push_back(crtCenterIndex + j + 1);
			}
		}

		for (int i = 0; i < _noTrunkCircles - 1; i++) {
			int level1CenterIndex = i * verticesPerLevel + i;
			int level2CenterIndex = (i + 1) * verticesPerLevel + (i + 1);
			for (int j = 1; j < verticesPerLevel; j += 2) {

				vecLevelIndices.push_back(level1CenterIndex + j); //11
				vecLevelIndices.push_back(level1CenterIndex + j + 1); //12
				vecLevelIndices.push_back(level2CenterIndex + j + 1); //22

				vecLevelIndices.push_back(level2CenterIndex + j + 1); //22
				vecLevelIndices.push_back(level2CenterIndex + j); //21
				vecLevelIndices.push_back(level1CenterIndex + j); //11
			}
		}

		for (int i = 0; i < _noTrunkCircles; i += 2) {
			int level1CenterIndex = i * verticesPerLevel + i;
			int level2CenterIndex = (i + 1) * verticesPerLevel + (i + 1);
			for (int j = 1; j < verticesPerLevel; j += 2) {

				vecLevelVertices[level1CenterIndex + j].texcoord_x = 0;
				vecLevelVertices[level1CenterIndex + j].texcoord_y = 0;

				vecLevelVertices[level1CenterIndex + j + 1].texcoord_x = 1;
				vecLevelVertices[level1CenterIndex + j + 1].texcoord_y = 0;

				vecLevelVertices[level2CenterIndex + j + 1].texcoord_x = 1;
				vecLevelVertices[level2CenterIndex + j + 1].texcoord_y = 1;

				vecLevelVertices[level2CenterIndex + j].texcoord_x = 0;
				vecLevelVertices[level2CenterIndex + j].texcoord_y = 1;
			}
		}

		// vertex array object -> un obiect ce reprezinta un container pentru starea de desenare
		unsigned int vao, vbo, ibo;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// vertex buffer object -> un obiect in care tinem vertecsii
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vecLevelVertices.size()*sizeof(VertexFormat), &vecLevelVertices[0], GL_STATIC_DRAW);

		// index buffer object -> un obiect in care tinem indecsii
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vecLevelIndices.size()*sizeof(unsigned int), &vecLevelIndices[0], GL_STATIC_DRAW);

		// legatura intre atributele vertecsilor si pipeline, datele noastre sunt INTERLEAVED.
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0);						// trimite pozitii pe pipe 0
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(float) * 3));		// trimite normale pe pipe 1
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(float) * 3));	// trimite texcoord pe pipe 2

		_tree_vao[l] = vao;
		_tree_vbo[l] = vbo;
		_tree_ibo[l] = ibo;
		_tree_indices[l] = vecLevelIndices.size();
	}

	int nBranches = rand() % 2 + 2;

	 for (int i = 0; i < nBranches; i++) {

		int nRotationX = rand() % 3 - 1;
		int nRotationY = rand() % 3 - 1;
		int nRotationZ = rand() % 3 - 1;
	
		int nRotation = rand() % 6;
		createBranch(topCircleCenter, glm::vec3(nRotationX, nRotationY, nRotationZ), _maxBranchLength, _maxBranchRadius, 1);
	 }
}

glm::vec3 Tree::getStartPosition()
{
	return vecStartPosition;
}

int Tree::selectTreeLod(glm::vec3 eyePosition, glm::vec3 treePosition) {
	float distance = glm::distance(eyePosition, treePosition);
	//std::cout << distance << std::endl;
	if (distance < 1000) {
		return FULLLOD;
	}
	else if (distance < 2000) {
		return PARTIALLOD;
	}
	else if (distance < 2500) {
		return LOWLOD;
	}
	else if (distance < 3000) {
		return VERYLOWLOD;
	}
	else {
		return INVISIBLE;
	}
}