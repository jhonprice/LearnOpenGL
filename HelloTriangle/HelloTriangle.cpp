#include<iostream>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "./LoadShader.h"

static int width = 600, height = 600;
const int NULL_ID = 0;

//<<��Ⱦ����>>
static void display(GLFWwindow* window, int shaderProgram_ID, int VAO_ID)
{
	
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram_ID);
	glBindVertexArray(VAO_ID);

	int fromIndex = 0;
	int drawLen = 3;
	glDrawArrays(GL_TRIANGLES, fromIndex, drawLen);

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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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
	unsigned int vertexShader_ID;
	int vertexShader_ID_NUM = 1;

	//FragmentShader
	unsigned int fragShader_ID;
	int fragShader_ID_NUM = 1;

	//Program
	unsigned int shaderProgram_ID;


	
#pragma endregion IDManaged

//<<���ö����������ݽ��͵Ļ���>>
#pragma region SetVAO
	glGenVertexArrays(VAO_ID_NUM, &VAO_ID);
	glBindVertexArray(VAO_ID);
#pragma endregion SetVAO


//<<���ö�������>>
#pragma region SetVertex
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f,  0.5f, 0.0f
	};
	//<<����buffer����buffer����ֵ����ԭbuffer>>
	glGenBuffers(VBO_ID_NUM, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
#pragma endregion SetVertex

//<<���Ͷ�����������>>
#pragma region ExplainVertex
	//layout��ָ����location
	int locationInShader = 0;
	//��Ӧ�������ݵ�ά��
	int inShaderDataSize = 3;
	//�洢ǰ�Ƿ��һ��
	auto needNormalize = GL_FALSE;
	glVertexAttribPointer(locationInShader, inShaderDataSize, GL_FLOAT, needNormalize, 3 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(locationInShader);
#pragma endregion ExplainVertex


//<<������ɫ�������������Ϣ>>
#pragma region SetVertexShader
	string vertexShaderSourceCodeString = readShader("../shader/hellovertex.vert");
	const char* vertexShaderSourceCode = vertexShaderSourceCodeString.c_str();
	vertexShader_ID = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader_ID, vertexShader_ID_NUM, &vertexShaderSourceCode, nullptr);
	glCompileShader(vertexShader_ID);


	debugInfo(vertexShader_ID);
#pragma endregion SetVertexShader

#pragma region SetFragShader
	string fragShaderSourceCodeString = readShader("../shader/hellofrag.frag");
	const char* fragShaderSourceCode = fragShaderSourceCodeString.c_str();
	fragShader_ID = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragShader_ID, fragShader_ID_NUM, &fragShaderSourceCode, nullptr);
	glCompileShader(fragShader_ID);


	debugInfo(fragShader_ID);
#pragma endregion SetFragShader


//<<����link��ɫ���������������Ϣ>>
#pragma region SetProgram
	shaderProgram_ID = glCreateProgram();
	glAttachShader(shaderProgram_ID, vertexShader_ID);
	glAttachShader(shaderProgram_ID, fragShader_ID);
	glLinkProgram(shaderProgram_ID);

	programInfo(shaderProgram_ID);
#pragma endregion SetProgram
//<<���ó���(��ѡ)��������ɫ������>>
#pragma region UseProgram
	//glUseProgram(shaderProgram_ID);
	glDeleteShader(vertexShader_ID);
	glDeleteShader(fragShader_ID);
#pragma endregion UseProgram

//<<����buffer��λ>>
#pragma region MyRegion
	//��һ�����ݽ������λ
	glBindBuffer(GL_ARRAY_BUFFER, NULL_ID);
	glBindVertexArray(NULL_ID);
#pragma endregion




//<<��Ⱦѭ��>>
#pragma region RenderPart
	while (!glfwWindowShouldClose(window)) {

		//<<��������>>
		processInput(window);
		//<<��Ⱦ����>>
		display(window,shaderProgram_ID,VAO_ID);
		//<<�¼�������>>
		glfwPollEvents();

	}
#pragma endregion



	glfwTerminate();
	return 0;
}
