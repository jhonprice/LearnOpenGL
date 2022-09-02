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

//glm::vec3 LightPositions(1.2f, 1.0f, 2.0f);
glm::vec3 LightPositions(-2.2f, 1.5f, -5.0f);
//方向光
glm::vec3 LightDirection(-0.2f, -1.0f, -0.3f);
#pragma endregion CameraPosInfo


//<<渲染函数>>
void display(GLFWwindow* window, unsigned int* shaderProgram_ID, unsigned int* VAO_ID, unsigned int* TEXTURE_ID)
{
	//std::cout  << cameraEnv.camera.Pos.x << "\t" << cameraEnv.camera.Pos.y << "\t" << cameraEnv.camera.Pos.z << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


//绘制不同的cube
#pragma region drawCube
	
	int nowProgam = 0;
	//方向光
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		nowProgam = 2;
		glUseProgram(shaderProgram_ID[nowProgam]);
		glUniform3f(glGetUniformLocation(shaderProgram_ID[nowProgam], "light.direction"),
			LightPositions.x, LightPositions.y, LightPositions.z);
	}
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		nowProgam = 3;
		glUseProgram(shaderProgram_ID[nowProgam]);
		glUniform3f(glGetUniformLocation(shaderProgram_ID[nowProgam], "light.position"),
			LightPositions.x, LightPositions.y, LightPositions.z);

		glUniform1f(glGetUniformLocation(shaderProgram_ID[nowProgam], "light.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(shaderProgram_ID[nowProgam], "light.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(shaderProgram_ID[nowProgam], "light.quadratic"), 0.032f);
	}
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
			nowProgam = 4;
		}
		else {
			nowProgam = 5;
		}
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
	}
	else {
		//不衰减点光源
		glUseProgram(shaderProgram_ID[nowProgam]);
		glUniform3f(glGetUniformLocation(shaderProgram_ID[0], "light.position"), 
			LightPositions.x, LightPositions.y, LightPositions.z);
	}
	glBindVertexArray(VAO_ID[0]);



//<<设置材质，光，观察点（通用）>>
#pragma region MaterialAndLightAndEyes
	//<<光线颜色>>
	glm::vec3 lightColor(1.f, 1.f, 1.f);
	glm::vec3 diffuseColor = lightColor * glm::vec3(0.8f);
	glm::vec3 ambientColor = lightColor * glm::vec3(0.1f);

	int tmpLoc = glGetUniformLocation(shaderProgram_ID[nowProgam], "material.diffuse");
	glUniform1i(tmpLoc, 0);
	tmpLoc = glGetUniformLocation(shaderProgram_ID[nowProgam], "material.specular");
	glUniform1i(tmpLoc, 1);
	tmpLoc = glGetUniformLocation(shaderProgram_ID[nowProgam], "material.shininess");
	glUniform1f(tmpLoc, 32.f);
	
	
	//光源
	tmpLoc = glGetUniformLocation(shaderProgram_ID[nowProgam], "light.ambient");
	glUniform3f(tmpLoc, ambientColor.x, ambientColor.y, ambientColor.z);
	tmpLoc = glGetUniformLocation(shaderProgram_ID[nowProgam], "light.diffuse");
	glUniform3f(tmpLoc, diffuseColor.x, diffuseColor.y, diffuseColor.z);
	tmpLoc = glGetUniformLocation(shaderProgram_ID[nowProgam], "light.specular");
	glUniform3f(tmpLoc, lightColor.x, lightColor.y, lightColor.z);


	//观察点：镜面反射
	tmpLoc = glGetUniformLocation(shaderProgram_ID[nowProgam], "eyePos");
	glUniform3f(tmpLoc, cameraEnv.camera.Pos.x, cameraEnv.camera.Pos.y, cameraEnv.camera.Pos.z);
#pragma endregion MaterialAndLightAndEyes

