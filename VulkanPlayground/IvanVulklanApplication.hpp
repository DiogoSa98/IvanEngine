#pragma once

#include <stdexcept>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <optional>
#include <set>
#include <fstream>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <chrono>

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

	// Read all of the bytes from the specified file and return them in a byte array
	static std::vector<char> ReadFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary); // start at eof to allocate buffer with size of file

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	struct Vertex {
		glm::vec3 pos;
		glm::vec3 normal;
		//glm::vec3 color;
		//glm::vec2 texCoord;

		static VkVertexInputBindingDescription GetBindingDescription() {
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, normal);
			//attributeDescriptions[1].offset = offsetof(Vertex, color);

			//attributeDescriptions[2].binding = 0;
			//attributeDescriptions[2].location = 2;
			//attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			//attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

			return attributeDescriptions;
		}

		bool operator==(const Vertex& other) const {
			//return pos == other.pos && color == other.color && texCoord == other.texCoord;
			return pos == other.pos && normal == other.normal;
		}
	};

	//const std::vector<Vertex> vertices = {
	//	{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	//	{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
	//	{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
	//	{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

	//	{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	//	{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
	//	{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
	//	{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
	//};

	//const std::vector<uint16_t> indices = {
	//	0, 1, 2, 2, 3, 0,
	//	4, 5, 6, 6, 7, 4
	//};

	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	const std::string MODEL_PATH = "models/JLRIN_shader_ball/knob.obj";
	const std::string TEXTURE_PATH = "textures/viking_room.png";

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

		VkDevice Device() { return device; }
	
		uint32_t currentFrame = 0;
		void DrawFrame();
	
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
		void CleanupSwapChain();
		void RecreateSwapChain();

		std::vector<VkImageView> swapChainImageViews;
		void CreateImageViews();


		VkPipeline graphicsPipeline;
		VkPipelineLayout pipelineLayout;
		void CreateGraphicsPipeline();

		VkDescriptorSetLayout descriptorSetLayout;
		void CreateDescriptorSetLayout();


		VkShaderModule CreateShaderModule(const std::vector<char>& code);

		VkRenderPass renderPass;
		void CreateRenderPass();

		std::vector<VkFramebuffer> swapChainFramebuffers;
		void CreateFramebuffers();

		VkCommandPool commandPool;
		void CreateCommandPool();

		std::vector<VkCommandBuffer> commandBuffers;
		void CreateCommandBuffers();
		void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);


		const int MAX_FRAMES_IN_FLIGHT = 2;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		void CreateSyncObjects();

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
		void LoadModel();
		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void CreateVertexBuffer();
		void CreateIndexBuffer();
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		
		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		std::vector<void*> uniformBuffersMapped;
		void CreateUniformBuffers();
		void UpdateUniformBuffer(uint32_t currentImage);

		VkDescriptorPool descriptorPool;
		void CreateDescriptorPool();
		std::vector<VkDescriptorSet> descriptorSets;
		void CreateDescriptorSets();

		uint32_t mipLevels;
		VkImage textureImage;
		VkDeviceMemory textureImageMemory;
		void CreateTextureImage();

		void CreateVkImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
		void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	
		VkImageView textureImageView;
		VkSampler textureSampler;
		void CreateTextureImageView();
		VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
		void CreateTextureSampler();


		VkImage depthImage;
		VkDeviceMemory depthImageMemory;
		VkImageView depthImageView;
		void CreateDepthResources();
		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat FindDepthFormat();
		bool HasStencilComponent(VkFormat format);


		VkSampleCountFlagBits msaaSamples;
		VkSampleCountFlagBits GetMaxUsableSampleCount();

		// for MSAA buffer must store more than one sample per pixel
		VkImage colorImage;
		VkDeviceMemory colorImageMemory;
		VkImageView colorImageView;
		void CreateColorResources();
		void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

	};
}

namespace std {
	template<> struct hash<Ivan::Vertex> {
		size_t operator()(Ivan::Vertex const& vertex) const {
			//return ((hash<glm::vec3>()(vertex.pos) ^
			//	(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
			//	(hash<glm::vec2>()(vertex.texCoord) << 1);
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.normal) << 1)) >> 1);
		}
	};
}