#include "BaseShaderApp.h"
#include "../HelloTriangle/vertex_model.h"
#include "../HelloTriangle/LoadShader.h"

void BaseShaderGui::guiPass() {
	// Poll and handle events (inputs, window resize, etc.)
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.


//新建imgui帧
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


	ImGui::ShowDemoWindow();

	// 2. 用Begin和end之间的操作建立一个窗口 Show a simple window that we create ourselves. 
	{

		ImGui::Begin("TriangleDemo");

		ImGui::Text("Back");
		ImGui::ColorEdit3("clear color", (float*)&clear_color);
		ImGui::Checkbox("line mode", (bool*)&line_mode);




		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	//Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void BaseShaderGui::processInput(GLFWwindow* window) {
	if (ImGui::IsKeyPressed(ImGuiKey_Escape))
		glfwSetWindowShouldClose(window, true);
}


int BaseShaderApp::setup() {
	glfwSetFramebufferSizeCallback(sc.window, reshape);

	//加载shader
	string vertexCode = "../shader/hellovertex.vert";
	string fragCode = "../shader/hellofrag.frag";

	loadShader(vertexCode, vertexId, GL_VERTEX_SHADER);
	loadShader(fragCode, fragId, GL_FRAGMENT_SHADER);
	linkProgram(vertexId, fragId, programId);


	//加载数据
	triangle_plane_without_index(vao, vbo, drawLen);

	return 0;
}
void BaseShaderApp::linemode() {
	//开启线框模式
	auto linemode = std::dynamic_pointer_cast<BaseShaderGui> (gui)->isLineMode();
	if (linemode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void BaseShaderApp::renderPass() {
	glUseProgram(programId);
	glBindVertexArray(vao);
	
	glDrawArrays(GL_TRIANGLES, 0, drawLen);
}
