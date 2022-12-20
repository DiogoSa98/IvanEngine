#include "Ivan.hpp"

namespace Ivan {

	void Ivan::run() {

		while (!ivanWindow.shouldClose()) {
			glfwPollEvents();
		}
	}
}