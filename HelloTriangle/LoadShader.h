#pragma once


#include<fstream>
#include<string>
using std::string;





string readShader(string fileName) {
	
	std::ifstream in(fileName.c_str());

	std::istreambuf_iterator<char> begin(in);
	std::istreambuf_iterator<char> end;

	std::string content(begin, end);
	in.close();
	return content;
	
}

//<<shader调试信息>>
int debugInfo(unsigned int shaderId) {
	int success;
	//获取指定对象信息
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		int len;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &len);

		char* infoLog = new char[len + 1];
		glGetShaderInfoLog(shaderId, len, &len, infoLog);
		std::cerr << "Shader compilation failed: " << infoLog << std::endl;
		delete[] infoLog;
	}
	return success;
}


int programInfo(unsigned int programId) {
	int linked;
	glGetProgramiv(programId, GL_LINK_STATUS, &linked);
	if (!linked) {
		int len;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &len);
		char* log = new char[len + 1];
		glGetProgramInfoLog(programId, len, &len, log);
		std::cerr << "Shader linking failed: " << log << std::endl;
		delete[] log;
	}
	return linked;
}

void loadShader(string path, unsigned int& shaderId, unsigned int shaderType) {
	string vertexShaderSourceCodeString = readShader(path);
	const char* vertexShaderSourceCode = vertexShaderSourceCodeString.c_str();
	shaderId = glCreateShader(shaderType);

	glShaderSource(shaderId, 1, &vertexShaderSourceCode, nullptr);
	glCompileShader(shaderId);


	debugInfo(shaderId);
}

void linkProgram(unsigned int vertexId, unsigned int fragId, unsigned int& programId) {
	programId = glCreateProgram();
	glAttachShader(programId, vertexId);
	glAttachShader(programId, fragId);
	glLinkProgram(programId);

	programInfo(programId);
}