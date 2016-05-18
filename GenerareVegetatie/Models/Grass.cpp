/* Alexandru-Catalin Ciobanu 341C4*/
#include "Grass.h"

Grass::Grass()
{

}

Grass::Grass(int noGrassCount, float grassWidth, float grassHeight, int coordX, int coordZ, int radius, Terrain * terrain)
{
	_noGrassCount = noGrassCount;
	_noGrass = 0;
	_grassWidth = grassWidth;
	_grassHeight = grassHeight;
	_grassRadius = radius;
	_terrain = terrain;
	_coordX = coordX;
	_coordZ = coordZ;
	_coordY = _terrain->getCoordHeight(_coordX / 40, _coordZ / 40);
	_terrainScaleFactor = 40;

	_noGrassIndices = std::vector<int>(_noLODLevels);
	for (int i = 0; i < _noLODLevels; i++) {
		_noGrassIndices[i] = 0;
		_grass_vao.push_back(0);
		_grass_vbo.push_back(0);
		_grass_ibo.push_back(0);
		_grass_indices.push_back(0);
	}

	_startPosition = glm::vec3(_coordX, _coordY, _coordZ);
	createGrassBlade();
}

void Grass::computePartialBladeIndices(std::vector<unsigned int> &indices, int lodLevel) {
	
	int lodIndices = _noGrassIndices[lodLevel];

	switch (lodLevel) {
		case 0:
			
			indices.push_back(lodIndices + 0); indices.push_back(lodIndices + 1); indices.push_back(lodIndices + 2);

			indices.push_back(lodIndices + 1); indices.push_back(lodIndices + 3); indices.push_back(lodIndices + 2);

			indices.push_back(lodIndices + 2); indices.push_back(lodIndices + 3); indices.push_back(lodIndices + 4);

			indices.push_back(lodIndices + 3); indices.push_back(lodIndices + 5); indices.push_back(lodIndices + 4);

			indices.push_back(lodIndices + 4); indices.push_back(lodIndices + 5); indices.push_back(lodIndices + 6);
			_noGrassIndices[lodLevel] += 7;
			break; 

		case 1:
			indices.push_back(lodIndices + 0); indices.push_back(lodIndices + 1); indices.push_back(lodIndices + 2);

			indices.push_back(lodIndices + 1); indices.push_back(lodIndices + 3); indices.push_back(lodIndices + 2);

			indices.push_back(lodIndices + 2); indices.push_back(lodIndices + 3); indices.push_back(lodIndices + 4);
			_noGrassIndices[lodLevel] += 5;
			break;
		
		case 2:

			indices.push_back(lodIndices + 0); indices.push_back(lodIndices + 1); indices.push_back(lodIndices + 2);
			_noGrassIndices[lodLevel] += 3;
	}
}

void Grass::computeFullBladeIndices(std::vector<unsigned int> &indices, int lodLevel) {

	int lodIndices = _noGrassIndices[lodLevel];

	switch (lodLevel) {
	case 0:

		indices.push_back(lodIndices + 0); indices.push_back(lodIndices + 1); indices.push_back(lodIndices + 2);

		indices.push_back(lodIndices + 1); indices.push_back(lodIndices + 3); indices.push_back(lodIndices + 2);

		indices.push_back(lodIndices + 2); indices.push_back(lodIndices + 3); indices.push_back(lodIndices + 4);

		indices.push_back(lodIndices + 3); indices.push_back(lodIndices + 5); indices.push_back(lodIndices + 4);

		indices.push_back(lodIndices + 4); indices.push_back(lodIndices + 5); indices.push_back(lodIndices + 6);
		_noGrassIndices[lodLevel] += 7;
		break;

	case 1:
		indices.push_back(lodIndices + 0); indices.push_back(lodIndices + 1); indices.push_back(lodIndices + 2);

		indices.push_back(lodIndices + 1); indices.push_back(lodIndices + 3); indices.push_back(lodIndices + 2);

		indices.push_back(lodIndices + 2); indices.push_back(lodIndices + 3); indices.push_back(lodIndices + 4);
		_noGrassIndices[lodLevel] += 5;
		break;

	case 2:

		indices.push_back(lodIndices + 0); indices.push_back(lodIndices + 1); indices.push_back(lodIndices + 2);
		_noGrassIndices[lodLevel] += 3;
	}
}

