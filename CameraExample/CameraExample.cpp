#include<iostream>
#include "../ShaderCleanCode/Shader.h"
#include <stb_image.h>
//GLM ͷ�ļ�
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//build world
#include "../MoreCubeAndZbuffer/build_world.h"

//һЩ����
static int width = 800, height = 600;
const int NULL_ID = 0;
const int vertexNum = 36;
const int vertexAttriNum = 5;

//�����λ���뷽��
#pragma region CameraPosInfo
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//�����ٶ�
float deltaTime = 0.0f;	
float lastFrame = 0.0f; 


//���ŷ����
float lastX = width/2, lastY = height/2;
float yaw=0 ,pitch=0;
bool firstMouse = true;


//fov
float fov = 45.0f;
#pragma endregion CameraPosInfo






//<<��Ⱦ����>>
static void display(GLFWwindow* window, unsigned int shaderProgram_ID, int VAO_ID, unsigned int* TEXTURE_ID)
{

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


#pragma region SetUniform

	glActiveTexture(GL_TEXTURE0); //��һ�����������¿���ʡ��
	glBindTexture(GL_TEXTURE_2D, TEXTURE_ID[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TEXTURE_ID[1]);
#pragma endregion SetUniform

	glUseProgram(shaderProgram_ID);
	glBindVertexArray(VAO_ID);

	//���Ʋ�ͬ��cube
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

		glm::mat4 view = glm::mat4(1.0f);
		if (0) {
			const float radius = 10.0f;
			float camX = sin(glfwGetTime()) * radius;
			float camZ = cos(glfwGetTime()) * radius;
			view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		}
		else {
			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		}
		int viewLoc = glGetUniformLocation(shaderProgram_ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));



		//͸��
		auto fovRadians = glm::radians(fov);
		auto aspect = 800.0f / 600.0f;
		auto nearPlane = 0.1f, farPlane = 100.0f;
		glm::mat4 projection;
		projection = glm::perspective(fovRadians, aspect, nearPlane, farPlane);
		int projLoc = glGetUniformLocation(shaderProgram_ID, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, vertexNum);
	}

#pragma endregion drawCube

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

//<<���λ��ӳ��ŷ����>>
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	//ȷ��λ��
	float xoffset = xpos - lastX;

	//����y�������Ǵӵ׵�����
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	//���Ʒ�Χ
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	//ŷ����ת�����޸��������
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
	std::cout<< "\r"<<cameraFront.x << " " << cameraFront.y << " " << cameraFront.z << std::flush;

}
//<<������ӳ��fov>>
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


	//<<��ʼ��GLFW>>
#pragma region InitGLFW
	auto initValue = glfwInit();
	if (!initValue) std::cerr << "GLFW FAILED" << std::endl;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#pragma endregion InitGLFW


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
#pragma endregion InitWindows


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
#pragma endregion InitGl3W


	//<<�ӿ����� & ��Ȳ�������>>
#pragma region ViewPortAndDepthSet
	glfwSetFramebufferSizeCallback(window, reshape);
	glEnable(GL_DEPTH_TEST);
#pragma endregion ViewPortAndDepthSet

	//<<�������>>
#pragma region MouseInput
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	//������
	glfwSetScrollCallback(window, scroll_callback);
#pragma endregion MouseInput



	//<<OpenGL����ĸ���ID>>
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

	//<<���ö����������ݽ��͵Ļ���>>
#pragma region SetVAO
	glGenVertexArrays(VAO_ID_NUM, &VAO_ID);
	glBindVertexArray(VAO_ID);
#pragma endregion SetVAO

	//<<���ö�������>>
