#pragma once
#include "Object.h"
#include "../Rendering/Shader.h"
#include "Utilities.h"
#include "../CoreModule/MathStructs.h"
#include "Texture.h"
#include "Interface/SerializableAndDeserializable.h"

namespace KritiaEngine::Rendering {
	class RenderingProvider;
	class OpenGLRendering;
}

namespace KritiaEngine {
	class Mesh;
	class Material : public Object, JsonDeserializable, PathDeserializable, JsonSerializable
	{
		friend class KritiaEngine::Rendering::RenderingProvider;
		friend class KritiaEngine::Rendering::OpenGLRendering;
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
		Color albedo;
		/// <summary>
		/// Specular shininess
		/// </summary>
		float shininess = 32;
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
	private:
		/// <summary>
        /// Compile Shader, Load Texture
        /// </summary>
		void Initialize();
		virtual std::string SerializeToJson() override;
		virtual void DeserializeFromJson(const json& json) override;
		virtual void DeserializeFromPath(const std::string& path) override;
		int diffuseSamplerIndex = 0, specularSamplerIndex = 1, normalSamplerIndex = 2, parallaxSamplerIndex = 3, shadowSamplerIndex = 4;
		unsigned int mainTextureID, specularMapID, normalMapID, parallaxMapID;
		unsigned int matricesVPID;
		bool initialized = false;
	};
}


