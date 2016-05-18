/* Alexandru-Catalin Ciobanu 341C4*/
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<vector>
#include<time.h>

#include "Core\Shader_Loader.h"
#include "Core\Image_Loader.h"
#include "Models\Terrain.h"
#include "Models\Grass.h"
#include "Models\Tree.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "GL\freeglut.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"

// variabile pentru programele shader
GLuint terrainShader, grassShader, treeShader, cubeMapShader, leafShader;

glm::mat4 matModel, matView, matProjection, matModelCubeMap;
glm::vec3 lightPosition, spotlightPosition, eyePosition, wind;
glm::vec3 right, up, dir;

std::string sHeightMap;
std::string sTexture;

// vectorul in care memorăm copacii
std::vector<Tree*> vecTrees;

// vectorul in care memorăm zonele de iarbă
std::vector<Grass*> vecGrass;

// obiectul folosit pentru generarea si redarea terenului
Terrain* tLand;

time_t startTime, currentTime, lastTime;

bool vecKeyPressed[256];
bool vecSpecialKeyPressed[256];

unsigned int texPlant, texPlant2, texPlant3, texDirt, texPlant1, texTreeTrunk, texTreeLeaf, texCubeMap;
unsigned int texGrass0, texGrass1, texGrass2, texGrass3, texGrass4, texGrass5;
unsigned int noGrass, noTrees;
unsigned int cubeMap_vao, cubeMap_vbo;

const float pi = 3.14159265f;
float fGrassSize, fGrassRadius;
float xpos, ypos, zpos, xrot, yrot, angle;
float lastx, lasty;
float level;

int noFrames = 0;

static bool bMouseMove = false;

double fpsLastTime, fpsCurrentTime;


std::ofstream myfile;
//nu suporta compresie! 
//incarca un fisier BMP intr-un array unsigned char
//returneaza un pointer la un array ce contine datele texturii si valori in argumentele trimise prin referinta width si height
unsigned char* _loadBMPFile(const std::string &filename, unsigned int &width, unsigned int &height){
	//structuri pentru datele din header
	struct header{
		unsigned char type[2];
		int f_lenght;
		short rezerved1;
		short rezerved2;
		int offBits;
	};
	struct header_info{
		int size;
		int width;
		int height;
		short planes;
		short bitCount;
		int compresion;
		int sizeImage;
		int xPelsPerMeter;
		int yPelsPerMeter;
		int clrUsed;
		int clrImportant;
	};

	//citim din fisier
	std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
	if (!file.good()){
		std::cout << "Texture Loader: Nu am gasit fisierul bmp " << filename << " sau nu am drepturile sa il deschid!" << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}
	std::cout << "Texture Loader: Incarc fisierul " << filename << std::endl;


	//citesc headere
	header h; header_info h_info;
	file.read((char*)&(h.type[0]), sizeof(char));
	file.read((char*)&(h.type[1]), sizeof(char));
	file.read((char*)&(h.f_lenght), sizeof(int));
	file.read((char*)&(h.rezerved1), sizeof(short));
	file.read((char*)&(h.rezerved2), sizeof(short));
	file.read((char*)&(h.offBits), sizeof(int));
	file.read((char*)&(h_info), sizeof(header_info));

	//aloc memorie (un pixel are 3 componente R, G, B)
	unsigned char *data = new unsigned char[h_info.width*h_info.height * 3];

	// verific daca exista grupuri de 4 octeti pe linie
	long padd = 0;
	if ((h_info.width * 3) % 4 != 0) padd = 4 - (h_info.width * 3) % 4;

	//save height &width
	width = h_info.width;
	height = h_info.height;

	long pointer;
	unsigned char r, g, b;
	//citesc matricea
	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			file.read((char*)&b, 1);	//in bmp ordinea in pixel este b,g,r (specific windows)
			file.read((char*)&g, 1);
			file.read((char*)&r, 1);

			pointer = ((height - i - 1)*width + j) * 3;
			//pointer = (i*width + j) * 3;
			data[pointer] = r;
			data[pointer + 1] = g;
			data[pointer + 2] = b;
		}

		file.seekg(padd, std::ios_base::cur);
	}
	file.close();

	//returneaza datele
	return data;
}

