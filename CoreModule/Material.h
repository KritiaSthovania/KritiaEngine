#pragma once
#include "Object.h"
#include "../Rendering/Shader.h"
#include "Utilities.h"
#include "../CoreModule/MathStructs.h"
#include "Texture.h"

namespace KritiaEngine {
	class Material : public Object
	{
		friend class RenderingProvider;
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
		std::shared_ptr<Shader> shader;
		bool GPUInstancingEnabled = false;
	private:
		/// <summary>
        /// Compile Shader, Load Texture
        /// </summary>
		void Initialize();
		void ApplyShaderOnRender(const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos);
		int diffuseSamplerIndex = 0, specularSamplerIndex = 1;
		unsigned int mainTextureID, specularMapID;
		unsigned int matricesVPID;
		bool initialized = false;
	};
}


