#include<iostream>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "../CameraCleanCode/Camera.h"
#include "../LightTextureExample/utils.h"
#include "../ShaderCleanCode/Shader.h"

using namespace std;
static int width = 800, height = 600;

CameraUtils cameraEnv(width,height);
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
int blinn = 1;

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

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		blinn = 1;
	}
	else {
		blinn = 0;
	}
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
	Shader vertexShader("../shader/blinn_phong_shader.vert", GL_VERTEX_SHADER);
	Shader fragShader("../shader/blinn_phong_shader.frag", GL_FRAGMENT_SHADER);

	ShaderProgram shaderProgram(vertexShader.getId(), fragShader.getId());

	
#pragma endregion ShaderProgram

#pragma region SetVertex
	//VAO
	unsigned int PLANE_VAO_ID;
	int VAO_ID_NUM = 1;
	
	//VBO
	unsigned int PLANE_VBO_ID;
	int VBO_ID_NUM = 1;


	//draw plane
	glGenVertexArrays(VAO_ID_NUM, &PLANE_VAO_ID);
	glGenBuffers(VBO_ID_NUM, &PLANE_VBO_ID);
	glBindVertexArray(PLANE_VAO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, PLANE_VBO_ID);

	float planeVertices[] = {
		// positions            // normals         // texcoords
		 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

		 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
		 10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);

#pragma endregion SetVertex

#pragma region SetTexture
	unsigned int planeTexture = loadTexture("../image/wood.png");
#pragma endregion SetTexture

//<<传递uniform变量(不变）>>
#pragma region StableUnform
	glUseProgram(shaderProgram.getId());
	shaderProgram.setInt("texture1", 0);
#pragma endregion



	//<<渲染循环>>
while (!glfwWindowShouldClose(window)) {
	cameraEnv.deltaUpdate(glfwGetTime());

	//<<渲染函数>>
#pragma region RenderFunc
	glfwSwapBuffers(window);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//清除深度缓冲区
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	//transform
	glm::mat4 view = cameraEnv.camera.GetViewMatrix();
	glm::mat4 projection = cameraEnv.getPerpective();
	shaderProgram.setMat4("view", view);
	shaderProgram.setMat4("projection", projection);

	// set light uniforms
	shaderProgram.setVec3("viewPos", cameraEnv.camera.Pos.x, cameraEnv.camera.Pos.y, cameraEnv.camera.Pos.z);
	shaderProgram.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
	shaderProgram.setInt("blinn", blinn);


	// floor
	glBindVertexArray(PLANE_VAO_ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, planeTexture);
	shaderProgram.setMat4("model", glm::mat4(1.0f));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

#pragma endregion RenderFunc

	//<<事件触发器>>
	glfwPollEvents();
	//<<处理输入>>
	processInput(window);
}



	glfwTerminate();
	return 0;
}