//incarca o imagine BMP si creeaza cu ea o textura
//aceasta este functia pe care o veti apela
//returneaza id-ul texturii
unsigned int loadTextureBMP(const std::string &filename){

	unsigned int width, height;
	unsigned char* data = _loadBMPFile(filename, width, height);

	//creeaza textura OpenGL
	unsigned int gl_texture_object;
	glGenTextures(1, &gl_texture_object);
	glBindTexture(GL_TEXTURE_2D, gl_texture_object);

	//filtrare
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	float maxAnisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

	//cand lucram cu texturi cu dimensiuni non multiple de 4 trebuie sa facem cititorul de randuri
	//ce incarca texturile in OpenGL sa lucreze cu memorie aliniata la 1 (default este la 4)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//genereaza textura
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	//distruge array-ul din RAM
	delete data;

	//creaza ierarhia de mipmapuri
	glGenerateMipmap(GL_TEXTURE_2D);

	//returneaza obiectul textura
	return gl_texture_object;
}

/* incarca 6 imagini BMP si creeaza cu ele un cubemap */
unsigned int loadTextureCubemapBMP(const std::string &posx, const std::string &posy, const std::string &posz, const std::string& negx, const std::string& negy, const std::string& negz){

	unsigned int width, height;
	unsigned char* data_posx = _loadBMPFile(posx, width, height);
	unsigned char* data_posy = _loadBMPFile(posy, width, height);
	unsigned char* data_posz = _loadBMPFile(posz, width, height);
	unsigned char* data_negx = _loadBMPFile(negx, width, height);
	unsigned char* data_negy = _loadBMPFile(negy, width, height);
	unsigned char* data_negz = _loadBMPFile(negz, width, height);

	//creeaza textura OpenGL
	unsigned int gl_texture_object;
	glGenTextures(1, &gl_texture_object);
	glBindTexture(GL_TEXTURE_CUBE_MAP, gl_texture_object);

	//filtrare
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	float maxAnisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

	//cand lucram cu texturi cu dimensiuni non multiple de 4 trebuie sa facem cititorul de randuri
	//ce incarca texturile in OpenGL sa lucreze cu memorie aliniata la 1 (default este la 4)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//adauga date
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_posx);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_posy);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_posz);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_negx);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_negy);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_negz);

	//distruge array-ul din RAM
	delete data_posx;
	delete data_posy;
	delete data_posz;
	delete data_negx;
	delete data_negy;
	delete data_negz;

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	//returneaza obiectul textura
	return gl_texture_object;
}

void initShaders()
{
	//load and compile shaders
	Shader_Loader shaderLoader;

	terrainShader = shaderLoader.CreateProgram("Resources\\Shaders\\terrain_vertex_shader.glsl",
		"Resources\\Shaders\\terrain_tess_control_shader.glsl",
		"Resources\\Shaders\\terrain_tess_eval_shader.glsl",
		"Resources\\Shaders\\terrain_fragment_shader.glsl");

	grassShader = shaderLoader.CreateProgram(
		"Resources\\Shaders\\grass_vertex_shader.glsl",
		"Resources\\Shaders\\grass_fragment_shader.glsl");

	treeShader = shaderLoader.CreateProgram(
		"Resources\\Shaders\\tree_vertex_shader.glsl",
		"Resources\\Shaders\\tree_fragment_shader.glsl");

	cubeMapShader = shaderLoader.CreateProgram(
		"Resources\\Shaders\\skybox_vertex_shader.glsl",
		"Resources\\Shaders\\skybox_fragment_shader.glsl");

	leafShader = shaderLoader.CreateProgram(
		"Resources\\Shaders\\leaf_vertex_shader.glsl",
		"Resources\\Shaders\\leaf_fragment_shader.glsl");
}

