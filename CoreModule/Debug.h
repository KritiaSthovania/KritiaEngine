#pragma once
#include <string>
#include <iostream>
#define Debug_Log(m) KritiaEngine::Debug::Log(m);

namespace KritiaEngine {
	class Debug
	{
	public:
		template <typename T> static void Log(const T &message) {
			std::cout << message << std::endl;
		}
	};

}