#pragma region SetVertex
	const int arrayNum = vertexNum * vertexAttriNum;
	float vertices[arrayNum];
	setWorld(vertices, arrayNum);


	//<<����buffer����buffer����ֵ����ԭbuffer>>
	glGenBuffers(VBO_ID_NUM, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
#pragma endregion SetVertex







	//<<���Ͷ�����������>>
#pragma region ExplainVertex
	//layout��ָ����location
	int locationInShader = 0;
	int textureInShader = 1;
	//��Ӧ�������ݵ�ά��
	int inShaderDataSize = 3;
	int texture_inShaderDataSize = 2;
	//�洢ǰ�Ƿ��һ��
	auto needNormalize = GL_FALSE;
	//ע����ƫ�����붥���������ݵĶ�Ӧ��ϵ
	glVertexAttribPointer(locationInShader, inShaderDataSize, GL_FLOAT, needNormalize, 5 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
	glVertexAttribPointer(textureInShader, texture_inShaderDataSize, GL_FLOAT, needNormalize, 5 * sizeof(GL_FLOAT), BUFFER_OFFSET(3 * sizeof(GL_FLOAT)));
	//ע��Ҫ������Ӧ��λ��
	glEnableVertexAttribArray(locationInShader);
	glEnableVertexAttribArray(textureInShader);
#pragma endregion ExplainVertex


	//<<���ó���������ɫ������>>
	//trap::+=<<�ڴ���uniform��ʱ�򣬱���Ҫ����Program>>
#pragma region UseProgram
	glUseProgram(shaderProgram_ID);
	glDeleteShader(vertexShader_ID);
	glDeleteShader(fragShader_ID);
#pragma endregion UseProgram


	//<<����ͼƬ>>
#pragma region Textures1
	int t_width, t_height, t_nrChannels;
	//<<ͼƬ��ת>>
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("../image/container.jpg", &t_width, &t_height, &t_nrChannels, 0);

	//<<����,��,����>>
	glGenTextures(texture_ID_NUM, &texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);

	//<<������������>>
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//<<����ͼƬ���ݣ�������mipmap>>
	int notSetMipMaplevel = 0;
	if (data)
	{
		//���ֶ�ָ��mipmap�ȼ�
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

	//<<����,��,����>>
	glGenTextures(texture_ID_NUM, &texture_ID_2);
	glBindTexture(GL_TEXTURE_2D, texture_ID_2);

	//<<������������>>
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//<<����ͼƬ���ݣ�������mipmap>>
	if (data)
	{
		//���ֶ�ָ��mipmap�ȼ�
		glTexImage2D(GL_TEXTURE_2D, notSetMipMaplevel, GL_RGBA, t_width, t_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


#pragma endregion Textures2

	//<<���������Ҫ��uniform>>
#pragma region  UniformTexture
	auto uniLoc1 = glGetUniformLocation(shaderProgram_ID, "texture1");
	auto uniLoc2 = glGetUniformLocation(shaderProgram_ID, "texture2");
	glUniform1i(uniLoc1, 0);
	glUniform1i(uniLoc2, 1);
#pragma endregion UniformTexture

	//<<���ݱ任����>>
#pragma region UniformMatrixs

#pragma endregion





	//<<����buffer��λ>>
#pragma region Reset
	//��һ�����ݽ������λ
	glBindBuffer(GL_ARRAY_BUFFER, NULL_ID);
	glBindVertexArray(NULL_ID);
	glBindTexture(GL_TEXTURE_2D, NULL_ID);
#pragma endregion Reset


	//<<��Ⱦѭ��>>
#pragma region RenderPart
	while (!glfwWindowShouldClose(window)) {
		//std::cerr << "\rActive Time: " << glfwGetTime() << std::flush;
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//<<��������>>
		processInput(window);
		//<<��Ⱦ����>>
		//<<EBO��ʽ����>>
		unsigned int textureIDs[2]{ texture_ID,texture_ID_2 };
		display(window, shaderProgram_ID, VAO_ID, textureIDs);
		//<<�¼�������>>
		glfwPollEvents();

	}
#pragma endregion RenderPart



	glfwTerminate();
	return 0;
}
