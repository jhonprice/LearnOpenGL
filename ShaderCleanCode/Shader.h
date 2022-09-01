#pragma once
#include<string>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "../HelloTriangle/LoadShader.h"
#include <stb_image.h>
class Shader {
public:
	Shader(string filename, int type) :fileName(filename), shader_type(type) { loadShader(); 
	}


	void loadShader(string filename) {
		std::string fragShaderSourceCodeString = readShader(filename);
		const char* fragShaderSourceCode = fragShaderSourceCodeString.c_str();
		//trap:https://stackoverflow.com/questions/54733233 用错shader_type的错误
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

//<<加载纹理函数>>
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}