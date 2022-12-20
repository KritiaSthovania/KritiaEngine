#pragma once
#include "Object.h"
#include "../Rendering/Shader.h"
#include "Utilities.h"
#include "../CoreModule/MathStructs.h"
#include "Texture.h"

namespace KritiaEngine {
	class Material : public Object
	{
		friend class MeshRenderer;
	public:
		enum RenderMode {
			Opaque,
			Transparent
		};
		Material();
		Material(const char* name);
		Material(const char* name, std::shared_ptr<Shader> shader);
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
	private:
		/// <summary>
        /// Compile Shader, Load Texture
        /// </summary>
		void Initialize();
		void ApplyShaderOnRender(const Matrix4x4& projection, const Matrix4x4& view, const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos);
		void SetMainLightProperties();
		void SetPointLightProperties(const Vector3 &pos);
		void SetSpotLightProperties(const Vector3 &pos);
		int w_diffuse, h_diffuse, r_diffuse, w_specular, h_specular, r_specular, diffuseSamplerIndex = 0, specularSamplerIndex = 1;
		unsigned int m_VertexShader, m_FragmentShader, m_ShaderProgram, mainTextureID, specularMapID;
	};
}


