#include "Ivan.hpp"

namespace Ivan {

	void Ivan::run() {

		while (!ivanWindow.shouldClose()) {
			glfwPollEvents();
			ivanVulkanApp.DrawFrame();
		}

		//  wait for the logical device to finish operations cause drawFrame operations are async
		vkDeviceWaitIdle(ivanVulkanApp.Device());
	}
}