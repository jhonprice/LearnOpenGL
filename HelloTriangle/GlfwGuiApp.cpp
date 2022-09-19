#include "GlfwGuiApp.h"

int OpenglContext::initGLFW(int major,int minor) {
	auto initValue = glfwInit();
	if (!initValue) {
		std::cerr << "GLFW FAILED" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	return 0;
}

int OpenglContext::initWindow() {
	window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	return 0;
}

void Gui::init(GLFWwindow* window) {
	//��ʼ��Imgui������
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	//����gui���
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	//����ƽ̨glfw��glsl�汾
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

int OpenglContext::initgl3w()
{
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
	return 0;
}




//<<�����ӿ�>>
void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (w > 1 ? w : 1), (h > 1 ? h : 1));
}
//<<��������>>
void Gui::processInput(GLFWwindow * window){
	if (ImGui::IsKeyPressed(ImGuiKey_Escape))
		glfwSetWindowShouldClose(window, true);
}



int GlfwGuiApp::run() {

	setup();
	while (!glfwWindowShouldClose(sc.window)) {
		loop();
	}
	terminate();
	return 0;
}

int GlfwGuiApp::init() {
	sc.initGLFW();
	sc.initWindow();
	sc.initgl3w();
	return 0;
}

int GlfwGuiApp::setup() {
	glfwSetFramebufferSizeCallback(sc.window, reshape);
	return 0;
}

int GlfwGuiApp::loop() {


	//<<��������>>
	gui->processInput(sc.window);
	//<<������Ⱦ>>
	render();
	gui->guiPass();
	//<<�¼�������>>
	glfwPollEvents();
	//<<����������>>
	glfwSwapBuffers(sc.window);

	return 0;
}

int GlfwGuiApp::terminate() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(sc.window);
	glfwTerminate();
	return 0;
}
int GlfwGuiApp::initGui() {
	gui->init(sc.window);
	return 0;
}