void initTextures()
{
	texPlant1 = loadTextureBMP("Resources\\Textures\\grass1.bmp");
	texPlant2 = loadTextureBMP("Resources\\Textures\\grass2.bmp");
	texPlant3 = loadTextureBMP("Resources\\Textures\\grass3.bmp");

	texGrass0 = loadTextureBMP("Resources\\Textures\\blade_texture1.bmp");
	texGrass1 = loadTextureBMP("Resources\\Textures\\blade_texture2.bmp");
	texGrass2 = loadTextureBMP("Resources\\Textures\\blade_texture3.bmp");
	texGrass3 = loadTextureBMP("Resources\\Textures\\blade_texture4.bmp");
	texGrass4 = loadTextureBMP("Resources\\Textures\\blade_texture5.bmp");

	texDirt = loadTextureBMP("Resources\\Textures\\land_grass_texture.bmp");
	texTreeTrunk = loadTextureBMP("Resources\\Textures\\tree_trunk_texture3.bmp");
	texTreeLeaf = loadTextureBMP("Resources\\Textures\\leaf_texture.bmp");

	texCubeMap = loadTextureCubemapBMP("Resources\\Textures\\skybox_texture1.bmp",
		"Resources\\Textures\\skybox_texture5.bmp",
		"Resources\\Textures\\skybox_texture4.bmp",
		"Resources\\Textures\\skybox_texture3.bmp",
		"Resources\\Textures\\skybox_texture6.bmp",
		"Resources\\Textures\\skybox_texture2.bmp");

	sHeightMap = "Resources\\Heightmaps\\heightmap1.bmp";
	sTexture = "Resources\\Textures\\land_texture1.bmp";

}

void initVectors()
{
	eyePosition = glm::vec3(1024, 200, 1024);

	lightPosition = glm::vec3(0, 70, 0);
	spotlightPosition = glm::vec3(0, 70, 0);
	wind = glm::vec3(2, 0.5, 2);

	matView = glm::lookAt(eyePosition, glm::vec3(1100, 150, 1100), up);

}

void initMouseMovements()
{
	RECT actualDesktop;
	GetWindowRect(GetDesktopWindow(), &actualDesktop);

	//pentru miscarea camerei dupa mouse
	xpos = actualDesktop.right / 2;		ypos = actualDesktop.bottom / 2;
	lastx = actualDesktop.right / 2;	lasty = actualDesktop.bottom / 2;
	yrot = 0.0f;		xrot = 3.145f;

	dir = glm::vec3(cos(yrot) * sin(xrot), sin(yrot), cos(yrot)*cos(xrot));
	right = glm::vec3(sin(xrot - 3.14f / 2.0f), 0.0f, cos(xrot - 3.14f / 2.0f));
	up = glm::cross(right, dir);
}

void initCubeMap() {
	float cubeMapSize = 5000;
	float points[] = {
		-cubeMapSize, cubeMapSize, -cubeMapSize,
		-cubeMapSize, -cubeMapSize, -cubeMapSize,
		cubeMapSize, -cubeMapSize, -cubeMapSize,
		cubeMapSize, -cubeMapSize, -cubeMapSize,
		cubeMapSize, cubeMapSize, -cubeMapSize,
		-cubeMapSize, cubeMapSize, -cubeMapSize,

		-cubeMapSize, -cubeMapSize, cubeMapSize,
		-cubeMapSize, -cubeMapSize, -cubeMapSize,
		-cubeMapSize, cubeMapSize, -cubeMapSize,
		-cubeMapSize, cubeMapSize, -cubeMapSize,
		-cubeMapSize, cubeMapSize, cubeMapSize,
		-cubeMapSize, -cubeMapSize, cubeMapSize,

		cubeMapSize, -cubeMapSize, -cubeMapSize,
		cubeMapSize, -cubeMapSize, cubeMapSize,
		cubeMapSize, cubeMapSize, cubeMapSize,
		cubeMapSize, cubeMapSize, cubeMapSize,
		cubeMapSize, cubeMapSize, -cubeMapSize,
		cubeMapSize, -cubeMapSize, -cubeMapSize,

		-cubeMapSize, -cubeMapSize, cubeMapSize,
		-cubeMapSize, cubeMapSize, cubeMapSize,
		cubeMapSize, cubeMapSize, cubeMapSize,
		cubeMapSize, cubeMapSize, cubeMapSize,
		cubeMapSize, -cubeMapSize, cubeMapSize,
		-cubeMapSize, -cubeMapSize, cubeMapSize,

		-cubeMapSize, cubeMapSize, -cubeMapSize,
		cubeMapSize, cubeMapSize, -cubeMapSize,
		cubeMapSize, cubeMapSize, cubeMapSize,
		cubeMapSize, cubeMapSize, cubeMapSize,
		-cubeMapSize, cubeMapSize, cubeMapSize,
		-cubeMapSize, cubeMapSize, -cubeMapSize,

		-cubeMapSize, -cubeMapSize, -cubeMapSize,
		-cubeMapSize, -cubeMapSize, cubeMapSize,
		cubeMapSize, -cubeMapSize, -cubeMapSize,
		cubeMapSize, -cubeMapSize, -cubeMapSize,
		-cubeMapSize, -cubeMapSize, cubeMapSize,
		cubeMapSize, -cubeMapSize, cubeMapSize
	};

	glGenBuffers(1, &cubeMap_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cubeMap_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &points, GL_STATIC_DRAW);

	glGenVertexArrays(1, &cubeMap_vao);
	glBindVertexArray(cubeMap_vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, cubeMap_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	matModelCubeMap = glm::translate(matModel, glm::vec3(2048, 0, 2048));
}

void initTerrain()
{
	tLand = new Terrain(sHeightMap, sTexture, 150);
}

void initVegetation()
{
	noTrees = 8;
	noGrass = 30000;
	float nGrassWidth = 1;
	float nGrassHeight = 25;
	fGrassRadius = 1000;
	for (size_t i = 0; i < noTrees; i++)
	{
		vecTrees.push_back(new Tree(tLand));
		vecTrees[i]->createTree();
	}

	int terrainWidth = tLand->getWidth();
	int terrainHeight = tLand->getHeight();

	for (size_t i = 0; i < terrainWidth * 40; i += 1000) {
		for (size_t j = 0; j < terrainHeight * 40; j += 1000) {
			vecGrass.push_back(new Grass(noGrass, nGrassWidth, nGrassHeight, i, j, fGrassRadius, tLand));
		}
	}
}

void Init(){

	glClearColor(0.5, 0.5, 0.5, 1);
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);

	for (size_t i = 0; i < 256; i++)
	{
		vecSpecialKeyPressed[i] = false;
		vecKeyPressed[i] = false;
	}

	initShaders();

	initTextures();

	initMouseMovements();

	initVectors();

	initCubeMap();

	initTerrain();

	initVegetation();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	GLint MaxPatchVertices = 0;
	glGetIntegerv(GL_MAX_PATCH_VERTICES, &MaxPatchVertices);
	printf("Max supported patch vertices %d\n", MaxPatchVertices);
}

