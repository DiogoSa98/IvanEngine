#pragma once

#include <array>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <vulkan/vulkan_core.h> 

namespace Ivan {


	struct Vertex {
		glm::vec3 pos;
		glm::vec3 normal;
		//glm::vec3 color;
		glm::vec2 texCoord;

		static VkVertexInputBindingDescription GetVertexBindingDescription();
		
		static std::array<VkVertexInputAttributeDescription, 3> GetVertexAttributeDescriptions();

		bool operator==(const Vertex& other) const {
			//return pos == other.pos && color == other.color && texCoord == other.texCoord;
			return pos == other.pos && normal == other.normal && texCoord == other.texCoord;
		}
	};




	struct Mesh {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;

		void LoadFromObjFile(const char* filename);
	};

}

namespace std {
	template<> struct hash<Ivan::Vertex> {
		size_t operator()(Ivan::Vertex const& vertex) const {
			//return ((hash<glm::vec3>()(vertex.pos) ^
			//	(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
			//	(hash<glm::vec2>()(vertex.texCoord) << 1);
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1)) >> 1) ^
				(hash<glm::vec3>()(vertex.normal) << 1);
		}
	};
}