void Grass::createPartialBlade(glm::vec3 bladePosition, 
	std::vector<VertexFormat> &vertices, int lod)
{
	glm::vec3 svPosition = bladePosition;
	float fOffsetX1 = _grassWidth * 3.0f / 5.0f;
	float fOffsetX2 = _grassWidth * 3.0f / 2.0f;
	float fOffsetX3 = 3.5f * _grassWidth;

	float fHDist = _grassWidth;

	float fHeight1 = _grassHeight * 2.0f / 3.0f;
	float fHeight2 = _grassHeight;
	float fHeight3 = _grassHeight;

	glm::vec4 vertexPosition;
	glm::vec3 up = glm::vec3(0, 1, 0);

	float rotateAngle = rand() % 360;
	glm::mat4 trans;
	glm::vec4 vRotation = glm::vec4(0, 0, 0, 0);

	trans = glm::rotate(trans, glm::radians(rotateAngle), up);
	float randMovement = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2;
	if (lod < 1) {
		// 0
		vertexPosition = glm::vec4(-fHDist * 2, fHeight1, 0, 0);
		vRotation = trans * vertexPosition;
		vertexPosition += glm::vec4(vRotation.x, vRotation.y, vRotation.z, 0) + glm::vec4(svPosition, 0);

		float randGreen = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 100.0f + 40.0f) / 255.0f);
		vertices.push_back(VertexFormat(vertexPosition.x, vertexPosition.y, vertexPosition.z, 0, randGreen, 0, 1, randMovement));
		// 1 
		vertexPosition = glm::vec4(0, fHeight1 * 2.0f / 3.0f, 0, 0);
		vRotation = trans * vertexPosition;
		vertexPosition += glm::vec4(vRotation.x, vRotation.y, vRotation.z, 0) + glm::vec4(svPosition, 0);

		randGreen = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 100.0f + 40.0f) / 255.0f);
		vertices.push_back(VertexFormat(vertexPosition.x, vertexPosition.y, vertexPosition.z, 0, randGreen, 0, 1, randMovement));
	}

	if (lod < 2) {
		// 2
		vertexPosition = glm::vec4(0, 0, 0, 0);
		vRotation = trans * vertexPosition;
		vertexPosition += glm::vec4(vRotation.x, vRotation.y, vRotation.z, 0) + glm::vec4(svPosition, 0);

		float randGreen = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 100.0f + 40.0f) / 255.0f);
		vertices.push_back(VertexFormat(vertexPosition.x, vertexPosition.y, vertexPosition.z, 0, randGreen, 0, 0, randMovement));

		// 3 
		vertexPosition = glm::vec4(fOffsetX1, 0, 0, 0);
		vRotation = trans * vertexPosition;
		vertexPosition += glm::vec4(vRotation.x, vRotation.y, vRotation.z, 0) + glm::vec4(svPosition, 0);

		randGreen = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 100.0f + 40.0f) / 255.0f);
		vertices.push_back(VertexFormat(vertexPosition.x, vertexPosition.y, vertexPosition.z, 0, randGreen, 0, 0, randMovement));
	}
	if (lod < 3) {
		// 4 
		vertexPosition = glm::vec4(fOffsetX1, fHeight1, 0, 0);
		vRotation = trans * vertexPosition;
		vertexPosition += glm::vec4(vRotation.x, vRotation.y, vRotation.z, 0) + glm::vec4(svPosition, 0);

		float randGreen = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 100.0f + 40.0f) / 255.0f);
		vertices.push_back(VertexFormat(vertexPosition.x, vertexPosition.y, vertexPosition.z, 0, randGreen, 0, 1, randMovement));
		// 5
		vertexPosition = glm::vec4(fOffsetX1 + fHDist, fHeight1, 0, 0);
		vRotation = trans * vertexPosition;
		vertexPosition += glm::vec4(vRotation.x, vRotation.y, vRotation.z, 0) + glm::vec4(svPosition, 0);

		randGreen = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 100.0f + 40.0f) / 255.0f);
		vertices.push_back(VertexFormat(vertexPosition.x, vertexPosition.y, vertexPosition.z, 0, randGreen, 0, 1, randMovement));
		// 6
		vertexPosition = glm::vec4(fOffsetX2 + fHDist, fHeight2, 0, 0);
		vRotation = trans * vertexPosition;
		vertexPosition += glm::vec4(vRotation.x, vRotation.y, vRotation.z, 0) + glm::vec4(svPosition, 0);

		randGreen = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 100.0f + 40.0f) / 255.0f);
		vertices.push_back(VertexFormat(vertexPosition.x, vertexPosition.y, vertexPosition.z, 0, randGreen, 0, 1, randMovement));
	}
}

