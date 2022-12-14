#include<iostream>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "../ShaderCleanCode/Shader.h"
#include "../CameraCleanCode/Camera.h"
#include "../LightTextureExample/utils.h"
#include <stb_image.h>
#include <vector>
using namespace std;
unsigned int loadCubemap(vector<std::string> faces);

static int width = 800, height = 600;
CameraUtils cameraEnv(width, height);
glm::vec2 translations[1000*1000];

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

    //开启深度测试

	//<<视口设置>>
	glfwSetFramebufferSizeCallback(window, reshape);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);




    int index = 0;
    float offset = 0.001f;
    for (int y = -1000; y < 1000; y += 2)
    {
        for (int x = -1000; x < 1000; x += 2)
        {
            glm::vec2 translation;
            translation.x = (float)x / 1000.0f + offset;
            translation.y = (float)y / 1000.0f + offset;
            translations[index++] = translation;
        }
    };
    float quadVertices[] = {
        // positions     // colors
        -0.0005f,  0.0005f,  1.0f, 0.0f, 0.0f,
         0.0005f, -0.0005f,  0.0f, 1.0f, 0.0f,
        -0.0005f, -0.0005f,  0.0f, 0.0f, 1.0f,

        -0.0005f,  0.0005f,  1.0f, 0.0f, 0.0f,
         0.0005f, -0.0005f,  0.0f, 1.0f, 0.0f,
         0.0005f,  0.0005f,  0.0f, 1.0f, 1.0f
    };
    unsigned int cubeVAO;
    unsigned int cubeVBO;
    //cube
    glGenVertexArrays(1,&cubeVAO);
    glBindVertexArray(cubeVAO);

    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));


    //<<实例化数组>>
    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 1000*1000, &translations[0], GL_STATIC_DRAW);

    //注意没有glBufferData
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1);

    //shader
    Shader vertexShader("../shader/instantial_shader3.vert", GL_VERTEX_SHADER);
    Shader fragShader("../shader/instantial_shader.frag", GL_FRAGMENT_SHADER);


    ShaderProgram program(vertexShader.getId(), fragShader.getId());

    

    
    double lastTime = glfwGetTime();
    int nbFrames = 0;


    //<<渲染循环>>
	while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
            // printf and reset timer
            printf("%f ms/frame\n", 1000.0 / double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
        }

		//<<处理输入>>
		processInput(window);
		//<<渲染函数>>
	#pragma region RenderPart
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glUseProgram(program.getId());
        glBindVertexArray(cubeVAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1000*1000);
        glBindVertexArray(0);


	#pragma endregion RenderPart

        glfwSwapBuffers(window);
		//<<事件触发器>>
		glfwPollEvents();
		
	}



	glfwTerminate();
	return 0;
}

//<<加载天空盒函数>>
// 加载顺序
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    //注意类型是CUBE_MAP
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}