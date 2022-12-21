#pragma once

#include <stdexcept>
#include <vector>
#include <iostream>
#include <unordered_set>

#include "IvanWindow.hpp"


namespace Ivan {
	class IvanVulkanApplication {
	public:
		IvanVulkanApplication(IvanWindow& window);
		~IvanVulkanApplication();

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif
		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
	
	private:
		VkInstance instance;
		IvanWindow& window;
		
		std::vector<VkExtensionProperties> extensions;
		std::unordered_set<std::string> extensionNames;

		VkDebugUtilsMessengerEXT debugMessenger;

		void InitVulkan();
		void GetApplicationExtension();
		std::vector<const char*> GetGLFWRequiredExtensions();
		void CheckGLFWRequiredExtensions(std::vector<const char*> glfwExtensions);

		bool CheckValidationLayerSupport();

		void SetupDebugMessenger();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	};
}