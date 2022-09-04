#include<iostream>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "../CameraCleanCode/Camera.h"
#include "../LightTextureExample/utils.h"
#include "../ShaderCleanCode/Shader.h"

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



//<<视口和鼠标设置>>
#pragma region ViewportAndMouseSet
	glfwSetFramebufferSizeCallback(window, reshape);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
#pragma endregion ViewportAndMouseSet

#pragma region ShaderProgram
	Shader vertexShader("../shader/framebuffer_shader.vert", GL_VERTEX_SHADER);
	Shader fragShader("../shader/framebuffer_shader.frag", GL_FRAGMENT_SHADER);

	ShaderProgram shaderProgram(vertexShader.getId(), fragShader.getId());

	Shader screenVertexShader("../shader/framebuffer_screen_shader.vert", GL_VERTEX_SHADER);
	Shader screenFragShader("../shader/framebuffer_screen_shader.frag", GL_FRAGMENT_SHADER);

	ShaderProgram screenShaderProgram(screenVertexShader.getId(), screenFragShader.getId());

	
#pragma endregion ShaderProgram

#pragma region SetVertex
	//VAO
	unsigned int VAO_ID;
	unsigned int PLANE_VAO_ID;
	unsigned int QUAD_VAO_ID;
	int VAO_ID_NUM = 1;
	
	//VBO
	unsigned int VBO_ID;
	unsigned int PLANE_VBO_ID;
	unsigned int QUAD_VBO_ID;
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


	//draw quad
	glGenVertexArrays(VAO_ID_NUM, &QUAD_VAO_ID);
	glGenBuffers(VBO_ID_NUM, &QUAD_VBO_ID);
	glBindVertexArray(QUAD_VAO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, QUAD_VBO_ID);
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);

#pragma endregion SetVertex

#pragma region SetTexture
	unsigned int cubeTexture = loadTexture("../image/container.jpg");
	unsigned int planeTexture = loadTexture("../image/metal.png");
#pragma endregion SetTexture

//<<传递uniform变量(不变）>>
#pragma region StableUnform
	glUseProgram(shaderProgram.getId());
	shaderProgram.setInt("texture1", 0);

	glUseProgram(screenShaderProgram.getId());
	screenShaderProgram.setInt("screenTexture", 0);
#pragma endregion

//<<帧缓冲>>
#pragma region framebuffer
	//生成帧缓冲
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);


	//<<纹理attachment>>
	// 建立一个颜色纹理
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	//与一般纹理的区别这里的长宽不来自于stb函数，而是窗口的长宽度
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//附加到帧缓冲区
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	//<<渲染缓冲区attachment>>
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	//生成，绑定，使用
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
	//附加到帧缓冲区
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

	//检查帧缓冲区是否完整
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	//回到默认帧缓冲区（影响屏幕
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

#pragma endregion framebuffer configuration



while (!glfwWindowShouldClose(window)) {
	cameraEnv.deltaUpdate(glfwGetTime());
	//<<处理输入>>
	processInput(window);

	//<<渲染函数>>
#pragma region RenderFunc
	//<<向自定义的帧缓冲区写入数据>>
	// bind to framebuffer and draw scene as we normally would to color texture 
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//清除深度缓冲区
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram.getId());
	//transform
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = cameraEnv.camera.GetViewMatrix();
	glm::mat4 projection = cameraEnv.getPerpective();
	shaderProgram.setMat4("view", view);
	shaderProgram.setMat4("projection", projection);

	//cube
	glBindVertexArray(VAO_ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);
	//cube1
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	shaderProgram.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//cube2
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	shaderProgram.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// floor
	glBindVertexArray(PLANE_VAO_ID);
	glBindTexture(GL_TEXTURE_2D, planeTexture);
	shaderProgram.setMat4("model", glm::mat4(1.0f));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);


	// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	// clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);

	//<<向默认帧缓冲区写入数据>>
	glUseProgram(screenShaderProgram.getId());
	glBindVertexArray(QUAD_VAO_ID);
	//这里的绑定体现了渲染到纹理的技术：我们实际只在场景中画了一个矩形，但在离屏渲染时向颜色缓冲区渲染了另一个场景
	//所以之前渲染的场景在这里作为纹理被绑定，故称渲染到纹理
	//如果去掉这句话，则绘制出的是最后一次被绑定的纹理（这里是floor的纹理
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);

#pragma endregion RenderFunc


	glfwSwapBuffers(window);
	//<<事件触发器>>
	glfwPollEvents();
	
}



	glfwTerminate();
	return 0;
}