void Grass::createFullBlade(glm::vec3 bladePosition, 
	std::vector<VertexFormat> &vertices, int lod)
{
	glm::vec3 svPosition = bladePosition;
	float fOffsetX1 = _grassWidth * 3.0f / 5.0f;
	float fOffsetX2 = _grassWidth * 3.0f / 2.0f;
	float fOffsetX3 = 3.5f * _grassWidth;

	float fHDist = _grassWidth;

	float fHeight1 = _grassHeight / 3.0f;
	float fHeight2 = _grassHeight * 2.0f / 3.0f;
	float fHeight3 = _grassHeight;

	glm::vec4 vertexPosition;
	glm::vec3 up = glm::vec3(0, 1, 0);

	float rotateAngle = rand() % 360;
	glm::mat4 trans;
	glm::vec4 vRotation = glm::vec4(0,0,0,0);

	trans = glm::rotate(trans, glm::radians(rotateAngle), up);
	float randMovement = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2;
	if (lod < 1) {
		// 0
		vertexPosition = glm::vec4(0, 0, 0, 0);
		vRotation = trans * vertexPosition;
		vertexPosition += glm::vec4(vRotation.x, vRotation.y, vRotation.z, 0) + glm::vec4(svPosition, 0);

		float randGreen = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 100.0f + 40.0f) / 255.0f);
		
		vertices.push_back(VertexFormat(vertexPosition.x, vertexPosition.y, vertexPosition.z, 0, randGreen, 0, 0, randMovement));
	
		// 1 
		vertexPosition = glm::vec4(fHDist, 0, 0, 0);
		vRotation = trans * vertexPosition;
		vertexPosition += glm::vec4(vRotation.x, vRotation.y, vRotation.z, 0) + glm::vec4(svPosition, 0);

		randGreen = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 100.0f + 40.0f) / 255.0f);
		vertices.push_back(VertexFormat(vertexPosition.x, vertexPosition.y, vertexPosition.z, 0, randGreen, 0, 0, randMovement));
		
	}
	if (lod < 2) {
		// 2
		vertexPosition = glm::vec4(fOffsetX1, fHeight1, 0, 0);
		vRotation = trans * vertexPosition;
		vertexPosition += glm::vec4(vRotation.x, vRotation.y, vRotation.z, 0) + glm::vec4(svPosition, 0);

		float randGreen = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 100.0f + 40.0f) / 255.0f);
		vertices.push_back(VertexFormat(vertexPosition.x, vertexPosition.y, vertexPosition.z, 0, randGreen, 0, 1, randMovement));
		

		// 3 
		vertexPosition = glm::vec4(fOffsetX1 + fHDist, fHeight1, 0, 0);
		vRotation = trans * vertexPosition;
		vertexPosition += glm::vec4(vRotation.x, vRotation.y, vRotation.z, 0) + glm::vec4(svPosition, 0);

		randGreen = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 100.0f + 40.0f) / 255.0f);
		vertices.push_back(VertexFormat(vertexPosition.x, vertexPosition.y, vertexPosition.z, 0, randGreen, 0, 1, randMovement));
	}
	if (lod < 3) {
		// 4 
		vertexPosition = glm::vec4(fOffsetX2, fHeight2, 0, 0);
		vRotation = trans * vertexPosition;
		vertexPosition += glm::vec4(vRotation.x, vRotation.y, vRotation.z, 0) + glm::vec4(svPosition, 0);

		float randGreen = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 100.0f + 40.0f) / 255.0f);
		vertices.push_back(VertexFormat(vertexPosition.x, vertexPosition.y, vertexPosition.z, 0, randGreen, 0, 1, randMovement));


		// 5
		vertexPosition = glm::vec4(fOffsetX2 + fHDist, fHeight2, 0, 0);
		vRotation = trans * vertexPosition;
		vertexPosition += glm::vec4(vRotation.x, vRotation.y, vRotation.z, 0) + glm::vec4(svPosition, 0);

		randGreen = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 100.0f + 40.0f) / 255.0f);
		vertices.push_back(VertexFormat(vertexPosition.x, vertexPosition.y, vertexPosition.z, 0, randGreen, 0, 1, randMovement));

		// 6
		vertexPosition = glm::vec4(fOffsetX3, fHeight3, 0, 0);
		vRotation = trans * vertexPosition;
		vertexPosition += glm::vec4(vRotation.x, vRotation.y, vRotation.z, 0) + glm::vec4(svPosition, 0);

		randGreen = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 100.0f + 40.0f) / 255.0f);
		vertices.push_back(VertexFormat(vertexPosition.x, vertexPosition.y, vertexPosition.z, 0, randGreen, 0, 1, randMovement));
	}
}

