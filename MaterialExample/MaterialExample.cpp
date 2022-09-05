#include<iostream>
#include "../ShaderCleanCode/Shader.h"
#include "../CameraCleanCode/Camera.h"

//һЩ����
static int width = 800, height = 600;
const int NULL_ID = 0;
const int vertexNum = 36;
const int vertexAttriNum = 6;

//�����λ���뷽��
#pragma region CameraPosInfo
CameraUtils cameraEnv(width, height);
#pragma endregion CameraPosInfo

glm::vec3 LightPositions(1.2f, 1.0f, 2.0f);




//<<��Ⱦ����>>
static void display(GLFWwindow* window, unsigned int* shaderProgram_ID, unsigned int* VAO_ID)
{

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//<<������ɫ>>
	glm::vec3 lightColor;
	float time = glfwGetTime();
	lightColor.x = sin(time * 2.0f);
	lightColor.y = sin(time * 0.7f);
	lightColor.z = sin(time * 1.3f);

	glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);


	//���Ʋ�ͬ��cube
#pragma region drawCube
	glUseProgram(shaderProgram_ID[0]);
	glBindVertexArray(VAO_ID[0]);


	//����
	int loctmp = glGetUniformLocation(shaderProgram_ID[0], "material.ambient");
	glUniform3f(loctmp, 0.0215f, 0.1745f, 0.0215f);
	loctmp = glGetUniformLocation(shaderProgram_ID[0], "material.diffuse");
	glUniform3f(loctmp, 0.07568f, 0.61424f, 0.07568f);
	loctmp = glGetUniformLocation(shaderProgram_ID[0], "material.specular");
	glUniform3f(loctmp, 0.633f, 0.727811f, .633f);
	loctmp = glGetUniformLocation(shaderProgram_ID[0], "material.shininess");
	glUniform1f(loctmp, 76.8f);

	loctmp = glGetUniformLocation(shaderProgram_ID[0], "light.ambient");
	glUniform3f(loctmp, ambientColor.x, ambientColor.y, ambientColor.z);
	loctmp = glGetUniformLocation(shaderProgram_ID[0], "light.diffuse");
	glUniform3f(loctmp, diffuseColor.x, diffuseColor.y,diffuseColor.z);
	loctmp = glGetUniformLocation(shaderProgram_ID[0], "light.specular");
	glUniform3f(loctmp, 1.f, 1.f, 1.f);


	//��Դ�㣺������,���淴��
	loctmp = glGetUniformLocation(shaderProgram_ID[0], "light.position");
	glUniform3f(loctmp, LightPositions.x, LightPositions.y, LightPositions.z);

	//�۲�㣺���淴��
	loctmp = glGetUniformLocation(shaderProgram_ID[0], "eyePos");
	glUniform3f(loctmp, cameraEnv.camera.Pos.x, cameraEnv.camera.Pos.y, cameraEnv.camera.Pos.z);




	glm::mat4 model = glm::mat4(1.0f);
	int modelLoc = glGetUniformLocation(shaderProgram_ID[0], "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 view = cameraEnv.camera.GetViewMatrix();
	int viewLoc = glGetUniformLocation(shaderProgram_ID[0], "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));



	//͸��
	auto fovRadians = glm::radians(cameraEnv.camera.Zoom);
	auto aspect = 800.0f / 600.0f;
	auto nearPlane = 0.1f, farPlane = 100.0f;
	glm::mat4 projection;
	projection = glm::perspective(fovRadians, aspect, nearPlane, farPlane);
	int projLoc = glGetUniformLocation(shaderProgram_ID[0], "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, vertexNum);


#pragma endregion drawCube



	//���Ʋ�ͬ��cube
#pragma region drawLight
	glUseProgram(shaderProgram_ID[1]);
	glBindVertexArray(VAO_ID[1]);


	

	model = glm::mat4(1.0f);
	model = glm::translate(model, LightPositions);
	model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
	modelLoc = glGetUniformLocation(shaderProgram_ID[1], "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


	viewLoc = glGetUniformLocation(shaderProgram_ID[1], "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


	//͸��
	projLoc = glGetUniformLocation(shaderProgram_ID[1], "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, vertexNum);


#pragma endregion drawLight

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

//<<���λ��ӳ��ŷ����>>
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	cameraEnv.mouseMove(xpos, ypos);

}
//<<������ӳ��fov>>
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cameraEnv.mouseRoll(yoffset);
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
	//0������1�ǵ�
	unsigned int VAO_ID[2];
	int VAO_ID_NUM = 2;

	//VBO
	unsigned int VBO_ID;
	int VBO_ID_NUM = 1;

	//VertexShader
	Shader vertexShader("../shader/material_shader.vert", GL_VERTEX_SHADER);
	Shader vertexLightShader("../shader/material_light_shader.vert", GL_VERTEX_SHADER);


	//FragmentShader
	Shader fragShader("../shader/material_shader.frag", GL_FRAGMENT_SHADER);
	Shader fragLightShader("../shader/material_light_shader.frag", GL_FRAGMENT_SHADER);
	unsigned int vertexShader_ID = vertexShader.getId();
	unsigned int vertexLightShader_ID = vertexLightShader.getId();
	unsigned int fragShader_ID = fragShader.getId();
	unsigned int fragLightShader_ID = fragLightShader.getId();


	//Program
	ShaderProgram shaderProgram(vertexShader_ID, fragShader_ID);
	ShaderProgram lightShaderProgram(vertexLightShader_ID, fragLightShader_ID);
	//0������1�ǵ�
	unsigned int shaderProgram_ID[2]{ shaderProgram.getId(),lightShaderProgram.getId() };



#pragma endregion IDManaged

	//<<���ö����������ݽ��͵Ļ���>>
#pragma region SetVAO
	glGenVertexArrays(VAO_ID_NUM, VAO_ID);
	glBindVertexArray(VAO_ID[0]);
#pragma endregion SetVAO

	//<<���ö�������>>
	//+=<<��ӷ�����Ϣ>>
#pragma region SetVertex
	const int arrayNum = vertexNum * vertexAttriNum;
	float vertices[arrayNum] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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
	int normalInShader = 1;
	//��Ӧ�������ݵ�ά��
	int inShaderDataSize = 3;
	//�洢ǰ�Ƿ��һ��
	auto needNormalize = GL_FALSE;
	//ע����ƫ�����붥���������ݵĶ�Ӧ��ϵ
	glVertexAttribPointer(locationInShader, inShaderDataSize, GL_FLOAT, needNormalize, 6 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
	glVertexAttribPointer(normalInShader, inShaderDataSize, GL_FLOAT, needNormalize, 6 * sizeof(GL_FLOAT), BUFFER_OFFSET(3 * sizeof(GL_FLOAT)));
	//ע��Ҫ������Ӧ��λ��
	glEnableVertexAttribArray(locationInShader);
	glEnableVertexAttribArray(normalInShader);

	glBindVertexArray(VAO_ID[1]);
	glVertexAttribPointer(locationInShader, inShaderDataSize, GL_FLOAT, needNormalize, 6 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(locationInShader);
#pragma endregion ExplainVertex


	//<<���ó���������ɫ������>>
	//trap::+=<<�ڴ���uniform��ʱ�򣬱���Ҫ����Program>>
#pragma region UseProgram
	//glUseProgram(shaderProgram_ID[0]);
	glDeleteShader(vertexShader_ID);
	glDeleteShader(fragShader_ID);
#pragma endregion UseProgram







	//<<����buffer��λ>>
#pragma region Reset
	//��һ�����ݽ������λ
	glBindBuffer(GL_ARRAY_BUFFER, NULL_ID);
	glBindVertexArray(NULL_ID);
#pragma endregion Reset


	//<<��Ⱦѭ��>>
#pragma region RenderPart
	while (!glfwWindowShouldClose(window)) {

		//<<��������>>
		processInput(window);
		//<<��Ⱦ����>>

		display(window, shaderProgram_ID, VAO_ID);
		//<<�¼�������>>
		glfwPollEvents();

	}
#pragma endregion RenderPart



	glfwTerminate();
	return 0;
}
