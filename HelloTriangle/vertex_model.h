#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#define BUFFER_OFFSET(a) ((void*)(a))

void triangle_plane_without_index(unsigned int& VAO_ID,unsigned int& VBO_ID, unsigned int& len) {

	glGenVertexArrays(1, &VAO_ID);
	glBindVertexArray(VAO_ID);

	//<<设置顶点坐标>>

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f,  0.5f, 0.0f
	};
	len = 3;
	//<<生成buffer，绑定buffer，传值，还原buffer>>
	glGenBuffers(1, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//layout中指定的location
	int locationInShader = 0;
	//对应传入数据的维度
	int inShaderDataSize = 3;
	//存储前是否归一化
	auto needNormalize = GL_FALSE;
	glVertexAttribPointer(locationInShader, inShaderDataSize, GL_FLOAT, needNormalize, 3 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(locationInShader);


}