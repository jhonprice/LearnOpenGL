#pragma once


#include<fstream>
#include<string>
using std::string;

#define BUFFER_OFFSET(a) ((void*)(a))

string readFile(string fileName) {
	
	std::ifstream in(fileName.c_str());

	std::istreambuf_iterator<char> begin(in);
	std::istreambuf_iterator<char> end;

	std::string content(begin, end);
	in.close();
	return content;
	
}

string readShader(string fileName) {

	return  readFile(fileName);

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