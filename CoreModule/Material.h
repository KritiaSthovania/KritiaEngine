#pragma once
#include "Object.h"
#include "../Rendering/Shader.h"
#include "Utilities.h"
#include "../CoreModule/MathStructs.h"
#include "Texture.h"

namespace KritiaEngine::Rendering {
	class RenderingProvider;
	class OpenGLRendering;
}

namespace KritiaEngine {
	class Material : public Object
	{
		friend class KritiaEngine::Rendering::RenderingProvider;
		friend class KritiaEngine::Rendering::OpenGLRendering;
		friend class MeshRenderer;
	public:
		enum RenderMode {
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
		RenderMode renderMode = Opaque;
		/// <summary>
		/// The main texture, either diffuse map or transparent texture.
		/// </summary>
		std::shared_ptr<Texture> mainTexture;
		std::shared_ptr<Texture> specularMap;
		std::shared_ptr<Texture> normalMap;
		std::shared_ptr<Shader> shader;
		bool GPUInstancingEnabled = false;
	private:
		/// <summary>
        /// Compile Shader, Load Texture
        /// </summary>
		void Initialize();
		int diffuseSamplerIndex = 0, specularSamplerIndex = 1, normalSamplerIndex = 2, shadowSamplerIndex = 3;
		unsigned int mainTextureID, specularMapID, normalMapID;
		unsigned int matricesVPID;
		bool initialized = false;
	};
}


