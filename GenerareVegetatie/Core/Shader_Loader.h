/* Alexandru-Catalin Ciobanu 341C4*/
#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"
#include <iostream>

	class Shader_Loader{
	private:

		std::string ReadShader(char *filename);
		GLuint CreateShader(GLenum shaderType,
			std::string source,
			char* shaderName);

	public:

		Shader_Loader(void);
		~Shader_Loader(void);

		GLuint CreateProgram(char* VertexShaderFilename,
			char* FragmentShaderFilename);

		GLuint CreateProgram(char* VertexShaderFilename, 
							char* GeometryShaderFilename,
							 char* FragmentShaderFilename);

		GLuint CreateProgram(char* VertexShaderFilename,
			char* TesselationControlShaderFilename,
			char* TesselationEvaluationShaderFilename,
			char* FragmentShaderFilename);

		GLuint CreateProgram(char* VertexShaderFilename,
			char* TesselationControlShaderFilename,
			char* TesselationEvaluationShaderFilename,
			char* GeometryShaderFilename,
			char* FragmentShaderFilename);


	};

