#include<iostream>
#include "../ShaderCleanCode/Shader.h"
#include <stb_image.h>
//GLM 头文件
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//一些常量
static int width = 800, height = 600;
const int NULL_ID = 0;
const int vertexNum = 36;
const int vertexAttriNum = 3;

//相机的位置与方向
#pragma region CameraPosInfo
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//控制速度
float deltaTime = 0.0f;
float lastFrame = 0.0f;


//相机欧拉角
float lastX = width / 2, lastY = height / 2;
float yaw = 0, pitch = 0;
bool firstMouse = true;


//fov
float fov = 45.0f;
#pragma endregion CameraPosInfo






//<<渲染函数>>
static void display(GLFWwindow* window, unsigned int* shaderProgram_ID, unsigned int* VAO_ID)
{

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	//绘制不同的cube
#pragma region drawCube
	glUseProgram(shaderProgram_ID[0]);
	glBindVertexArray(VAO_ID[0]);



	int loctmp = glGetUniformLocation(shaderProgram_ID[0], "objectColor");
	glUniform3f(loctmp, 1.f, .5f, .31f);

	loctmp = glGetUniformLocation(shaderProgram_ID[0], "lightColor");
	glUniform3f(loctmp, 1.f, 1.f, 1.f);

	glm::mat4 model = glm::mat4(1.0f);
	int modelLoc = glGetUniformLocation(shaderProgram_ID[0], "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	int viewLoc = glGetUniformLocation(shaderProgram_ID[0], "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));



	//透射
	auto fovRadians = glm::radians(fov);
	auto aspect = 800.0f / 600.0f;
	auto nearPlane = 0.1f, farPlane = 100.0f;
	glm::mat4 projection;
	projection = glm::perspective(fovRadians, aspect, nearPlane, farPlane);
	int projLoc = glGetUniformLocation(shaderProgram_ID[0], "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, vertexNum);


#pragma endregion drawCube



	//绘制不同的cube
#pragma region drawLight
	glUseProgram(shaderProgram_ID[1]);
	glBindVertexArray(VAO_ID[1]);


	glm::vec3 cubePositions(1.2f, 1.0f, 2.0f);

	model = glm::mat4(1.0f);
	model = glm::translate(model, cubePositions);
	float angle = 45.0f;
	model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
	modelLoc = glGetUniformLocation(shaderProgram_ID[1], "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


	viewLoc = glGetUniformLocation(shaderProgram_ID[1], "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


	//透射
	projLoc = glGetUniformLocation(shaderProgram_ID[1], "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, vertexNum);


#pragma endregion drawLight

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

#pragma region WASD_MOVE
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
#pragma endregion WASD_MOVE


}

//<<鼠标位移映射欧拉角>>
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	//确定位移
	float xoffset = xpos - lastX;

	//由于y的坐标是从底到顶的
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	//控制范围
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	//欧拉角转坐标修改相机朝向
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
	std::cout << "\r" << cameraFront.x << " " << cameraFront.y << " " << cameraFront.z << std::flush;

}
//<<鼠标滚轮映射fov>>
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
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
#pragma endregion InitGLFW


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
#pragma endregion InitWindows


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
#pragma endregion InitGl3W


	//<<视口设置 & 深度测试设置>>
#pragma region ViewPortAndDepthSet
	glfwSetFramebufferSizeCallback(window, reshape);
	glEnable(GL_DEPTH_TEST);
#pragma endregion ViewPortAndDepthSet

	//<<鼠标设置>>
#pragma region MouseInput
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	//鼠标滚轮
	glfwSetScrollCallback(window, scroll_callback);
#pragma endregion MouseInput



	//<<OpenGL对象的各种ID>>
#pragma region IDManaged
	//VAO
	//0是物体1是灯
	unsigned int VAO_ID[2];
	int VAO_ID_NUM = 2;

	//VBO
	unsigned int VBO_ID;
	int VBO_ID_NUM = 1;

	//VertexShader
	Shader vertexShader("../shader/color_shader.vert", GL_VERTEX_SHADER);


	//FragmentShader
	Shader fragShader("../shader/color_shader.frag", GL_FRAGMENT_SHADER);
	Shader fragLightShader("../shader/color_light_shader.frag", GL_FRAGMENT_SHADER);
	unsigned int vertexShader_ID = vertexShader.getId();
	unsigned int fragShader_ID = fragShader.getId();
	unsigned int fragLightShader_ID = fragLightShader.getId();


	//Program
	ShaderProgram shaderProgram(vertexShader_ID, fragShader_ID);
	ShaderProgram lightShaderProgram(vertexShader_ID, fragLightShader_ID);
	//0是物体1是灯
	unsigned int shaderProgram_ID[2]{ shaderProgram.getId(),lightShaderProgram.getId() };



#pragma endregion IDManaged

	//<<设置顶点数据内容解释的缓存>>
#pragma region SetVAO
	glGenVertexArrays(VAO_ID_NUM, VAO_ID);
	glBindVertexArray(VAO_ID[0]);
#pragma endregion SetVAO

	//<<设置顶点坐标>>
#pragma region SetVertex
	const int arrayNum = vertexNum * vertexAttriNum;
	float vertices[arrayNum] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
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
	//注意多个偏移量与顶点数组数据的对应关系
	glVertexAttribPointer(locationInShader, inShaderDataSize, GL_FLOAT, needNormalize, 3 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
	//注意要启用相应的位置
	glEnableVertexAttribArray(locationInShader);

	glBindVertexArray(VAO_ID[1]);
	glVertexAttribPointer(locationInShader, inShaderDataSize, GL_FLOAT, needNormalize, 3 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(locationInShader);
#pragma endregion ExplainVertex


	//<<启用程序，清理着色器代码>>
	//trap::+=<<在传递uniform的时候，必须要激活Program>>
#pragma region UseProgram
	//glUseProgram(shaderProgram_ID[0]);
	glDeleteShader(vertexShader_ID);
	glDeleteShader(fragShader_ID);
#pragma endregion UseProgram







//<<各种buffer归位>>
#pragma region Reset
	//在一批数据结束后归位
	glBindBuffer(GL_ARRAY_BUFFER, NULL_ID);
	glBindVertexArray(NULL_ID);
#pragma endregion Reset


	//<<渲染循环>>
#pragma region RenderPart
	while (!glfwWindowShouldClose(window)) {
		//std::cerr << "\rActive Time: " << glfwGetTime() << std::flush;
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//<<处理输入>>
		processInput(window);
		//<<渲染函数>>

		display(window, shaderProgram_ID, VAO_ID);
		//<<事件触发器>>
		glfwPollEvents();

	}
#pragma endregion RenderPart



	glfwTerminate();
	return 0;
}
