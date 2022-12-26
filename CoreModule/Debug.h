#pragma once
#include <string>
#include <iostream>
namespace KritiaEngine {
	class Debug
	{
	public:
		template <typename T> static void Log(const T &message) {
			std::cout << message << std::endl;
		}
	};

}


