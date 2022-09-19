#include<iostream>
#include "../ShaderCleanCode/Shader.h"
#include <stb_image.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
static int width = 600, height = 600;
const int NULL_ID = 0;
int y = 0;
//<<��Ⱦ����>>
//<<EBO��ʽ����>>
//<<����Uniformȫ�ֱ���>>
static void display(GLFWwindow* window, unsigned int shaderProgram_ID, int VAO_ID, int EBO_ID, unsigned int* TEXTURE_ID)
{

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

#pragma region D_MOVE
	
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		y += 1;
		y = y % 200;
		std::cerr<<"Y value:" << y << std::endl;
	}
#pragma endregion F_LD_MOVEINE

#pragma region SetUniform

	glActiveTexture(GL_TEXTURE0); //��һ�����������¿���ʡ��
	glBindTexture(GL_TEXTURE_2D, TEXTURE_ID[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TEXTURE_ID[1]);
#pragma endregion SetUniform
	glm::mat4 transform = glm::mat4(1.0f);
	
	transform = glm::translate(transform, glm::vec3(0.5f, (y-100) /100.0f, 0.0f));
	transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

	unsigned int transformLoc = glGetUniformLocation(shaderProgram_ID, "transform");
	int numMat = 1;
	auto needTransMat = GL_FALSE;
	glUniformMatrix4fv(transformLoc, numMat, needTransMat, glm::value_ptr(transform));
#pragma region SetRotation

#pragma endregion


	glUseProgram(shaderProgram_ID);
	glBindVertexArray(VAO_ID);
	//<<EBO��ʽ����>>
	int drawLen = 6;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ID);

	auto indexArrayType = GL_UNSIGNED_INT;
	auto indexArrayOffset = 0;
	glDrawElements(GL_TRIANGLES, drawLen, indexArrayType, (void*)(indexArrayOffset));

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
//<<GLM TEST>>
#pragma region GLMTEST
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans = glm::mat4(1.0f);

	//translate
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << trans[i][j] << " ";
		}
		std::cout << std::endl;
	}
	vec = trans * vec;
	std::cout << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << std::endl;
	//rotate
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	//scale
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
#pragma endregion GLMTEST



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


	//<<�ӿ�����>>
#pragma region ViewPortSet
	glfwSetFramebufferSizeCallback(window, reshape);
#pragma endregion ViewPortSet

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
	Shader vertexShader("../shader/glm_shader.vert", GL_VERTEX_SHADER);


	//FragmentShader
	//unsigned int fragShader_ID[2];
	//int fragShader_ID_NUM = 1;
	Shader fragShader1("../shader/glm_shader.frag", GL_FRAGMENT_SHADER);
	//Shader fragShader2("../shader/clean_shader_uniform.frag", GL_FRAGMENT_SHADER);
	unsigned int vertexShader_ID = vertexShader.getId();
	unsigned int fragShader_ID[2]{ fragShader1.getId(),fragShader1.getId() };


	//Program
	//unsigned int shaderProgram_ID[2];
	ShaderProgram shaderProgram1(vertexShader_ID, fragShader_ID[0]);
	//ShaderProgram shaderProgram2(vertexShader_ID, fragShader_ID[1]);
	unsigned int shaderProgram_ID[2]{ shaderProgram1.getId(),shaderProgram1.getId() };

	//EBO
	unsigned int EBO_ID;
	int EBO_ID_NUM = 1;

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
	//+=<<��ά������>>
#pragma region SetVertex
	float vertices[] = {
	 0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,// top right //blue //texture
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,// bottom right //red //texture
	-0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,// bottom left //black //texture
	-0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top left  //green //texture
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
	int textureInShader = 1;
	//��Ӧ�������ݵ�ά��
	int inShaderDataSize = 3;
	int texture_inShaderDataSize = 2;
	//�洢ǰ�Ƿ��һ��
	auto needNormalize = GL_FALSE;
	//ע����ƫ�����붥���������ݵĶ�Ӧ��ϵ
	glVertexAttribPointer(locationInShader, inShaderDataSize, GL_FLOAT, needNormalize, 8 * sizeof(GL_FLOAT), (void*)(0));
	glVertexAttribPointer(textureInShader, texture_inShaderDataSize, GL_FLOAT, needNormalize, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
	//ע��Ҫ������Ӧ��λ��
	glEnableVertexAttribArray(locationInShader);
	glEnableVertexAttribArray(textureInShader);
#pragma endregion ExplainVertex


	//<<���ó���������ɫ������>>
	//trap::+=<<�ڴ���texture��ʱ�򣬱���Ҫ����Program>>
#pragma region UseProgram
	glUseProgram(shaderProgram_ID[0]);
	glDeleteShader(vertexShader_ID);
	glDeleteShader(fragShader_ID[0]);
	//glDeleteShader(fragShader_ID[1]);
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
	auto uniLoc1 = glGetUniformLocation(shaderProgram_ID[0], "texture1");
	auto uniLoc2 = glGetUniformLocation(shaderProgram_ID[0], "texture2");
	glUniform1i(uniLoc1, 0);
	glUniform1i(uniLoc2, 1);
#pragma endregion UniformTexture




	//<<����buffer��λ>>
#pragma region Reset
	//��һ�����ݽ������λ
	glBindBuffer(GL_ARRAY_BUFFER, NULL_ID);
	glBindVertexArray(NULL_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL_ID);
	glBindTexture(GL_TEXTURE_2D, NULL_ID);
#pragma endregion Reset

	//<<��Ⱦѭ��>>
#pragma region RenderPart
	while (!glfwWindowShouldClose(window)) {
		std::cerr << "\rActive Time: " << glfwGetTime() << std::flush;
		//<<��������>>
		processInput(window);
		//<<��Ⱦ����>>
		//<<EBO��ʽ����>>
		unsigned int textureIDs[2]{ texture_ID,texture_ID_2 };
		display(window, shaderProgram_ID[0], VAO_ID, EBO_ID, textureIDs);
		//<<�¼�������>>
		glfwPollEvents();

	}
#pragma endregion RenderPart



	glfwTerminate();
	return 0;
}
