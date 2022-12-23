#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <string>
#include <stdexcept>
#include <algorithm>

namespace Ivan {

	class IvanWindow {
	public: 
		IvanWindow(int width, int height, std::string name);
		~IvanWindow();

		// don't accidently copy the window address
		IvanWindow(const IvanWindow&) = delete;
		IvanWindow& operator=(const IvanWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }

		void CreateWindowSurface(VkInstance vkInstance, VkSurfaceKHR* surface);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		bool FramebufferResized() { return framebufferResized; }
		void FramebufferResized(bool resized) { framebufferResized = resized; }

		void HandleMinimization();
	private:
		void InitWindow();

		const int width, height;

		std::string windowName;
		GLFWwindow* window;
		bool framebufferResized;
		static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);
	};
}