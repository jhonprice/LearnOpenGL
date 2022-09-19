#pragma once
#include "GlfwGuiApp.h"

class ExampleGui : public Gui {
private:
	//imgui part
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
public:
	virtual void guiPass() override;
	const ImVec4& getColor() {
		return clear_color;
	}
};

class ExampleApp :public GlfwGuiApp {
public:
	ExampleApp(std::shared_ptr<Gui> gui) :GlfwGuiApp(gui) {}
	//<<äÖÈ¾º¯Êý>>
	void render() override
	{
		auto clear_color = std::dynamic_pointer_cast<ExampleGui> (gui)->getColor();
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
	}
};

