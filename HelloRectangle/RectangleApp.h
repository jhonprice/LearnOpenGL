#pragma once
#include "../HelloTriangle/GlfwGuiApp.h"

class RectangleGui : public Gui {
private:
	//imgui part
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	bool line_mode = false;
public:
	virtual void guiPass() override;
	virtual void processInput(GLFWwindow*) override;
	const ImVec4& getColor() {
		return clear_color;
	}
	const bool isLineMode() {
		return line_mode;
	}
};

class RectangleApp :public GlfwGuiApp {
private:
	unsigned int vertexId = 0, fragId = 0, programId = 0;
	unsigned int vao = 0, vbo = 0, ebo=0;
	unsigned int drawLen = 0;
public:
	RectangleApp(std::shared_ptr<Gui> gui) :GlfwGuiApp(gui) {}
	//≥ı ºªØ
	virtual int setup() override;
	//<<‰÷»æ∫Ø ˝>>
	void colorPass() {
		auto clear_color = std::dynamic_pointer_cast<RectangleGui> (gui)->getColor();
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void renderPass();
	void render() override
	{
		colorPass();
		renderPass();
	}
};


