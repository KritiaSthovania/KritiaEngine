#pragma once
#include "Object.h"
#include "Interface/ISerializable.h"
#include "Interface/IInspectable.h"
#include "Utilities.h"

namespace KritiaEngine::Editor {
	class AssetDatabase;
}

namespace KritiaEngine::Manager {
	class ResourceManager;
}

namespace KritiaEngine::Rendering {
	class OpenGLRendering;
	class SoftwareRendering;
}

namespace KritiaEngine {
	class Material;
	class Texture : public Object, IJsonSerializable, IFileSerializable, IInspectable
	{
		friend class Material;
		friend class KritiaEngine::Editor::AssetDatabase;
		friend class KritiaEngine::Manager::ResourceManager;
		friend class KritiaEngine::Rendering::OpenGLRendering;
		friend class KritiaEngine::Rendering::SoftwareRendering;
	public:
		Texture() = default;
		Texture(const std::string& path);
		void LoadImage(const std::string& path, bool alphaChannel);
		std::string path;
		Vector2 size;
		int channels;
	private:
		// The ID in Graphics API, e.g. GLuint
		unsigned int ID = 0;
		bool loaded = false;
		virtual std::string SerializeToJson() override;
		static std::shared_ptr<Texture> DeserializeFromJson(const json& json);

		// 通过 FileSerializable 继承
		virtual void SerializeToFile() override;

		// 通过 IInspectable 继承
		virtual void OnInspector() override;
	};
}


