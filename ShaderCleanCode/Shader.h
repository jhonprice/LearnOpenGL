#pragma once
#include<string>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "../HelloTriangle/LoadShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;
	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec2(const std::string& name, float x, float y) const;
private:
	unsigned int program_ID = 0;

};
void ShaderProgram::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(this->program_ID, name.c_str()), value);
}
void ShaderProgram::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(this->program_ID, name.c_str()), value);
}
void ShaderProgram::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(program_ID, name.c_str()), x, y, z);
}
void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(program_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(program_ID, name.c_str()), 1, &value[0]);
}
void ShaderProgram::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(program_ID, name.c_str()), x, y);
}