#pragma once
#include <iostream>
#include <memory>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include<IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw.h>
#include <IMGUI/imgui_impl_opengl3.h>



void reshape(GLFWwindow* window, int w, int h);

//Context
class OpenglContext {
public:
	GLFWwindow* window=nullptr;
	int width = 1280, height = 1024;
public:
	int initGLFW(int major=3,int minor=3);
	int initWindow();
	int initgl3w();
};

//GUI
class Gui {
public:
	void init(GLFWwindow*);
	virtual void guiPass() = 0;
	virtual void processInput(GLFWwindow*) = 0;
};
class ExampleGui : public Gui{
private:
	//imgui part
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
public:
	virtual void guiPass() override;
	virtual void processInput(GLFWwindow*) override;
	const ImVec4& getColor(){
		return clear_color;
	}
};


//APP
class GlfwGuiApp {
protected:
	OpenglContext sc;
	std::shared_ptr<Gui> gui=nullptr;
protected:
	int init();
	int initGui();
public:
	GlfwGuiApp(std::shared_ptr<Gui> gui):gui(gui) {
		init();
		initGui();
	};
	int run();
	int loop();
protected:
	virtual int setup();
	virtual void render()=0;
	virtual int terminate();
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