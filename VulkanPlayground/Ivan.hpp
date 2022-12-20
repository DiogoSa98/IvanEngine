#pragma once

#include "IvanWindow.hpp"

namespace Ivan {
	class Ivan {
	
	public:
		static constexpr uint32_t WIDTH = 800;
		static constexpr uint32_t HEIGHT = 600;

		void run();
	
	private:
		IvanWindow ivanWindow{ WIDTH, HEIGHT, "Ivan" };
	};
}