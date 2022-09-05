#include<iostream>
#include "../ShaderCleanCode/Shader.h"
#include "../CameraCleanCode/Camera.h"

//一些常量
static int width = 800, height = 600;
const int NULL_ID = 0;
const int vertexNum = 36;
const int vertexAttriNum = 6;

//相机的位置与方向
#pragma region CameraPosInfo
CameraUtils cameraEnv(width, height);
#pragma endregion CameraPosInfo

glm::vec3 LightPositions(1.2f, 1.0f, 2.0f);




//<<渲染函数>>
static void display(GLFWwindow* window, unsigned int* shaderProgram_ID, unsigned int* VAO_ID)
{

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//<<光线颜色>>
	glm::vec3 lightColor;
	float time = glfwGetTime();
	lightColor.x = sin(time * 2.0f);
	lightColor.y = sin(time * 0.7f);
	lightColor.z = sin(time * 1.3f);

	glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);


	//绘制不同的cube
#pragma region drawCube
	glUseProgram(shaderProgram_ID[0]);
	glBindVertexArray(VAO_ID[0]);


	//材质
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


	//光源点：漫反射,镜面反射
	loctmp = glGetUniformLocation(shaderProgram_ID[0], "light.position");
	glUniform3f(loctmp, LightPositions.x, LightPositions.y, LightPositions.z);

	//观察点：镜面反射
	loctmp = glGetUniformLocation(shaderProgram_ID[0], "eyePos");
	glUniform3f(loctmp, cameraEnv.camera.Pos.x, cameraEnv.camera.Pos.y, cameraEnv.camera.Pos.z);




	glm::mat4 model = glm::mat4(1.0f);
	int modelLoc = glGetUniformLocation(shaderProgram_ID[0], "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 view = cameraEnv.camera.GetViewMatrix();
	int viewLoc = glGetUniformLocation(shaderProgram_ID[0], "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));



	//透射
	auto fovRadians = glm::radians(cameraEnv.camera.Zoom);
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


	

	model = glm::mat4(1.0f);
	model = glm::translate(model, LightPositions);
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
	//0是物体1是灯
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
	//0是物体1是灯
	unsigned int shaderProgram_ID[2]{ shaderProgram.getId(),lightShaderProgram.getId() };



#pragma endregion IDManaged

	//<<设置顶点数据内容解释的缓存>>
#pragma region SetVAO
	glGenVertexArrays(VAO_ID_NUM, VAO_ID);
	glBindVertexArray(VAO_ID[0]);
#pragma endregion SetVAO

	//<<设置顶点坐标>>
	//+=<<添加法线信息>>
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
	//对应传入数据的维度
	int inShaderDataSize = 3;
	//存储前是否归一化
	auto needNormalize = GL_FALSE;
	//注意多个偏移量与顶点数组数据的对应关系
	glVertexAttribPointer(locationInShader, inShaderDataSize, GL_FLOAT, needNormalize, 6 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
	glVertexAttribPointer(normalInShader, inShaderDataSize, GL_FLOAT, needNormalize, 6 * sizeof(GL_FLOAT), BUFFER_OFFSET(3 * sizeof(GL_FLOAT)));
	//注意要启用相应的位置
	glEnableVertexAttribArray(locationInShader);
	glEnableVertexAttribArray(normalInShader);

	glBindVertexArray(VAO_ID[1]);
	glVertexAttribPointer(locationInShader, inShaderDataSize, GL_FLOAT, needNormalize, 6 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
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
