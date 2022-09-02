#include<iostream>
#include "../ShaderCleanCode/Shader.h"
//GLM 头文件
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../LightTextureExample/utils.h"

//camera
#include "../CameraCleanCode/Camera.h"
#include "model.h"


//一些常量
int width = 800, height = 600;
const int NULL_ID = 0;

//相机的位置与方向
#pragma region CameraPosInfo
CameraUtils cameraEnv(width, height);


glm::vec3 LightPositions(-2.2f, 1.5f, -5.0f);
//方向光
glm::vec3 LightDirection(-0.2f, -1.0f, -0.3f);
#pragma endregion CameraPosInfo


//<<渲染函数>>
void display(GLFWwindow* window, unsigned int* shaderProgram_ID)
{

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	int nowProgam = 5;
	glUseProgram(shaderProgram_ID[nowProgam]);
	glUniform3f(glGetUniformLocation(shaderProgram_ID[nowProgam], "light.position"),
		cameraEnv.camera.Pos.x, cameraEnv.camera.Pos.y, cameraEnv.camera.Pos.z);
	glUniform3f(glGetUniformLocation(shaderProgram_ID[nowProgam], "light.direction"),
		cameraEnv.camera.Front.x, cameraEnv.camera.Front.y, cameraEnv.camera.Front.z);
	glUniform1f(glGetUniformLocation(shaderProgram_ID[nowProgam], "light.cutOff"),
		glm::cos(glm::radians(12.5f)));
	glUniform1f(glGetUniformLocation(shaderProgram_ID[nowProgam], "light.outerCutOff"),
		glm::cos(glm::radians(17.5f)));


	glUniform1f(glGetUniformLocation(shaderProgram_ID[nowProgam], "light.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram_ID[nowProgam], "light.linear"), 0.09f);
	glUniform1f(glGetUniformLocation(shaderProgram_ID[nowProgam], "light.quadratic"), 0.032f);
	



	//<<设置材质，光，观察点（通用）>>
#pragma region MaterialAndLightAndEyes
	//<<光线颜色>>
	glm::vec3 lightColor(1.f, 1.f, 1.f);
	glm::vec3 diffuseColor = lightColor * glm::vec3(0.8f);
	glm::vec3 ambientColor = lightColor * glm::vec3(0.1f);

	glm::mat4 model = glm::mat4(1.0f);
	float angle = 20.0f;
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	int modelLoc = glGetUniformLocation(shaderProgram_ID[nowProgam], "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


	glm::mat4 view = cameraEnv.camera.GetViewMatrix();
	int viewLoc = glGetUniformLocation(shaderProgram_ID[nowProgam], "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


	
	//透射
	glm::mat4 projection = cameraEnv.getPerpective();
	int projLoc = glGetUniformLocation(shaderProgram_ID[nowProgam], "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


	//光源
	int tmpLoc = glGetUniformLocation(shaderProgram_ID[nowProgam], "light.ambient");
	glUniform3f(tmpLoc, ambientColor.x, ambientColor.y, ambientColor.z);
	tmpLoc = glGetUniformLocation(shaderProgram_ID[nowProgam], "light.diffuse");
	glUniform3f(tmpLoc, diffuseColor.x, diffuseColor.y, diffuseColor.z);
	tmpLoc = glGetUniformLocation(shaderProgram_ID[nowProgam], "light.specular");
	glUniform3f(tmpLoc, lightColor.x, lightColor.y, lightColor.z);


	//观察点：镜面反射
	tmpLoc = glGetUniformLocation(shaderProgram_ID[nowProgam], "eyePos");
	glUniform3f(tmpLoc, cameraEnv.camera.Pos.x, cameraEnv.camera.Pos.y, cameraEnv.camera.Pos.z);
#pragma endregion MaterialAndLightAndEyes








}

//<<设置视口>>
void reshape(GLFWwindow* window, int w, int h)
{
	width = w > 1 ? w : 1;
	height = h > 1 ? h : 1;
	glViewport(0, 0, width, height);
}
//<<处理输入>>
void processInput(GLFWwindow* window) {
	cameraEnv.deltaUpdate(glfwGetTime());
#pragma region Esc_CLose
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
#pragma endregion Esc_CLose

#pragma region G_LINE
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
#pragma endregion F_LINE

#pragma region WASD_MOVE
	cameraEnv.wsad(window);
#pragma endregion WASD_MOVE


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
	

	//VertexShader
	Shader vertexShader("../shader/model_shader.vert", GL_VERTEX_SHADER);
	unsigned int vertexShader_ID = vertexShader.getId();



	Shader flashFragShader("../shader/model_shader.frag", GL_FRAGMENT_SHADER);
	unsigned int flashFragShader_ID = flashFragShader.getId();


	//Program
	ShaderProgram flashLightshaderProgram(vertexShader_ID, flashFragShader_ID);

	unsigned int shaderProgram_ID[6]{ flashLightshaderProgram.getId(),
		flashLightshaderProgram.getId(),
		flashLightshaderProgram.getId(),flashLightshaderProgram.getId(),flashLightshaderProgram.getId(),flashLightshaderProgram.getId() };

	Model ourModel("../image/backpack/backpack.obj");
#pragma endregion IDManaged




	//<<渲染循环>>
#pragma region RenderPart
	while (!glfwWindowShouldClose(window)) {
		//<<处理输入>>
		processInput(window);
		//<<渲染函数>>
		//<<EBO方式绘制>>

		display(window, shaderProgram_ID);
		ourModel.Draw(flashLightshaderProgram);
		glfwSwapBuffers(window);
		//<<事件触发器>>
		glfwPollEvents();

	}
#pragma endregion RenderPart



	glfwTerminate();
	return 0;
}
