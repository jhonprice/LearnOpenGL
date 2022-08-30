#include<iostream>
#include "Shader.h"

static int width = 600, height = 600;
const int NULL_ID = 0;

//<<��Ⱦ����>>
//<<EBO��ʽ����>>
//<<����Uniformȫ�ֱ���>>
static void display(GLFWwindow* window, unsigned int shaderProgram_ID, int VAO_ID, int EBO_ID, int useUniform)
{

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram_ID);
	glBindVertexArray(VAO_ID);

#pragma region SetUniform
	if (useUniform) {
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram_ID, "ourColor");

		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
	}
#pragma endregion SetUniform


	//<<EBO��ʽ����>>
	int drawLen = 6;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ID);

	auto indexArrayType = GL_UNSIGNED_INT;
	auto indexArrayOffset = 0;
	glDrawElements(GL_TRIANGLES, drawLen, indexArrayType, BUFFER_OFFSET(indexArrayOffset));

	glfwSwapBuffers(window);
}

//<<�����ӿ�>>
static void reshape(GLFWwindow* window, int w, int h)
{
	width = w > 1 ? w : 1;
	height = h > 1 ? h : 1;
	glViewport(0, 0, width, height);
}
//<<��������>>
static void processInput(GLFWwindow* window) {
#pragma region Esc_CLose
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
#pragma endregion Esc_CLose

#pragma region F_LINE
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
#pragma endregion F_LINE

}
static int enableUniform(GLFWwindow* window) {

	int i = 0;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		i = 1;
	}
	else {
		i = 0;
	}
	return i;
}

int main(int argc, char** argv)
{

	//<<��ʼ��GLFW>>
#pragma region InitGLFW
	auto initValue = glfwInit();
	if (!initValue) std::cerr << "GLFW FAILED" << std::endl;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#pragma endregion


	//<<��������>>
#pragma region InitWindows
	GLFWwindow* window;
	window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
#pragma endregion


	//<<��ʼ��GL3W>>
#pragma region InitGl3W
	if (gl3wInit()) {
		std::cerr << "failed to initialize OpenGL" << std::endl;
		return -1;
	}
	if (!gl3wIsSupported(3, 3)) {
		std::cerr << "OpenGL 3.3 not supported" << std::endl;
		return -1;
	}
	std::cerr << "OpenGL: " << glGetString(GL_VERSION)
		<< " GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#pragma endregion


	//<<�ӿ�����>>
#pragma region ViewPortSet
	glfwSetFramebufferSizeCallback(window, reshape);
#pragma endregion

	//<<OpenGL����ĸ���ID>>
#pragma region IDManaged
	//VAO
	unsigned int VAO_ID;
	int VAO_ID_NUM = 1;

	//VBO
	unsigned int VBO_ID;
	int VBO_ID_NUM = 1;

	//VertexShader
	//unsigned int vertexShader_ID=0;
	//int vertexShader_ID_NUM = 1;
	Shader vertexShader("../shader/clean_shader.vert",GL_VERTEX_SHADER);


	//FragmentShader
	//unsigned int fragShader_ID[2];
	//int fragShader_ID_NUM = 1;
	Shader fragShader1("../shader/clean_shader.frag", GL_FRAGMENT_SHADER);
	Shader fragShader2("../shader/clean_shader_uniform.frag", GL_FRAGMENT_SHADER);
	unsigned int vertexShader_ID = vertexShader.getId();
	unsigned int fragShader_ID[2]{ fragShader1.getId(),fragShader2.getId() };


	//Program
	//unsigned int shaderProgram_ID[2];
	ShaderProgram shaderProgram1(vertexShader_ID, fragShader_ID[0]);
	ShaderProgram shaderProgram2(vertexShader_ID, fragShader_ID[1]);
	unsigned int shaderProgram_ID[2]{ shaderProgram1.getId(),shaderProgram2.getId() };

	//EBO
	unsigned int EBO_ID;
	int EBO_ID_NUM = 1;




#pragma endregion IDManaged

	//<<���ö����������ݽ��͵Ļ���>>
#pragma region SetVAO
	glGenVertexArrays(VAO_ID_NUM, &VAO_ID);
	glBindVertexArray(VAO_ID);
#pragma endregion SetVAO


	//<<���ö�������>>
	//+=<<��ά������>>
#pragma region SetVertex
	float vertices[] = {
	 0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,// top right //blue
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,// bottom right //red
	-0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,// bottom left //black
	-0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f// top left  //green
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	//<<����buffer����buffer����ֵ����ԭbuffer>>
	glGenBuffers(VBO_ID_NUM, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
#pragma endregion SetVertex

	//<<����EBO>>
#pragma region SetEBO
	glGenBuffers(EBO_ID_NUM, &EBO_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
#pragma endregion SetEBO


	//<<���Ͷ�����������>>
	//+<<ƫ������ʹ�÷���>>
#pragma region ExplainVertex
	//layout��ָ����location
	int locationInShader = 0;
	int colorInShader = 1;
	//��Ӧ�������ݵ�ά��
	int inShaderDataSize = 3;
	//�洢ǰ�Ƿ��һ��
	auto needNormalize = GL_FALSE;
	//ע����ƫ�����붥���������ݵĶ�Ӧ��ϵ
	glVertexAttribPointer(locationInShader, inShaderDataSize, GL_FLOAT, needNormalize, 6 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
	glVertexAttribPointer(colorInShader, inShaderDataSize, GL_FLOAT, needNormalize, 6 * sizeof(GL_FLOAT), BUFFER_OFFSET(3 * sizeof(GL_FLOAT)));
	//ע��Ҫ������Ӧ��λ��
	glEnableVertexAttribArray(locationInShader);
	glEnableVertexAttribArray(colorInShader);
#pragma endregion ExplainVertex


	//<<���ó���(��ѡ)��������ɫ������>>
#pragma region UseProgram
	//glUseProgram(shaderProgram_ID);
	glDeleteShader(vertexShader_ID);
	glDeleteShader(fragShader_ID[0]);
	glDeleteShader(fragShader_ID[1]);
#pragma endregion UseProgram

	//<<����buffer��λ>>
#pragma region Reset
	//��һ�����ݽ������λ
	glBindBuffer(GL_ARRAY_BUFFER, NULL_ID);
	glBindVertexArray(NULL_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL_ID);
#pragma endregion Reset




	//<<��Ⱦѭ��>>
#pragma region RenderPart
	while (!glfwWindowShouldClose(window)) {
		std::cerr << "\rActive Time: " << glfwGetTime() << std::flush;
		//<<��������>>
		processInput(window);
		int useUniform = enableUniform(window);
		//<<��Ⱦ����>>
		//<<EBO��ʽ����>>
		display(window, shaderProgram_ID[useUniform], VAO_ID, EBO_ID, useUniform);
		//<<�¼�������>>
		glfwPollEvents();

	}
#pragma endregion



	glfwTerminate();
	return 0;
}