void drawTerrain() {

	glUseProgram(terrainShader);

	//trimite variabile uniforme la shader
	glUniformMatrix4fv(glGetUniformLocation(terrainShader, "model_matrix"), 1, false, glm::value_ptr(matModel));
	glUniformMatrix4fv(glGetUniformLocation(terrainShader, "view_matrix"), 1, false, glm::value_ptr(matView));
	glUniformMatrix4fv(glGetUniformLocation(terrainShader, "projection_matrix"), 1, false, glm::value_ptr(matProjection));
	glUniform3f(glGetUniformLocation(terrainShader, "gEyeWorldPos"), eyePosition.x, eyePosition.y, eyePosition.z);

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, texDirt);
	glUniform1i(glGetUniformLocation(terrainShader, "textura"), 1);

	tLand->render();
}

void drawVegetation() {

	time(&currentTime);

	float seconds = difftime(currentTime, startTime) * 5;

	for (int i = 0; i < noTrees; i++) {

		glm::vec3 treeStartPosition = vecTrees[i]->getStartPosition();
		int treeLodLevel = Tree::selectTreeLod(eyePosition, treeStartPosition);

		glUseProgram(treeShader);
		glm::mat4 matModel = glm::mat4(2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2);
		glUniformMatrix4fv(glGetUniformLocation(treeShader, "model_matrix"), 1, false, glm::value_ptr(matModel));
		glUniformMatrix4fv(glGetUniformLocation(treeShader, "view_matrix"), 1, false, glm::value_ptr(matView));
		glUniformMatrix4fv(glGetUniformLocation(treeShader, "projection_matrix"), 1, false, glm::value_ptr(matProjection));

		glUniform3f(glGetUniformLocation(treeShader, "eye_position"), eyePosition.x, eyePosition.y, eyePosition.z);
		glUniform3f(glGetUniformLocation(treeShader, "wind"), wind.x, wind.y, wind.z);
		glUniform1f(glGetUniformLocation(treeShader, "time"), cos(seconds));
		glUniform1f(glGetUniformLocation(treeShader, "circlesPerBranch"), 6);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, texTreeTrunk);

		glUniform1i(glGetUniformLocation(treeShader, "textura"), 1);

		vecTrees[i]->drawTree(treeLodLevel);

		glUseProgram(leafShader);
		glUniformMatrix4fv(glGetUniformLocation(leafShader, "model_matrix"), 1, false, glm::value_ptr(matModel));
		glUniformMatrix4fv(glGetUniformLocation(leafShader, "view_matrix"), 1, false, glm::value_ptr(matView));
		glUniformMatrix4fv(glGetUniformLocation(leafShader, "projection_matrix"), 1, false, glm::value_ptr(matProjection));

		glUniform3f(glGetUniformLocation(leafShader, "wind"), wind.x, wind.y, wind.z);
		glUniform1f(glGetUniformLocation(leafShader, "time"), cos(seconds));
		glUniform1f(glGetUniformLocation(leafShader, "circlesPerBranch"), 6);
		glUniform1f(glGetUniformLocation(leafShader, "branchLevels"), 5);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, texTreeLeaf);

		glUniform1i(glGetUniformLocation(leafShader, "textura"), 1);

		vecTrees[i]->drawLeaves(treeLodLevel);
	}

	glUseProgram(grassShader);
	glUniformMatrix4fv(glGetUniformLocation(grassShader, "model_matrix"), 1, false, glm::value_ptr(matModel));
	glUniformMatrix4fv(glGetUniformLocation(grassShader, "view_matrix"), 1, false, glm::value_ptr(matView));
	glUniformMatrix4fv(glGetUniformLocation(grassShader, "projection_matrix"), 1, false, glm::value_ptr(matProjection));

	glUniform3f(glGetUniformLocation(grassShader, "eye_position"), eyePosition.x, eyePosition.y, eyePosition.z);
	glUniform3f(glGetUniformLocation(grassShader, "wind"), wind.x, wind.y, wind.z);
	glUniform1f(glGetUniformLocation(grassShader, "time"), cos(seconds));
	float fRandX = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX))) / 20;
	float fRandY = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX))) / 20;
	float fRandZ = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX))) / 20;
	glUniform3f(glGetUniformLocation(grassShader, "random"), fRandX, fRandY, fRandZ);

	int noGrassZones = vecGrass.size();
	for (size_t i = 0; i < noGrassZones; i++) {
		int grassLodLevel = Grass::selectGrassLod(eyePosition, vecGrass[i]->getStartPosition(), fGrassRadius);
		vecGrass[i]->drawGrass(grassLodLevel);
	}
}