void Grass::createGrassBlade()
{
	std::vector<std::vector<VertexFormat> > vertices;
	std::vector<std::vector<unsigned int> > indices;

	for (int i = 0; i < _noLODLevels; i++) {
		vertices.push_back(std::vector<VertexFormat>());
		indices.push_back(std::vector<unsigned int>());
	}
	int zoneDimension = _terrain->getWidth();
	int terrainX = zoneDimension * _terrainScaleFactor;
	int terrainZ = zoneDimension * _terrainScaleFactor;

	for (int i = 0; i < _noLODLevels; i++) {
		for (int j = 0; j < _noGrassCount; j++)
		{
			float fOffsetX = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 1000;
			float fOffsetZ = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 1000;
			
			while (_coordX + fOffsetX > terrainX - _terrainScaleFactor) {
				 fOffsetX = static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 1000;
			}

			while (_coordZ + fOffsetZ > terrainZ - _terrainScaleFactor) {
				fOffsetZ = static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 1000;
			}

			float fCoordX = _coordX + fOffsetX;
			float fCoordZ = _coordZ + fOffsetZ;

			float fCoordY = _terrain->getCoordHeight(fCoordX / 40, fCoordZ / 40);

			if (j % 3 == 0) {
				createFullBlade(glm::vec3(fCoordX, fCoordY, fCoordZ), vertices[i], i);
				computeFullBladeIndices(indices[i], i);
			}
			else {
				createPartialBlade(glm::vec3(fCoordX, fCoordY, fCoordZ), vertices[i], i);
				computePartialBladeIndices(indices[i], i);
			}

		}
		
		unsigned int gl_vertex_array_object, gl_vertex_buffer_object, gl_index_buffer_object;

		glGenVertexArrays(1, &gl_vertex_array_object);
		glBindVertexArray(gl_vertex_array_object);
		
		glGenBuffers(1, &gl_vertex_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer_object);
		glBufferData(GL_ARRAY_BUFFER, vertices[i].size()*sizeof(VertexFormat), &vertices[i][0], GL_STATIC_DRAW);

		glGenBuffers(1, &gl_index_buffer_object);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_index_buffer_object);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices[i].size()*sizeof(unsigned int), &indices[i][0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0);					
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(float) * 6));

		_grass_vao[i] = gl_vertex_array_object;
		_grass_vbo[i] = gl_vertex_buffer_object;
		_grass_ibo[i] = gl_index_buffer_object;
		_grass_indices[i] = indices[i].size();

	}
}

