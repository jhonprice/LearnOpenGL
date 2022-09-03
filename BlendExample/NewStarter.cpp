#include<iostream>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "../CameraCleanCode/Camera.h"
#include "../LightTextureExample/utils.h"
#include "../ShaderCleanCode/Shader.h"
#include <vector>

using namespace std;
static int width = 800, height = 600;

CameraUtils cameraEnv(width,height);



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

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	cameraEnv.wsad(window);
}
//<<鼠标位移映射欧拉角>>
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	cameraEnv.mouseMove(xpos, ypos);

}
//<<鼠标滚轮映射fov>>
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cameraEnv.mouseRoll(yoffset);
}


int main(int argc, char** argv)
{

	//<<初始化GLFW>>
#pragma region InitGLFW
	auto initValue = glfwInit();
	if (!initValue) std::cerr << "GLFW FAILED" << std::endl;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

//<<深度测试>>
#pragma region DepthTest
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glDepthMask(GL_FALSE);
#pragma endregion DepthTest


	


//<<视口和鼠标设置>>
#pragma region ViewportAndMouseSet
	glfwSetFramebufferSizeCallback(window, reshape);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
#pragma endregion ViewportAndMouseSet

#pragma region ShaderProgram
	Shader vertexShader("../shader/blend_test_shader.vert", GL_VERTEX_SHADER);
	Shader fragShader("../shader/blend_test_shader.frag", GL_FRAGMENT_SHADER);

	ShaderProgram shaderProgram(vertexShader.getId(), fragShader.getId());

	
#pragma endregion ShaderProgram

#pragma region SetVertex
	//VAO
	unsigned int VAO_ID;
	unsigned int PLANE_VAO_ID;
	unsigned int KUSA_VAO_ID;
	int VAO_ID_NUM = 1;
	
	//VBO
	unsigned int VBO_ID;
	unsigned int PLANE_VBO_ID;
	unsigned int KUSA_VBO_ID;
	int VBO_ID_NUM = 1;


	//draw cube
	glGenVertexArrays(VAO_ID_NUM, &VAO_ID);
	glGenBuffers(VBO_ID_NUM, &VBO_ID);
	glBindVertexArray(VAO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);

	float cubeVertices[] = {
		// positions          // texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glBindVertexArray(0);

	//draw plane
	glGenVertexArrays(VAO_ID_NUM, &PLANE_VAO_ID);
	glGenBuffers(VBO_ID_NUM, &PLANE_VBO_ID);
	glBindVertexArray(PLANE_VAO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, PLANE_VBO_ID);

	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);


	//draw kusa
	glGenVertexArrays(VAO_ID_NUM, &KUSA_VAO_ID);
	glGenBuffers(VBO_ID_NUM, &KUSA_VBO_ID);
	glBindVertexArray(KUSA_VAO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, KUSA_VBO_ID);

	float transparentVertices[] = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), &transparentVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	// transparent vegetation locations
	// --------------------------------
	vector<glm::vec3> vegetation
	{
		glm::vec3(-1.5f, 0.0f, -0.48f),
		glm::vec3(1.5f, 0.0f, 0.51f),
		glm::vec3(0.0f, 0.0f, 0.7f),
		glm::vec3(-0.3f, 0.0f, -2.3f),
		glm::vec3(0.5f, 0.0f, -0.6f)
	};

#pragma endregion SetVertex

#pragma region SetTexture
	unsigned int cubeTexture = loadTexture("../image/marble.jpg");
	unsigned int planeTexture = loadTexture("../image/metal.png");
	unsigned int kusaTexture = loadTexture("../image/grass.png");
	unsigned int winTexture = loadTexture("../image/window.png");
#pragma endregion SetTexture

//<<传递uniform变量(不变）>>
#pragma region StableUnform
	glUseProgram(shaderProgram.getId());
	shaderProgram.setInt("texture1", 0);
#pragma endregion


int kusaOrWindow = 1;
	//<<渲染循环>>
while (!glfwWindowShouldClose(window)) {
	cameraEnv.deltaUpdate(glfwGetTime());

	//<<渲染函数>>
#pragma region RenderFunc
	glfwSwapBuffers(window);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//清除深度缓冲区和模板缓冲区
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//深度可视化
	//mode=3  开启discard
	int mode = 3; //不进行深度可视化
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		mode = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		mode = 1; //非线性深度值
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
			mode = 2; //线性深度值
		}
	}
	shaderProgram.setInt("depthmode", mode);
	
	//transform
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = cameraEnv.camera.GetViewMatrix();
	glm::mat4 projection = cameraEnv.getPerpective();
	shaderProgram.setMat4("view", view);
	shaderProgram.setMat4("projection", projection);

	glStencilMask(0x00);
	// floor
	glBindVertexArray(PLANE_VAO_ID);
	glBindTexture(GL_TEXTURE_2D, planeTexture);
	shaderProgram.setMat4("model", glm::mat4(1.0f));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);


	//kusa 
	glBindVertexArray(KUSA_VAO_ID);
	glBindTexture(GL_TEXTURE_2D, kusaTexture);
	for (unsigned int i = 0; i < vegetation.size(); i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, vegetation[i]);
		shaderProgram.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glBindVertexArray(0);



	
	//cube
	glBindVertexArray(VAO_ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);
	//cube1
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	shaderProgram.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//cube2
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	shaderProgram.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	

#pragma endregion RenderFunc

	//<<事件触发器>>
	glfwPollEvents();
	//<<处理输入>>
	processInput(window);
}



	glfwTerminate();
	return 0;
}
