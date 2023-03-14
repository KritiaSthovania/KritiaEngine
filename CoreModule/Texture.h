#pragma once
#include "Object.h"
#include "Interface/SerializableAndDeserializable.h"
namespace KritiaEngine {
	class Texture : public Object, JsonSerializable, PathDeserializable
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
		void DeserializeFromPath(const std::string& path);
	};
}


