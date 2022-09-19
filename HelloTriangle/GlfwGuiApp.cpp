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
	//初始化Imgui上下文
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	//设置gui风格
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	//设置平台glfw和glsl版本
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
void ExampleGui::guiPass() {

	// Poll and handle events (inputs, window resize, etc.)
	// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
	// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
	// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
	// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.


	//新建imgui帧
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// 1. 显示demo窗口 (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. 用Begin和end之间的操作建立一个窗口 Show a simple window that we create ourselves. 
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
	// 4. Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}



//<<设置视口>>
void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (w > 1 ? w : 1), (h > 1 ? h : 1));
}
//<<处理输入>>
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


	//<<处理输入>>
	gui->processInput(sc.window);
	//<<背景渲染>>
	render();
	gui->guiPass();
	//<<事件触发器>>
	glfwPollEvents();
	//<<交换缓冲区>>
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