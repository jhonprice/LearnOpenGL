#include<iostream>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "./LoadShader.h"

static int width = 600, height = 600;
const int NULL_ID = 0;

//<<渲染函数>>
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

//<<设置视口>>
static void reshape(GLFWwindow* window, int w, int h)
{
	width = w > 1 ? w : 1;
	height = h > 1 ? h : 1;
	glViewport(0, 0, width, height);
}
//<<处理输入>>
static void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


int main(int argc, char** argv)
{

//<<初始化GLFW>>
#pragma region InitGLFW
	auto initValue = glfwInit();
	if (!initValue) std::cerr << "GLFW FAILED" << std::endl;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#pragma endregion


//<<创建窗口>>
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


//<<初始化GL3W>>
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


//<<视口设置>>
#pragma region ViewPortSet
	glfwSetFramebufferSizeCallback(window, reshape);
#pragma endregion

//<<OpenGL对象的各种ID>>
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

//<<设置顶点数据内容解释的缓存>>
#pragma region SetVAO
	glGenVertexArrays(VAO_ID_NUM, &VAO_ID);
	glBindVertexArray(VAO_ID);
#pragma endregion SetVAO


//<<设置顶点坐标>>
#pragma region SetVertex
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f,  0.5f, 0.0f
	};
	//<<生成buffer，绑定buffer，传值，还原buffer>>
	glGenBuffers(VBO_ID_NUM, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
#pragma endregion SetVertex

//<<解释顶点数据内容>>
#pragma region ExplainVertex
	//layout中指定的location
	int locationInShader = 0;
	//对应传入数据的维度
	int inShaderDataSize = 3;
	//存储前是否归一化
	auto needNormalize = GL_FALSE;
	glVertexAttribPointer(locationInShader, inShaderDataSize, GL_FLOAT, needNormalize, 3 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(locationInShader);
#pragma endregion ExplainVertex


//<<编译着色器，输出调试信息>>
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


//<<构建link着色器程序，输出调试信息>>
#pragma region SetProgram
	shaderProgram_ID = glCreateProgram();
	glAttachShader(shaderProgram_ID, vertexShader_ID);
	glAttachShader(shaderProgram_ID, fragShader_ID);
	glLinkProgram(shaderProgram_ID);

	programInfo(shaderProgram_ID);
#pragma endregion SetProgram
//<<启用程序(可选)，清理着色器代码>>
#pragma region UseProgram
	//glUseProgram(shaderProgram_ID);
	glDeleteShader(vertexShader_ID);
	glDeleteShader(fragShader_ID);
#pragma endregion UseProgram

//<<各种buffer归位>>
#pragma region MyRegion
	//在一批数据结束后归位
	glBindBuffer(GL_ARRAY_BUFFER, NULL_ID);
	glBindVertexArray(NULL_ID);
#pragma endregion




//<<渲染循环>>
#pragma region RenderPart
	while (!glfwWindowShouldClose(window)) {

		//<<处理输入>>
		processInput(window);
		//<<渲染函数>>
		display(window,shaderProgram_ID,VAO_ID);
		//<<事件触发器>>
		glfwPollEvents();

	}
#pragma endregion



	glfwTerminate();
	return 0;
}
