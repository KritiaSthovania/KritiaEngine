#pragma once
#include "Object.h"
#include "Interface/SerializableAndDeserializable.h"

namespace KritiaEngine::Editor {
	class AssetDatabase;
}

namespace KritiaEngine {
	class Material;
	class Texture : public Object, JsonSerializable, PathDeserializable, JsonDeserializable, FileSerializable
	{
		friend class Material;
		friend class KritiaEngine::Editor::AssetDatabase;
	public:
		Texture() = default;
		Texture(const std::string& path);
		void LoadImage(const std::string& path);
		std::string path;
	private:
		virtual std::string SerializeToJson() override;
		virtual void DeserializeFromPath(const std::string& path) override;
		virtual void DeserializeFromJson(const json& json) override;

		// Í¨¹ý FileSerializable ¼Ì³Ð
		virtual void SerializeToFile() override;
	};
}


