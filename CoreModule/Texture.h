#pragma once
#include "Object.h"
namespace KritiaEngine {
	class Texture : public Object
	{
	public:
		Texture() = default;
		Texture(const std::string& path);
		void LoadImage(const std::string& path);
		unsigned int id;
		std::string type;
		std::string path;
	private:
		virtual std::string Serialize() override;
		std::shared_ptr<Texture> DeserializeFromPath(const std::string& path);
	};
}


