#pragma once
#include<string>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "../HelloTriangle/LoadShader.h"
class Shader {
public:
	Shader(string filename, int type) :fileName(filename), shader_type(type) { loadShader(); 
	}


	void loadShader(string filename) {
		std::string fragShaderSourceCodeString = readShader(filename);
		const char* fragShaderSourceCode = fragShaderSourceCodeString.c_str();
		//trap:https://stackoverflow.com/questions/54733233 ÓÃ´íshader_typeµÄ´íÎó
		shader_ID = glCreateShader(shader_type);

		glShaderSource(shader_ID, shader_code_string_num, &fragShaderSourceCode, nullptr);
		glCompileShader(shader_ID);


		debugInfo(shader_ID);
	}
	void loadShader() {
		loadShader(fileName);
	}

	unsigned int  getId() {
		return shader_ID;
	}
private:
	unsigned int shader_ID = 0;
	std::string fileName;
	const int shader_code_string_num = 1;
	int shader_type=-1;
};
class ShaderProgram {
public:
	ShaderProgram(int vertexId,int fragId){
		loadShader(vertexId, fragId);
	}
	void loadShader(int vertexId, int fragId) {
		program_ID = glCreateProgram();
		glAttachShader(program_ID, vertexId);
		glAttachShader(program_ID, fragId);
		glLinkProgram(program_ID);
		programInfo(program_ID);
	}

	unsigned int  getId() {
		return program_ID;
	}
private:
	unsigned int program_ID = 0;

};

