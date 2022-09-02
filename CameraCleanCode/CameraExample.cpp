#include<iostream>
#include "../ShaderCleanCode/Shader.h"
//GLM 头文件
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<stb_image.h>

//camera
#include "Camera.h"
#include "../MoreCubeAndZbuffer/build_world.h"

//一些常量
int width = 800, height = 600;
const int NULL_ID = 0;
const int vertexNum = 36;
const int vertexAttriNum = 5;

//相机的位置与方向
#pragma region CameraPosInfo
CameraUtils cameraEnv(width,height);
#pragma endregion CameraPosInfo






//<<渲染函数>>
void display(GLFWwindow* window, unsigned int shaderProgram_ID, int VAO_ID, unsigned int* TEXTURE_ID)
{
	std::cout << "\r";
	std::cout << "Camera Front: " << cameraEnv.camera.Front.x << " " << cameraEnv.camera.Front.y << " " << cameraEnv.camera.Front.z;
	std::cout<<"\t\t\t\tCamera Pos:" << cameraEnv.camera.Pos.x << " " << cameraEnv.camera.Pos.y << " " << cameraEnv.camera.Pos.z<<std::flush;
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


#pragma region SetUniform

	glActiveTexture(GL_TEXTURE0); //在一个纹理的情况下可以省略
	glBindTexture(GL_TEXTURE_2D, TEXTURE_ID[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TEXTURE_ID[1]);
#pragma endregion SetUniform

	glUseProgram(shaderProgram_ID);
	glBindVertexArray(VAO_ID);

	//绘制不同的cube
#pragma region drawCube

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	for (unsigned int i = 0; i < 10; i++) {

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i + 1.0f;
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		int modelLoc = glGetUniformLocation(shaderProgram_ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


		glm::mat4 view = cameraEnv.camera.GetViewMatrix();
		int viewLoc = glGetUniformLocation(shaderProgram_ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));



		//透射
		auto fovRadians = glm::radians(cameraEnv.camera.Zoom);
		auto aspect = static_cast<float>(width / height);
		auto nearPlane = 0.1f, farPlane = 100.0f;
		glm::mat4 projection = glm::perspective(fovRadians, aspect, nearPlane, farPlane);
		int projLoc = glGetUniformLocation(shaderProgram_ID, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, vertexNum);
	}

#pragma endregion drawCube

	glfwSwapBuffers(window);
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

#pragma region F_LINE
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
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
	//VAO
	unsigned int VAO_ID;
	int VAO_ID_NUM = 1;

	//VBO
	unsigned int VBO_ID;
	int VBO_ID_NUM = 1;

	//VertexShader
	Shader vertexShader("../shader/camera_shader.vert", GL_VERTEX_SHADER);


	//FragmentShader
	Shader fragShader("../shader/camera_shader.frag", GL_FRAGMENT_SHADER);
	unsigned int vertexShader_ID = vertexShader.getId();
	unsigned int fragShader_ID = fragShader.getId();


	//Program
	ShaderProgram shaderProgram(vertexShader_ID, fragShader_ID);
	unsigned int shaderProgram_ID = shaderProgram.getId();


	//Texture
	unsigned int texture_ID;
	unsigned int texture_ID_2;
	int texture_ID_NUM = 1;

#pragma endregion IDManaged

	//<<设置顶点数据内容解释的缓存>>
#pragma region SetVAO
	glGenVertexArrays(VAO_ID_NUM, &VAO_ID);
	glBindVertexArray(VAO_ID);
#pragma endregion SetVAO

	//<<设置顶点坐标>>
#pragma region SetVertex
	const int arrayNum = vertexNum * vertexAttriNum;
	float vertices[arrayNum];
	setWorld(vertices, arrayNum);


	//<<生成buffer，绑定buffer，传值，还原buffer>>
	glGenBuffers(VBO_ID_NUM, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
#pragma endregion SetVertex







	//<<解释顶点数据内容>>
#pragma region ExplainVertex
	//layout中指定的location
	int locationInShader = 0;
	int textureInShader = 1;
	//对应传入数据的维度
	int inShaderDataSize = 3;
	int texture_inShaderDataSize = 2;
	//存储前是否归一化
	auto needNormalize = GL_FALSE;
	//注意多个偏移量与顶点数组数据的对应关系
	glVertexAttribPointer(locationInShader, inShaderDataSize, GL_FLOAT, needNormalize, 5 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
	glVertexAttribPointer(textureInShader, texture_inShaderDataSize, GL_FLOAT, needNormalize, 5 * sizeof(GL_FLOAT), BUFFER_OFFSET(3 * sizeof(GL_FLOAT)));
	//注意要启用相应的位置
	glEnableVertexAttribArray(locationInShader);
	glEnableVertexAttribArray(textureInShader);
#pragma endregion ExplainVertex


	//<<启用程序，清理着色器代码>>
	//trap::+=<<在传递uniform的时候，必须要激活Program>>
#pragma region UseProgram
	glUseProgram(shaderProgram_ID);
	glDeleteShader(vertexShader_ID);
	glDeleteShader(fragShader_ID);
#pragma endregion UseProgram


	//<<加载图片>>
#pragma region Textures1
	int t_width, t_height, t_nrChannels;
	//<<图片翻转>>
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("../image/container.jpg", &t_width, &t_height, &t_nrChannels, 0);

	//<<生成,绑定,输入>>
	glGenTextures(texture_ID_NUM, &texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);

	//<<设置纹理属性>>
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//<<传入图片数据，并生成mipmap>>
	int notSetMipMaplevel = 0;
	if (data)
	{
		//不手动指定mipmap等级
		glTexImage2D(GL_TEXTURE_2D, notSetMipMaplevel, GL_RGB, t_width, t_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
#pragma endregion Textures1

#pragma region Textures2

	data = stbi_load("../image/awesomeface.png", &t_width, &t_height, &t_nrChannels, 0);

	//<<生成,绑定,输入>>
	glGenTextures(texture_ID_NUM, &texture_ID_2);
	glBindTexture(GL_TEXTURE_2D, texture_ID_2);

	//<<设置纹理属性>>
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//<<传入图片数据，并生成mipmap>>
	if (data)
	{
		//不手动指定mipmap等级
		glTexImage2D(GL_TEXTURE_2D, notSetMipMaplevel, GL_RGBA, t_width, t_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


#pragma endregion Textures2

	//<<多个纹理需要绑定uniform>>
#pragma region  UniformTexture
	auto uniLoc1 = glGetUniformLocation(shaderProgram_ID, "texture1");
	auto uniLoc2 = glGetUniformLocation(shaderProgram_ID, "texture2");
	glUniform1i(uniLoc1, 0);
	glUniform1i(uniLoc2, 1);
#pragma endregion UniformTexture

	//<<传递变换矩阵>>
#pragma region UniformMatrixs

#pragma endregion





	//<<各种buffer归位>>
#pragma region Reset
	//在一批数据结束后归位
	glBindBuffer(GL_ARRAY_BUFFER, NULL_ID);
	glBindVertexArray(NULL_ID);
	glBindTexture(GL_TEXTURE_2D, NULL_ID);
#pragma endregion Reset


	//<<渲染循环>>
#pragma region RenderPart
	while (!glfwWindowShouldClose(window)) {
		//<<处理输入>>
		processInput(window);
		//<<渲染函数>>
		//<<EBO方式绘制>>
		unsigned int textureIDs[2]{ texture_ID,texture_ID_2 };
		display(window, shaderProgram_ID, VAO_ID, textureIDs);
		//<<事件触发器>>
		glfwPollEvents();

	}
#pragma endregion RenderPart



	glfwTerminate();
	return 0;
}
