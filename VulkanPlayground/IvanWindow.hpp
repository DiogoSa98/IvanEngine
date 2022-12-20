#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace Ivan {

	class IvanWindow {
	public: 
		IvanWindow(int w, int h, std::string name);
		~IvanWindow();

		// don't accidently copy the window address
		IvanWindow(const IvanWindow&) = delete;
		IvanWindow& operator=(const IvanWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }

	private:
		void InitWindow();

		const int width, height;

		std::string windowName;
		GLFWwindow* window;
	};
}