void Grass::createHighDetailGrass()
{
	std::vector<VertexFormat> vertices;

	glm::mat4 matRotateY45, matRotateY45I, matTranslate;

	std::vector<unsigned int> indices;

	glm::mat4 trans;
	int h = 0;
	int fTerrainWidth = _terrain->getWidth();
	int fTerrainHeight = _terrain->getHeight();

	float noGrassPerTile = _noGrassCount / ( fTerrainWidth * fTerrainHeight);
	float crtGrassPieces = 0;

	for (size_t i = 0; i < _noGrassCount * 4; i += 4)
	{
		int coordX = rand() % fTerrainWidth;
		int coordZ = rand() % fTerrainHeight;
		int coordY = _terrain->getCoordHeight(coordX, coordZ);

		coordX *=  20 * 3;
		coordZ *=  20 * 3;

		float fCoordX = static_cast <float> (coordX) + static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 20.0f * 3.0f;
		float fCoordZ = static_cast <float> (coordZ) + static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 20.0f * 3.0f;

		h++;

		float rotateStep = rand() % 360;
		trans = glm::rotate(trans, glm::radians(rotateStep), glm::vec3(0, 1, 0));


		/**************************************************************************************************
		************************************* FIRST BLADES OF GRASS **************************************/

		// first grass object
		glm::vec4 vRotation = trans * glm::vec4(0, 0, 0, 0);
		VertexFormat vertex1 = VertexFormat(fCoordX + vRotation.x, coordY + vRotation.y, fCoordZ + vRotation.z, 0, 1, 0, 1, 1);

		vRotation = trans * glm::vec4(_nGrassSize, 0, 0, 0);
		VertexFormat vertex2 = VertexFormat(fCoordX + vRotation.x + _nGrassSize, coordY + vRotation.y, fCoordZ + vRotation.z, 0, 1, 0, 0, 1);

		vRotation = trans * glm::vec4(_nGrassSize, _nGrassSize, 0, 0);
		VertexFormat vertex3 = VertexFormat(fCoordX + vRotation.x + _nGrassSize, coordY + vRotation.y + _nGrassSize, fCoordZ + vRotation.z, 0, 1, 0, 0, 0);

		vRotation = trans * glm::vec4(0, _nGrassSize, 0, 0);
		VertexFormat vertex4 = VertexFormat(fCoordX + vRotation.x, coordY + _nGrassSize + vRotation.y, fCoordZ + vRotation.z, 0, 1, 0, 1, 0);

		/********************************************************************************************************
		****************************************** BUILD MESH **************************************************/
		vertices.push_back(vertex1);  vertices.push_back(vertex2); vertices.push_back(vertex3); vertices.push_back(vertex4);

		indices.push_back(i + 0); indices.push_back(i + 1); indices.push_back(i + 2);
		indices.push_back(i + 2); indices.push_back(i + 3);  indices.push_back(i + 0);
	}
	
	//creeaza obiectele OpenGL necesare desenarii
	unsigned int gl_vertex_array_object, gl_vertex_buffer_object, gl_index_buffer_object;

	//vertex array object -> un obiect ce reprezinta un container pentru starea de desenare
	glGenVertexArrays(1, &gl_vertex_array_object);
	glBindVertexArray(gl_vertex_array_object);

	//vertex buffer object -> un obiect in care tinem vertecsii
	glGenBuffers(1, &gl_vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(VertexFormat), &vertices[0], GL_STATIC_DRAW);

	//index buffer object -> un obiect in care tinem indecsii
	glGenBuffers(1, &gl_index_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_index_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//legatura intre atributele vertecsilor si pipeline, datele noastre sunt INTERLEAVED.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0);					// trimite pozitii pe pipe 0
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(float) * 3));	// trimite normale pe pipe 1
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(float) * 6));	// trimite texcoords pe pipe 2

	/*
	_vao[ = gl_vertex_array_object;
	_vbo = gl_vertex_buffer_object;
	_ibo = gl_index_buffer_object;
	_indicesCount = indices.size();
	*/
}

