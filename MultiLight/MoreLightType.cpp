#include<iostream>
#include "../ShaderCleanCode/Shader.h"
//GLM 头文件
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../LightTextureExample/utils.h"

//camera
#include "../CameraCleanCode/Camera.h"


//一些常量
int width = 800, height = 600;
const int NULL_ID = 0;
const int vertexNum = 36;
const int vertexAttriNum = 8;

//相机的位置与方向
#pragma region CameraPosInfo
CameraUtils cameraEnv(width,height);

glm::vec3 LightPositions(-2.2f, 1.5f, -5.0f);
//方向光
glm::vec3 LightDirection(-0.2f, -1.0f, -0.3f);
#pragma endregion CameraPosInfo


//<<渲染函数>>


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
#pragma endregion G_LINE

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
	unsigned int VAO_ID[2];
	int VAO_ID_NUM = 2;

	//VBO
	unsigned int VBO_ID;
	int VBO_ID_NUM = 1;

	//VertexShader
	Shader vertexShader("../shader/light_type_shader.vert", GL_VERTEX_SHADER);
	Shader lightVertexShader("../shader/lightmode_light_shader.vert", GL_VERTEX_SHADER);


	//FragmentShader
	Shader fragShader("../shader/multi_light_shader.frag", GL_FRAGMENT_SHADER);
	Shader lightFragShader("../shader/lightmode_light_shader.frag", GL_FRAGMENT_SHADER);


	unsigned int vertexShader_ID = vertexShader.getId();
	unsigned int lightVertexShader_ID = lightVertexShader.getId();

	unsigned int fragShader_ID = fragShader.getId();
	unsigned int lightFragShader_ID = lightFragShader.getId();
	


	//Program
	ShaderProgram shaderProgram(vertexShader_ID, fragShader_ID);
	ShaderProgram lightShaderProgram(lightVertexShader_ID, lightFragShader_ID);
	unsigned int shaderProgram_ID = shaderProgram.getId();
	unsigned int lightShaderProgram_ID = lightShaderProgram.getId();

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
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};


	//<<生成buffer，绑定buffer，传值，还原buffer>>
	glGenBuffers(VBO_ID_NUM, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


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


	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
#pragma endregion SetVertex



	//<<解释顶点数据内容>>
#pragma region ExplainVertex
	//layout中指定的location
	int locationInShader = 0;
	int normalInShader = 1;
	int textureInShader = 2;
	
	//对应传入数据的维度
	int inShaderDataSize = 3;
	int texture_inShaderDataSize = 2;
	//存储前是否归一化
	auto needNormalize = GL_FALSE;
	//注意多个偏移量与顶点数组数据的对应关系
	glVertexAttribPointer(locationInShader, inShaderDataSize, GL_FLOAT, needNormalize, 8 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
	glVertexAttribPointer(normalInShader, inShaderDataSize, GL_FLOAT, needNormalize, 8 * sizeof(GL_FLOAT), BUFFER_OFFSET(3 * sizeof(GL_FLOAT)));
	glVertexAttribPointer(textureInShader, texture_inShaderDataSize, GL_FLOAT, needNormalize, 8 * sizeof(GL_FLOAT), BUFFER_OFFSET(6 * sizeof(GL_FLOAT)));
	//注意要启用相应的位置
	glEnableVertexAttribArray(locationInShader);
	glEnableVertexAttribArray(normalInShader);
	glEnableVertexAttribArray(textureInShader);


	glBindVertexArray(VAO_ID[1]);
	glVertexAttribPointer(locationInShader, inShaderDataSize, GL_FLOAT, needNormalize, 8 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(locationInShader);
#pragma endregion ExplainVertex



#pragma region texture_ID
	glUseProgram(shaderProgram_ID);
	//Texture
	unsigned int texture_ID = loadTexture("../image/container2.png");
	unsigned int texture_ID_2 = loadTexture("../image/container2_specular.png");
	int texture_ID_NUM = 1;
	unsigned int textureIDs[2]{ texture_ID,texture_ID_2 };

	shaderProgram.setInt("material.diffuse", 0);
	shaderProgram.setInt("material.specular", 1);
	shaderProgram.setFloat("material.shininess", 32.0f);
#pragma endregion texture_ID



	//<<各种buffer归位>>
#pragma region Reset
	//在一批数据结束后归位
	glBindBuffer(GL_ARRAY_BUFFER, NULL_ID);
	glBindVertexArray(NULL_ID);
	glBindTexture(GL_TEXTURE_2D, NULL_ID);
#pragma endregion Reset

	

	//<<渲染循环>>

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		#pragma region RenderPart

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgram_ID);
		shaderProgram.setVec3("viewPos", cameraEnv.camera.Pos.x, cameraEnv.camera.Pos.y, cameraEnv.camera.Pos.z);
		#pragma region LightData
		// directional light
		shaderProgram.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		shaderProgram.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		shaderProgram.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		shaderProgram.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		shaderProgram.setVec3("pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		shaderProgram.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		shaderProgram.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		shaderProgram.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		shaderProgram.setFloat("pointLights[0].constant", 1.0f);
		shaderProgram.setFloat("pointLights[0].linear", 0.09f);
		shaderProgram.setFloat("pointLights[0].quadratic", 0.032f);
		// point light 2
		shaderProgram.setVec3("pointLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		shaderProgram.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		shaderProgram.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		shaderProgram.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		shaderProgram.setFloat("pointLights[1].constant", 1.0f);
		shaderProgram.setFloat("pointLights[1].linear", 0.09f);
		shaderProgram.setFloat("pointLights[1].quadratic", 0.032f);
		// point light 3
		shaderProgram.setVec3("pointLights[2].position", pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		shaderProgram.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		shaderProgram.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		shaderProgram.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		shaderProgram.setFloat("pointLights[2].constant", 1.0f);
		shaderProgram.setFloat("pointLights[2].linear", 0.09f);
		shaderProgram.setFloat("pointLights[2].quadratic", 0.032f);
		// point light 4
		shaderProgram.setVec3("pointLights[3].position", pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		shaderProgram.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		shaderProgram.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		shaderProgram.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		shaderProgram.setFloat("pointLights[3].constant", 1.0f);
		shaderProgram.setFloat("pointLights[3].linear", 0.09f);
		shaderProgram.setFloat("pointLights[3].quadratic", 0.032f);
		// spotLight
		shaderProgram.setVec3("spotLight.position", cameraEnv.camera.Pos.x, cameraEnv.camera.Pos.y, cameraEnv.camera.Pos.z);
		shaderProgram.setVec3("spotLight.direction", cameraEnv.camera.Front.x, cameraEnv.camera.Front.y, cameraEnv.camera.Front.z);
		shaderProgram.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		shaderProgram.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		shaderProgram.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		shaderProgram.setFloat("spotLight.constant", 1.0f);
		shaderProgram.setFloat("spotLight.linear", 0.09f);
		shaderProgram.setFloat("spotLight.quadratic", 0.032f);
		shaderProgram.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		shaderProgram.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
		#pragma endregion LightData


		#pragma region Texture

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_ID);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_ID_2);
		#pragma endregion Texture

		#pragma region DrawCube
		glUseProgram(shaderProgram_ID);
		glBindVertexArray(VAO_ID[0]);
		shaderProgram.setMat4("projection", cameraEnv.getPerpective());
		shaderProgram.setMat4("view", cameraEnv.camera.GetViewMatrix());
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shaderProgram.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		#pragma endregion DrawCube


		#pragma region DrawLightCube
		glUseProgram(lightShaderProgram_ID);
		lightShaderProgram.setMat4("projection", cameraEnv.getPerpective());
		lightShaderProgram.setMat4("view", cameraEnv.camera.GetViewMatrix());

		// we now draw as many light bulbs as we have point lights.
		glBindVertexArray(VAO_ID[1]);
		for (unsigned int i = 0; i < 4; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lightShaderProgram.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		#pragma endregion





		#pragma endregion RenderPart

		glfwSwapBuffers(window);
		//<<事件触发器>>
		glfwPollEvents();
	}




	glfwTerminate();
	return 0;
}
