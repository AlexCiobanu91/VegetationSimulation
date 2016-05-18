/* Alexandru-Catalin Ciobanu 341C4*/
#include "Shader_Loader.h"
#include<iostream>
#include<fstream>
#include<vector>

Shader_Loader::Shader_Loader(void){}
Shader_Loader::~Shader_Loader(void){}

std::string Shader_Loader::ReadShader(char *filename){

	std::string shaderCode;
	std::ifstream file(filename, std::ios::in);

	if (!file.good()){
		std::cout << "Can't read file " << filename << std::endl;
		std::terminate();
	}

	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&shaderCode[0], shaderCode.size());
	file.close();
	return shaderCode;
}

GLuint Shader_Loader::CreateShader(GLenum shaderType, std::string
	source, char* shaderName){

	int compile_result = 0;

	GLuint shader = glCreateShader(shaderType);
	const char *shader_code_ptr = source.c_str();
	const int shader_code_size = source.size();

	glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

	//check for errors
	if (compile_result == GL_FALSE){

		int info_log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> shader_log(info_log_length);
		glGetShaderInfoLog(shader, info_log_length, NULL, &shader_log[0]);
		std::cout << "ERROR compiling shader: " << shaderName << std::endl << &shader_log[0] << std::endl;
		return 0;
	}
	return shader;
}

GLuint Shader_Loader::CreateProgram(char* vertexShaderFilename,
	char* fragmentShaderFilename){

	//read the shader files and save the code
	std::string vertex_shader_code = ReadShader(vertexShaderFilename);
	std::string fragment_shader_code = ReadShader(fragmentShaderFilename);

	GLuint vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, "vertex shader");
	GLuint fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, "fragment shader");

	int link_result = 0;
	//create the program handle, attatch the shaders and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	//check for link errors
	if (link_result == GL_FALSE){

		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
		return 0;
	}
	return program;
}

GLuint Shader_Loader::CreateProgram(char* vertexShaderFilename,
	char* geometryShaderFilename,
	char* fragmentShaderFilename){

	//read the shader files and save the code
	std::string vertex_shader_code = ReadShader(vertexShaderFilename);
	std::string geometry_shader_code = ReadShader(geometryShaderFilename);
	std::string fragment_shader_code = ReadShader(fragmentShaderFilename);

	GLuint vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, "vertex shader");
	GLuint geometry_shader = CreateShader(GL_GEOMETRY_SHADER, geometry_shader_code, "geometry shader");
	GLuint fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, "fragment shader");

	int link_result = 0;

	//create the program handle, attatch the shaders and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, geometry_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	//check for link errors
	if (link_result == GL_FALSE){

		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
		return 0;
	}
	return program;
}

GLuint Shader_Loader::CreateProgram(char* vertexShaderFilename,
	char* tessControlShaderFilename,
	char* tessEvalShaderFilename,
	char* fragmentShaderFilename){

	//read the shader files and save the code
	std::string vertex_shader_code = ReadShader(vertexShaderFilename);
	std::string tess_control_shader_code = ReadShader(tessControlShaderFilename);
	std::string tess_eval_shader_code = ReadShader(tessEvalShaderFilename);
	std::string fragment_shader_code = ReadShader(fragmentShaderFilename);

	GLuint vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, "vertex shader");
	GLuint tess_control_shader = CreateShader(GL_TESS_CONTROL_SHADER, tess_control_shader_code, "tess control shader");
	GLuint tess_eval_shader = CreateShader(GL_TESS_EVALUATION_SHADER, tess_eval_shader_code, "tess eval shader");
	GLuint fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, "fragment shader");
	int link_result = 0;

	//create the program handle, attatch the shaders and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, tess_control_shader);
	glAttachShader(program, tess_eval_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	//check for link errors
	if (link_result == GL_FALSE){

		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
		return 0;
	}
	return program;
}


GLuint Shader_Loader::CreateProgram(char* vertexShaderFilename,
	char* tessControlShaderFilename,
	char* tessEvalShaderFilename,
	char* geometryShaderFilename,
	char* fragmentShaderFilename)
{

	//read the shader files and save the code
	std::string vertex_shader_code = ReadShader(vertexShaderFilename);
	std::string tess_control_shader_code = ReadShader(tessControlShaderFilename);
	std::string tess_eval_shader_code = ReadShader(tessEvalShaderFilename);
	std::string geometry_shader_code = ReadShader(geometryShaderFilename);
	std::string fragment_shader_code = ReadShader(fragmentShaderFilename);

	GLuint vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, "vertex shader");
	GLuint tess_control_shader = CreateShader(GL_TESS_CONTROL_SHADER, tess_control_shader_code, "tess control shader");
	GLuint tess_eval_shader = CreateShader(GL_TESS_EVALUATION_SHADER, tess_eval_shader_code, "tess eval shader");
	GLuint geometry_shader = CreateShader(GL_GEOMETRY_SHADER, geometry_shader_code, "geometry shader");
	GLuint fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, "fragment shader");
	int link_result = 0;

	//create the program handle, attatch the shaders and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, tess_control_shader);
	glAttachShader(program, tess_eval_shader);
	glAttachShader(program, geometry_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	//check for link errors
	if (link_result == GL_FALSE){

		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
		return 0;
	}
	return program;
}
