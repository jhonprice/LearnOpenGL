#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#define BUFFER_OFFSET(a) ((void*)(a))

void triangle_plane_without_index(unsigned int& VAO_ID,unsigned int& VBO_ID, unsigned int& len) {

	glGenVertexArrays(1, &VAO_ID);
	glBindVertexArray(VAO_ID);

	//<<���ö�������>>

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f,  0.5f, 0.0f
	};
	len = 3;
	//<<����buffer����buffer����ֵ����ԭbuffer>>
	glGenBuffers(1, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//layout��ָ����location
	int locationInShader = 0;
	//��Ӧ�������ݵ�ά��
	int inShaderDataSize = 3;
	//�洢ǰ�Ƿ��һ��
	auto needNormalize = GL_FALSE;
	glVertexAttribPointer(locationInShader, inShaderDataSize, GL_FLOAT, needNormalize, 3 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(locationInShader);


}