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
	virtual void processInput(GLFWwindow*);
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