//<<设置光照贴图>>
#pragma region SetUniform

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEXTURE_ID[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TEXTURE_ID[1]);
#pragma endregion SetUniform

	//绘制方块
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
		int modelLoc = glGetUniformLocation(shaderProgram_ID[nowProgam], "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


		glm::mat4 view = cameraEnv.camera.GetViewMatrix();
		int viewLoc = glGetUniformLocation(shaderProgram_ID[nowProgam], "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));



		//透射
		auto fovRadians = glm::radians(cameraEnv.camera.Zoom);
		auto aspect = static_cast<float>(width / height);
		auto nearPlane = 0.1f, farPlane = 100.0f;
		glm::mat4 projection = glm::perspective(fovRadians, aspect, nearPlane, farPlane);
		int projLoc = glGetUniformLocation(shaderProgram_ID[nowProgam], "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, vertexNum);
	}

#pragma endregion drawCube

#pragma region drawLight
	glUseProgram(shaderProgram_ID[1]);
	glBindVertexArray(VAO_ID[1]);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, LightPositions);
	model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
	int modelLoc = glGetUniformLocation(shaderProgram_ID[1], "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


	int viewLoc = glGetUniformLocation(shaderProgram_ID[1], "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cameraEnv.camera.GetViewMatrix()));


	//透射
	int projLoc = glGetUniformLocation(shaderProgram_ID[1], "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(cameraEnv.getPerpective()));

	if (nowProgam != 2&& nowProgam != 4) {
		glDrawArrays(GL_TRIANGLES, 0, vertexNum);
	}
	
#pragma endregion drawLight

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
	Shader fragShader("../shader/light_type_shader.frag", GL_FRAGMENT_SHADER);
	Shader lightFragShader("../shader/lightmode_light_shader.frag", GL_FRAGMENT_SHADER);

	//lighttype
	Shader directFragShader("../shader/light_type_direction_shader.frag", GL_FRAGMENT_SHADER);
	unsigned int direcFragShader_ID = directFragShader.getId();
	Shader pointFragShader("../shader/light_type_point_shader.frag", GL_FRAGMENT_SHADER);
	unsigned int pointFragShader_ID = pointFragShader.getId();
	Shader spotFragShader("../shader/light_type_spot_shader.frag", GL_FRAGMENT_SHADER);
	unsigned int spotFragShader_ID = spotFragShader.getId();
	Shader flashFragShader("../shader/light_type_flash_shader.frag", GL_FRAGMENT_SHADER);
	unsigned int flashFragShader_ID = flashFragShader.getId();



	unsigned int vertexShader_ID = vertexShader.getId();
	unsigned int lightVertexShader_ID = lightVertexShader.getId();

	unsigned int fragShader_ID = fragShader.getId();
	unsigned int lightFragShader_ID = lightFragShader.getId();
	


	//Program
	ShaderProgram shaderProgram(vertexShader_ID, fragShader_ID);
	ShaderProgram lightShaderProgram(lightVertexShader_ID, lightFragShader_ID);
	ShaderProgram direcLightshaderProgram(vertexShader_ID, direcFragShader_ID);
	ShaderProgram pointLightshaderProgram(vertexShader_ID, pointFragShader_ID);
	ShaderProgram spotLightshaderProgram(vertexShader_ID, spotFragShader_ID);
	ShaderProgram flashLightshaderProgram(vertexShader_ID, flashFragShader_ID);

	unsigned int shaderProgram_ID[6]{ shaderProgram.getId(), 
		lightShaderProgram.getId(),
		direcLightshaderProgram.getId(),pointLightshaderProgram.getId(),spotLightshaderProgram.getId(),flashLightshaderProgram.getId() };


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


	//<<启用程序，清理着色器代码>>
	//trap::+=<<在传递uniform的时候，必须要激活Program>>
#pragma region UseProgram
	glUseProgram(shaderProgram_ID[0]);
	glDeleteShader(vertexShader_ID);
	glDeleteShader(fragShader_ID);
#pragma endregion UseProgram

#pragma region texture_ID
	//Texture
	unsigned int texture_ID = loadTexture("../image/container2.png");
	unsigned int texture_ID_2 = loadTexture("../image/container2_specular.png");
	int texture_ID_NUM = 1;
	unsigned int textureIDs[2]{ texture_ID,texture_ID_2 };
#pragma endregion texture_ID



	







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
		
		display(window, shaderProgram_ID, VAO_ID, textureIDs);
		//<<事件触发器>>
		glfwPollEvents();

	}
#pragma endregion RenderPart



	glfwTerminate();
	return 0;
}
