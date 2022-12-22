#pragma once

#include <stdexcept>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <map>
#include <optional>
#include <set>

#include "IvanWindow.hpp"


namespace Ivan {
	
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool IsComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

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

		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};
	
	private:
		VkInstance instance;
		IvanWindow& window;
		
		void InitVulkan();

		std::vector<VkExtensionProperties> extensions;
		std::unordered_set<std::string> extensionNames;
		void GetApplicationExtension();
		std::vector<const char*> GetGLFWRequiredExtensions();
		void CheckGLFWRequiredExtensions(std::vector<const char*> glfwExtensions);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

		bool CheckValidationLayerSupport();

		VkDebugUtilsMessengerEXT debugMessenger;
		void SetupDebugMessenger();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		void PickPhysicalDevice();
		int RateDeviceSuitability(VkPhysicalDevice device);

		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

		VkDevice device;
		void CreateLogicalDevice();

		VkQueue graphicsQueue;
		VkQueue presentQueue;

		VkSurfaceKHR surface;
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		void CreateSurface();
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		void CreateSwapChain();
	};
}