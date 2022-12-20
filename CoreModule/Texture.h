#pragma once
#include "Object.h"
namespace KritiaEngine {
	class Texture : public Object
	{
	public:
		Texture() = default;
		Texture(std::string path);
		void LoadImage(std::string path);
		unsigned int id;
		std::string type;
		std::string path;
	private:

	};
}