void drawCubeMap()
{
	glUseProgram(cubeMapShader);

	glUniformMatrix4fv(glGetUniformLocation(cubeMapShader, "view_matrix"), 1, false, glm::value_ptr(matView));
	glUniformMatrix4fv(glGetUniformLocation(cubeMapShader, "projection_matrix"), 1, false, glm::value_ptr(matProjection));
	glUniformMatrix4fv(glGetUniformLocation(cubeMapShader, "model_matrix"), 1, false, glm::value_ptr(matModelCubeMap));

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texCubeMap);
	glUniform1i(glGetUniformLocation(cubeMapShader, "texture_cubemap"), 1);

	glBindVertexArray(cubeMap_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}

void reshapeView(int width, int height)
{
	//reshape
	// if (height == 0) height = 1;
	glViewport(0, 0, width, height);
	matProjection = glm::perspective(90.0f, (float)width / (float)height, 0.1f, 10000.0f);
}

void doMovements()
{
	if (vecSpecialKeyPressed[GLUT_KEY_RIGHT]) {
		eyePosition += right * 3.0f;
		matView = glm::lookAt(eyePosition, eyePosition + dir, up);
	}
	if (vecSpecialKeyPressed[GLUT_KEY_LEFT]) {
		eyePosition -= right * 3.0f;
		matView = glm::lookAt(eyePosition, eyePosition + dir, up);
	}
	if (vecSpecialKeyPressed[GLUT_KEY_UP]) {
		eyePosition += dir * 10.0f;
		matView = glm::lookAt(eyePosition, eyePosition + dir, up);
	}
	if (vecSpecialKeyPressed[GLUT_KEY_DOWN]) {
		eyePosition -= dir * 10.0f;
		matView = glm::lookAt(eyePosition, eyePosition + dir, up);
	}
}

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	fpsCurrentTime = glfwGetTime();
	noFrames++;
	if (fpsCurrentTime - fpsLastTime >= 1.0) {
		myfile << "ms/frame " << 1000.0 / double(noFrames) << std::endl;
		noFrames = 0;
		fpsLastTime += 1.0;
	}

	drawCubeMap();
	drawTerrain();
	drawVegetation();

	doMovements();

	glutSwapBuffers();

	lastTime = currentTime;
}

