#include "IvanWindow.hpp"

namespace Ivan {
	IvanWindow::IvanWindow(int width, int height, std::string name) : width(width), height(height), windowName(name) {
		InitWindow();
	}
	
	IvanWindow::~IvanWindow() {
		glfwDestroyWindow(window);
		glfwTerminate(); 
	}

	void IvanWindow::InitWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // don't create OpenGL context
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	}
}