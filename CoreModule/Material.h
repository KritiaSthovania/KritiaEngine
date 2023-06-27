#pragma once
#include "Object.h"
#include "../Rendering/Shader.h"
#include "Utilities.h"
#include "../CoreModule/MathStructs.h"
#include "Texture.h"
#include "Interface/ISerializable.h"
#include "Interface/IInspectable.h"

namespace KritiaEngine::Rendering {
	class RenderingProvider;
	class OpenGLRendering;
}

namespace KritiaEngine::Editor::GUI {
	class ProjectFileExplorer;
}

namespace KritiaEngine::Manager {
	class ResourceManager;
}

namespace KritiaEngine {
	class Mesh;
	class Material : public Object, IJsonSerializable, IFileSerializable, IInspectable
	{
		friend class KritiaEngine::Rendering::RenderingProvider;
		friend class KritiaEngine::Rendering::OpenGLRendering;
		friend class KritiaEngine::Editor::GUI::ProjectFileExplorer;
		friend class KritiaEngine::Manager::ResourceManager;
		friend class MeshRenderer;
		friend class Mesh;
	public:
		enum class RenderMode {
			Opaque,
			Transparent
		};
		Material();
		Material(const char* name);
		Material(const char* name, const std::shared_ptr<Shader>& shader);
		Material(const std::shared_ptr<Shader>& shader);
		Color albedo;
		/// <summary>
		/// Specular shininess
		/// </summary>
		float shininess = 32;
		// PBR
		float metallic = 0;
		float roughness = 0;
		float ao = 0;
		bool PBR = false;
		bool cachedPBR = false;
		RenderMode renderMode = RenderMode::Opaque;
		/// <summary>
		/// The main texture, either diffuse map or transparent texture.
		/// </summary>
		std::shared_ptr<Texture> mainTexture;
		std::shared_ptr<Texture> specularMap;
		std::shared_ptr<Texture> normalMap;
		std::shared_ptr<Texture> parallaxMap;
		std::shared_ptr<Shader> shader;
		bool GPUInstancingEnabled = false;
		std::string path;
	private:
		/// <summary>
        /// Compile Shader, Load Texture
        /// </summary>
		void Initialize();
		void SetPropertiesOnRender();
		virtual std::string SerializeToJson() override;
		static std::shared_ptr<Material> DeserializeFromJson(const json& json);
		int diffuseSamplerIndex = 0, specularSamplerIndex = 1, normalSamplerIndex = 2, parallaxSamplerIndex = 3, shadowSamplerIndex = 4;
		unsigned int matricesVPID;
		bool initialized = false;
		// 通过 FileSerializable 继承
		virtual void SerializeToFile() override;

		// 通过 Inspectable 继承
		virtual void OnInspector() override;
	};
}


