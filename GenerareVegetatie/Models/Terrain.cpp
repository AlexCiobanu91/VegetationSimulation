/* Alexandru-Catalin Ciobanu 341C4*/
#include "Terrain.h"
#include "..\Core\Image_Loader.h"
#include <algorithm>

Terrain::Terrain()
{
}

Terrain::Terrain(const std::string &filename,
	const std::string &filename2, int height) {

	loadTerrain(filename, filename2, height);
}

Terrain::~Terrain()
{
}

void Terrain::readTerrain(const std::string &filename,
	const std::string &filename2,
	int height)
{

	Image* image = loadBMP(filename.c_str());

	Image* color = loadBMP(filename2.c_str());

	_nWidth = image->width;
	_nHeight = image->height;

	char * arrImagePixels = image->pixels;
	char * arrColorPixels = color->pixels;

	for (int i = 0; i < _nHeight; i++) {
		for (int j = 0; j < _nWidth; j++) {
			float maxim, minim;
			float gr, gg, gb;	

			gr = ((unsigned char)arrImagePixels[3 * (i * _nWidth + j)]) / 255.0f;
			gg = ((unsigned char)arrImagePixels[3 * (i * _nWidth + j) + 1]) / 255.0f;
			gb = ((unsigned char)arrImagePixels[3 * (i * _nWidth + j) + 2]) / 255.0f;

			maxim = std::max(std::max(gr, gg), gb);
			minim = std::min(std::min(gr, gg), gb);

			float lighting = (maxim + minim) / 2.0;

			float hs = height * lighting;
			_vecVertices.push_back(VertexFormat(i, hs, j, 0, 1, 0, 0, 0));
			_vecHeights.push_back(hs);
		}
	}

	int texWidth = color->width;
	int texHeight = color->height;
	int crtVertex = 0;
	for (int i = 0; i < _nHeight; i++) {
		for (int j = 0; j < _nWidth; j++) {
			crtVertex = _nHeight * i + j;
			_vecVertices[crtVertex].texcoord_x = i % _nHeight;
			_vecVertices[crtVertex].texcoord_y = j % _nWidth;
		}
	}

	for (int i = 0; i < _vecVertices.size(); i++) {
		_vecVertices[i].coordX *= _terrainScale;
		_vecVertices[i].coordY *= 1;
		_vecVertices[i].coordZ *= _terrainScale;
		_vecHeights[i] = _vecVertices[i].coordY;
	}

	computeNormals(0.5);

	for (int row = 0; row < _nHeight - 1; row++) {
		for (int col = 0; col < _nWidth - 1; col++) {
			int i0 = row * _nWidth + col;
			int i1 = i0 + _nHeight;

			_vecIndices.push_back(i0);
			_vecIndices.push_back(i0 + 1);
			_vecIndices.push_back(i1);

			_vecIndices.push_back(i1);
			_vecIndices.push_back(i1 + 1);
			_vecIndices.push_back(i0 + 1);
		}
	}

}

void Terrain::computeNormals(float level)
{

	glm::vec3** normals2 = new glm::vec3*[_nHeight];
	for (int i = 0; i < _nHeight; i++) {
		normals2[i] = new glm::vec3[_nWidth];
	}

	for (int z = 0; z < _nHeight; z++) {
		for (int x = 0; x < _nWidth; x++) {
			glm::vec3 sum(0.0f, 0.0f, 0.0f);

			glm::vec3 out;
			if (z > 0) {
				float fCoordY = _vecVertices[_nWidth * (z - 1) + x].coordY - _vecVertices[_nWidth * z + x].coordY;
				out = glm::vec3(0.0f, fCoordY, -1.0f);
			}

			glm::vec3 in;
			if (z < _nHeight - 1) {
				float fCoordY = _vecVertices[_nWidth * (z + 1) + x].coordY - _vecVertices[_nWidth * z + x].coordY;
				in = glm::vec3(0.0f, fCoordY, 1.0f);
			}

			glm::vec3 left;
			if (x > 0) {
				float fCoordY = _vecVertices[_nWidth * z + x - 1].coordY - _vecVertices[_nWidth * z + x].coordY;
				left = glm::vec3(-1.0f, fCoordY, 0.0f);
			}

			glm::vec3 right;
			if (x < _nWidth - 1) {
				float fCoordY = _vecVertices[_nWidth * z + x + 1].coordY - _vecVertices[_nWidth * z + x].coordY;
				right = glm::vec3(1.0f, fCoordY, 0.0f);
			}

			if (x > 0 && z > 0) {
				sum += glm::normalize(glm::cross(out, left));

			}
			if (x > 0 && z < _nHeight - 1) {
				sum += glm::normalize(glm::cross(left, in));
			}
			if (x < _nWidth - 1 && z < _nHeight - 1) {
				sum += glm::normalize(glm::cross(in, right));
			}
			if (x < _nWidth - 1 && z > 0) {
				sum += glm::normalize(glm::cross(right, out));
			}

			normals2[z][x] = sum;
		}
	}

	const float FALLOUT_RATIO = level;
	for (int i = 0; i < _nHeight; i++) {
		for (int j = 0; j < _nWidth; j++) {
			glm::vec3 sum = normals2[i][j];

			if (j > 0) {
				sum += normals2[i][j - 1] * FALLOUT_RATIO;
			}
			if (j < _nWidth - 1) {
				sum += normals2[i][j + 1] * FALLOUT_RATIO;
			}
			if (i > 0) {
				sum += normals2[i - 1][j] * FALLOUT_RATIO;
			}
			if (i < _nHeight - 1) {
				sum += normals2[i + 1][j] * FALLOUT_RATIO;
			}

			if (sqrt(sum.x*sum.x + sum.y*sum.y + sum.z*sum.z) == 0) {
				sum = glm::vec3(0.0f, 1.0f, 0.0f);
			}

			_vecVertices[i * _nWidth + j].normalX = sum.x;
			_vecVertices[i * _nWidth + j].normalY = sum.y;
			_vecVertices[i * _nWidth + j].normalZ = sum.z;
		}
	}
}

void Terrain::loadTerrain(const std::string &filename,
	const std::string &filename2,
	int height)
{
	readTerrain(filename, filename2, height);

	unsigned int gl_vertex_array_object, gl_vertex_buffer_object, gl_index_buffer_object;

	glGenVertexArrays(1, &gl_vertex_array_object);
	glBindVertexArray(gl_vertex_array_object);

	glGenBuffers(1, &gl_vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, _vecVertices.size()*sizeof(VertexFormat), &_vecVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &gl_index_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_index_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _vecIndices.size()*sizeof(unsigned int), &_vecIndices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0);						
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(float) * 3));		
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(float) * 3));	

	_nVao = gl_vertex_array_object;
	_nVbo = gl_vertex_buffer_object;
	_nIbo = gl_index_buffer_object;
	_nIndices = _vecIndices.size();
}

float Terrain::getCoordHeight(int nCoordY, int nCoordX){
	return _vecVertices[nCoordY  * _nWidth + nCoordX].coordY;
}

void Terrain::render()
{
	glBindVertexArray(_nVao);
	glDrawElements(GL_PATCHES, _nIndices, GL_UNSIGNED_INT, 0);
}

int Terrain::getWidth()
{
	return _nWidth;
}

int Terrain::getHeight()
{
	return _nHeight;
}