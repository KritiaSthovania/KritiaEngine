#pragma once
#include "Object.h"
#include "Interface/SerializableAndDeserializable.h"
namespace KritiaEngine {
	class Material;
	class Texture : public Object, JsonSerializable, PathDeserializable, JsonDeserializable
	{
		friend class Material;
	public:
		Texture() = default;
		Texture(const std::string& path);
		void LoadImage(const std::string& path);
		std::string path;
	private:
		virtual std::string SerializeToJson() override;
		virtual void DeserializeFromPath(const std::string& path) override;
		virtual void DeserializeFromJson(const json& json) override;
	};
}


