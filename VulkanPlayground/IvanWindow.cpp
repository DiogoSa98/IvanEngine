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

	void IvanWindow::CreateWindowSurface(VkInstance vkInstance, VkSurfaceKHR* surface) {
		if (glfwCreateWindowSurface(vkInstance, window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}

	VkExtent2D IvanWindow::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}
}