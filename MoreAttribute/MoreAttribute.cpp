#include<iostream>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "../HelloTriangle/LoadShader.h"

static int width = 600, height = 600;
const int NULL_ID = 0;

//<<渲染函数>>
//<<EBO方式绘制>>
//<<设置Uniform全局变量>>
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


	//<<EBO方式绘制>>
	int drawLen = 6;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ID);

	auto indexArrayType = GL_UNSIGNED_INT;
	auto indexArrayOffset = 0;
	glDrawElements(GL_TRIANGLES, drawLen, indexArrayType, BUFFER_OFFSET(indexArrayOffset));

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
	unsigned int fragShader_ID[2];
	int fragShader_ID_NUM = 1;

	//Program
	unsigned int shaderProgram_ID[2];

	//EBO
	unsigned int EBO_ID;
	int EBO_ID_NUM = 1;




#pragma endregion IDManaged

	//<<设置顶点数据内容解释的缓存>>
#pragma region SetVAO
	glGenVertexArrays(VAO_ID_NUM, &VAO_ID);
	glBindVertexArray(VAO_ID);
#pragma endregion SetVAO


	//<<设置顶点坐标>>
	//+=<<多维度属性>>
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
	//<<生成buffer，绑定buffer，传值，还原buffer>>
	glGenBuffers(VBO_ID_NUM, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
#pragma endregion SetVertex

	//<<设置EBO>>
#pragma region SetEBO
	glGenBuffers(EBO_ID_NUM, &EBO_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
#pragma endregion SetEBO


	//<<解释顶点数据内容>>
	//+<<偏移量的使用方法>>
#pragma region ExplainVertex
	//layout中指定的location
	int locationInShader = 0;
	int colorInShader = 1;
	//对应传入数据的维度
	int inShaderDataSize = 3;
	//存储前是否归一化
	auto needNormalize = GL_FALSE;
	//注意多个偏移量与顶点数组数据的对应关系
	glVertexAttribPointer(locationInShader, inShaderDataSize, GL_FLOAT, needNormalize, 6 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
	glVertexAttribPointer(colorInShader, inShaderDataSize, GL_FLOAT, needNormalize, 6 * sizeof(GL_FLOAT), BUFFER_OFFSET(3 * sizeof(GL_FLOAT)));
	//注意要启用相应的位置
	glEnableVertexAttribArray(locationInShader);
	glEnableVertexAttribArray(colorInShader);
#pragma endregion ExplainVertex


	//<<编译着色器，输出调试信息>>
#pragma region SetVertexShader
	string vertexShaderSourceCodeString = readShader("../shader/moreAttri_shader.vert");
	const char* vertexShaderSourceCode = vertexShaderSourceCodeString.c_str();
	vertexShader_ID = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader_ID, vertexShader_ID_NUM, &vertexShaderSourceCode, nullptr);
	glCompileShader(vertexShader_ID);


	debugInfo(vertexShader_ID);
#pragma endregion SetVertexShader

#pragma region SetFragShader
	string fragShaderSourceCodeString = readShader("../shader/moreAttri_shader.frag");
	const char* fragShaderSourceCode = fragShaderSourceCodeString.c_str();
	fragShader_ID[0] = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragShader_ID[0], fragShader_ID_NUM, &fragShaderSourceCode, nullptr);
	glCompileShader(fragShader_ID[0]);


	debugInfo(fragShader_ID[0]);

	//--------------------------------------------------------------------------------------

	fragShaderSourceCodeString = readShader("../shader/example_shader_uniform.frag");
	fragShaderSourceCode = fragShaderSourceCodeString.c_str();
	fragShader_ID[1] = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragShader_ID[1], fragShader_ID_NUM, &fragShaderSourceCode, nullptr);
	glCompileShader(fragShader_ID[1]);


	debugInfo(fragShader_ID[1]);

#pragma endregion SetFragShader


	//<<构建link着色器程序，输出调试信息>>
#pragma region SetProgram
	shaderProgram_ID[0] = glCreateProgram();
	glAttachShader(shaderProgram_ID[0], vertexShader_ID);
	glAttachShader(shaderProgram_ID[0], fragShader_ID[0]);
	glLinkProgram(shaderProgram_ID[0]);

	programInfo(shaderProgram_ID[0]);

	//-----------------------------------------------------------------------------
	shaderProgram_ID[1] = glCreateProgram();
	glAttachShader(shaderProgram_ID[1], vertexShader_ID);
	glAttachShader(shaderProgram_ID[1], fragShader_ID[1]);
	glLinkProgram(shaderProgram_ID[1]);

	programInfo(shaderProgram_ID[1]);
#pragma endregion SetProgram
	//<<启用程序(可选)，清理着色器代码>>
#pragma region UseProgram
	//glUseProgram(shaderProgram_ID);
	glDeleteShader(vertexShader_ID);
	glDeleteShader(fragShader_ID[0]);
	glDeleteShader(fragShader_ID[1]);
#pragma endregion UseProgram

	//<<各种buffer归位>>
#pragma region Reset
	//在一批数据结束后归位
	glBindBuffer(GL_ARRAY_BUFFER, NULL_ID);
	glBindVertexArray(NULL_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL_ID);
#pragma endregion Reset




	//<<渲染循环>>
#pragma region RenderPart
	while (!glfwWindowShouldClose(window)) {
		std::cerr << "Active Time: \r" << glfwGetTime() << std::flush;
		//<<处理输入>>
		processInput(window);
		int useUniform = enableUniform(window);
		//<<渲染函数>>
		//<<EBO方式绘制>>
		display(window, shaderProgram_ID[useUniform], VAO_ID, EBO_ID, useUniform);
		//<<事件触发器>>
		glfwPollEvents();

	}
#pragma endregion



	glfwTerminate();
	return 0;
}