void keyPressed(unsigned char key_pressed, int mouse_x, int mouse_y){
	if (key_pressed == 27) exit(0);

	if (key_pressed == 'w'){
		static bool wire = true;
		wire = !wire;
		glPolygonMode(GL_FRONT_AND_BACK, (wire ? GL_LINE : GL_FILL));
	}

	if (key_pressed == 'q') {
		bMouseMove = !bMouseMove;
	}
}

void specialKeyPressed(int key_pressed, int mouse_x, int mouse_y)
{
	switch (key_pressed) {
	case GLUT_KEY_RIGHT:
		vecSpecialKeyPressed[GLUT_KEY_RIGHT] = true;
		break;
	case GLUT_KEY_LEFT:
		vecSpecialKeyPressed[GLUT_KEY_LEFT] = true;
		break;
	case GLUT_KEY_DOWN:
		vecSpecialKeyPressed[GLUT_KEY_DOWN] = true;
		break;
	case GLUT_KEY_UP:
		vecSpecialKeyPressed[GLUT_KEY_UP] = true;
		break;
	}

	if (key_pressed == GLUT_KEY_F1) glutFullScreenToggle();
}

void specialKeyReleased(int key_pressed, int mouse_x, int mouse_y)
{
	switch (key_pressed) {
	case GLUT_KEY_RIGHT:
		vecSpecialKeyPressed[GLUT_KEY_RIGHT] = false;
		break;
	case GLUT_KEY_LEFT:	
		vecSpecialKeyPressed[GLUT_KEY_LEFT] = false;
		break;
	case GLUT_KEY_DOWN:	
		vecSpecialKeyPressed[GLUT_KEY_DOWN] = false;
		break;
	case GLUT_KEY_UP:	
		vecSpecialKeyPressed[GLUT_KEY_UP] = false;
		break;
	}
}

void idleFunction()
{
	glutPostRedisplay();
}

void mouseDrag(int mouse_x, int mouse_y)
{

}

void mouseMove(int mouse_x, int mouse_y){
	if (bMouseMove) {
		xpos = mouse_x;
		ypos = mouse_y;

		if (xpos != lastx || ypos != lasty){
			xrot += 0.003f*(800.0f / 2.0f - xpos);
			yrot += 0.003f*(600.0f / 2.0f - ypos);

			dir = glm::vec3(cos(yrot) * sin(xrot), sin(yrot), cos(yrot)*cos(xrot));
			right = glm::vec3(sin(xrot - 3.14f / 2.0f), 0.0f, cos(xrot - 3.14f / 2.0f));
			up = glm::cross(right, dir);

			matView = glm::lookAt(eyePosition, eyePosition + dir, up);
			lastx = mouse_x;
			lasty = mouse_y;
			glutWarpPointer(800 / 2, 600 / 2);
		}
	}
}

void mouseClick(int button, int state, int mouse_x, int mouse_y){ }

void mouseScroll(int wheel, int direction, int mouse_x, int mouse_y){ std::cout << "Mouse scroll" << std::endl; }

int main(int argc, char **argv)
{

	srand(time(NULL));
	time(&startTime);
	lastTime = startTime;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowPosition(500, 500);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Test");

	glewInit();
	if (!glfwInit())
		exit(EXIT_FAILURE);

	Init();
	if (glewIsSupported("GL_VERSION_3_3"))
	{
		std::cout << "GLEW Version is 3.3\n";
	}
	else
	{
		std::cout << "GLEW 3.3 is not supported\n";
	}
	glEnable(GL_DEPTH_TEST);
	glutIgnoreKeyRepeat(true);

	glutIdleFunc(idleFunction);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshapeView);
	glutSpecialFunc(specialKeyPressed);
	glutSpecialUpFunc(specialKeyReleased);
	glutKeyboardFunc(keyPressed);
	glutMotionFunc(mouseDrag);
	glutPassiveMotionFunc(mouseMove);
	glutMouseFunc(mouseClick);
	glutMouseWheelFunc(mouseScroll);

	const unsigned char* version = glGetString(GL_VERSION);
	std::cout << "Using openGL version " << version << std::endl;
	glutFullScreen();           // making the window full screen
	fpsLastTime = 0;
	fpsCurrentTime = 0;
	glfwSetTime(0);

	myfile.open("fps.txt");

	glutMainLoop();
	myfile.close();
	return 0;
}