void Grass::createGrass()
{
	std::vector<VertexFormat> vertices;
	
	float fCenterX;
	float fCenterY;
	float fCenterZ;
	float fRotateAngle = M_PI / 4;

	glm::mat4 matRotateY45, matRotateY45I, matTranslate;

	std::vector<unsigned int> indices;

	std::vector<std::vector<float> > matGrassObject;

	matRotateY45 = glm::mat4(glm::vec4(std::cos(fRotateAngle), 0.0f, - std::sin(fRotateAngle), 0.0f),
							glm::vec4(0.0f, 1, 0.0f, 0.0f),					
							glm::vec4(-std::sin(fRotateAngle), 0, std::cos(fRotateAngle), 0.0f),
							glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	matRotateY45I = glm::mat4(glm::vec4(std::cos(-fRotateAngle), 0.0f, -std::sin(-fRotateAngle), 0.0f),
							glm::vec4(0.0f, 1, 0.0f, 0.0f),
							glm::vec4(-std::sin(-fRotateAngle), 0, std::cos(-fRotateAngle), 0.0f),
							glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	int h = 0;
	for (size_t i = 0; i < 3 *_noGrassCount * 4; i+= 12)
	{
		float fX = _vecPositions[h].x;
		float fY = _vecPositions[h].y;
		float fZ = _vecPositions[h].z;
		h++;
		fCenterX = fCenterY = fCenterZ = _nGrassSize / 2;


		/**************************************************************************************************
		 ************************************* FIRST BLADES OF GRASS **************************************/
		
		// first grass object
		VertexFormat vertex11 = VertexFormat(fX, fY, fZ, 0, 1, 0, 1, 1);
		VertexFormat vertex12 = VertexFormat(fX + _nGrassSize, fY, fZ, 0, 1, 0, 0, 1);
		VertexFormat vertex13 = VertexFormat(fX + _nGrassSize, fY + _nGrassSize, fZ, 0, 1, 0, 0, 0);
		VertexFormat vertex14 = VertexFormat(fX, fY + _nGrassSize, fZ, 0, 1, 0, 1, 0);
		
		/**************************************************************************************************
		************************************* SECOND BLADES OF GRASS **************************************/

		// second grass object initial position
		glm::vec4 vecVertex145 = glm::vec4(-fCenterX,				-fCenterY,				0, 1);
		glm::vec4 vecVertex245 = glm::vec4(_nGrassSize - fCenterX,	-fCenterY,				0, 1);
		glm::vec4 vecVertex345 = glm::vec4(_nGrassSize - fCenterX,	_nGrassSize - fCenterY,	0, 1);
		glm::vec4 vecVertex445 = glm::vec4(-fCenterX,				_nGrassSize - fCenterY,	0, 1);
				
		// rotate second grass object 45 degrees
		vecVertex145 = vecVertex145 * matRotateY45;
		vecVertex245 = vecVertex245 * matRotateY45;
		vecVertex345 = vecVertex345 * matRotateY45;
		vecVertex445 = vecVertex445 * matRotateY45;

		// second grass object
		VertexFormat vertex21 = VertexFormat(fX + vecVertex145.x + fCenterX, fY + vecVertex145.y + fCenterY, fZ + vecVertex145.z, 0, 1, 0, 1, 1);
		VertexFormat vertex22 = VertexFormat(fX + vecVertex245.x + fCenterX, fY + vecVertex245.y + fCenterY, fZ + vecVertex245.z, 0, 1, 0, 0, 1);
		VertexFormat vertex23 = VertexFormat(fX + vecVertex345.x + fCenterX, fY + vecVertex345.y + fCenterY, fZ + vecVertex345.z, 0, 1, 0, 0, 0);
		VertexFormat vertex24 = VertexFormat(fX + vecVertex445.x + fCenterX, fY + vecVertex445.y + fCenterY, fZ + vecVertex445.z, 0, 1, 0, 1, 0);


		/**************************************************************************************************
		************************************* THIRD BLADES OF GRASS ***************************************/

		// third grass object initial position
		glm::vec4 vecVertex145I = glm::vec4(-fCenterX,				-fCenterY,				0, 1);
		glm::vec4 vecVertex245I = glm::vec4(_nGrassSize - fCenterX,	-fCenterY,				0, 1);
		glm::vec4 vecVertex345I = glm::vec4(_nGrassSize - fCenterX,	_nGrassSize - fCenterY,	0, 1);
		glm::vec4 vecVertex445I = glm::vec4(-fCenterX,				_nGrassSize - fCenterY,	0, 1);

		// rotate third grass object -45 degrees
		vecVertex145I = vecVertex145I * matRotateY45I;
		vecVertex245I = vecVertex245I * matRotateY45I;
		vecVertex345I = vecVertex345I * matRotateY45I;
		vecVertex445I = vecVertex445I * matRotateY45I;

		// third grass object
		VertexFormat vertex31 = VertexFormat(fX + vecVertex145I.x + fCenterX, fY + vecVertex145I.y + fCenterY, fZ + vecVertex145I.z, 0, 1, 0, 1, 1);
		VertexFormat vertex32 = VertexFormat(fX + vecVertex245I.x + fCenterX, fY + vecVertex245I.y + fCenterY, fZ + vecVertex245I.z, 0, 1, 0, 0, 1);
		VertexFormat vertex33 = VertexFormat(fX + vecVertex345I.x + fCenterX, fY + vecVertex345I.y + fCenterY, fZ + vecVertex345I.z, 0, 1, 0, 0, 0);
		VertexFormat vertex34 = VertexFormat(fX + vecVertex445I.x + fCenterX, fY + vecVertex445I.y + fCenterY, fZ + vecVertex445I.z, 0, 1, 0, 1, 0);

		/********************************************************************************************************
		 ****************************************** BUILD MESH **************************************************/
		vertices.push_back(vertex11);  vertices.push_back(vertex12); vertices.push_back(vertex13); vertices.push_back(vertex14);

		indices.push_back(i + 0); indices.push_back(i + 1); indices.push_back(i + 2);
		indices.push_back(i + 2); indices.push_back(i + 3);  indices.push_back(i + 0);

		vertices.push_back(vertex21);  vertices.push_back(vertex22); vertices.push_back(vertex23); vertices.push_back(vertex24);

		indices.push_back(i + 4); indices.push_back(i + 5); indices.push_back(i + 6);
		indices.push_back(i + 6); indices.push_back(i + 7);  indices.push_back(i + 4);

		vertices.push_back(vertex31);  vertices.push_back(vertex32); vertices.push_back(vertex33); vertices.push_back(vertex34);

		indices.push_back(i + 8); indices.push_back(i + 9); indices.push_back(i + 10);
		indices.push_back(i + 10); indices.push_back(i + 11);  indices.push_back(i + 8);

	}
	//creeaza obiectele OpenGL necesare desenarii
	unsigned int gl_vertex_array_object, gl_vertex_buffer_object, gl_index_buffer_object;

	//vertex array object -> un obiect ce reprezinta un container pentru starea de desenare
	glGenVertexArrays(1, &gl_vertex_array_object);
	glBindVertexArray(gl_vertex_array_object);

	//vertex buffer object -> un obiect in care tinem vertecsii
	glGenBuffers(1, &gl_vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(VertexFormat), &vertices[0], GL_STATIC_DRAW);

	//index buffer object -> un obiect in care tinem indecsii
	glGenBuffers(1, &gl_index_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_index_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//legatura intre atributele vertecsilor si pipeline, datele noastre sunt INTERLEAVED.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0);					// trimite pozitii pe pipe 0
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(float) * 3));	// trimite normale pe pipe 1
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(float) * 6));	// trimite texcoords pe pipe 2

	/*
	_vao = gl_vertex_array_object;
	_vbo = gl_vertex_buffer_object;
	_ibo = gl_index_buffer_object;
	_indicesCount = indices.size();
	*/
}

void Grass::drawGrass(int lodLevel)
{
	if (lodLevel != INVISIBLE) {
		glBindVertexArray(_grass_vao[lodLevel]);
		glDrawElements(GL_TRIANGLES, _grass_indices[lodLevel], GL_UNSIGNED_INT, 0);
	}

}

glm::vec3 Grass::getStartPosition()
{
	return _startPosition;
}

Grass::~Grass()
{
}

int Grass::selectGrassLod(glm::vec3 eyePosition, glm::vec3 treePosition, float grassRadius) {
	float distance = glm::distance(eyePosition, treePosition);
	//std::cout << distance << std::endl;
	if (distance < grassRadius) {
		return FULLLOD;
	}
	else if (distance < grassRadius * 5) {
		return PARTIALLOD;
	}
	else if (distance < grassRadius * 6) {
		return LOWLOD;
	}
	else {
		return INVISIBLE;
	}
}
