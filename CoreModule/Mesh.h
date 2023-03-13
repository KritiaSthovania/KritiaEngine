#pragma once
#include "pch.h"
#include "Object.h"
#include "MathStructs.h"
#include "Material.h"

namespace KritiaEngine {
	class Mesh : public Object{
		friend class MeshFilter;
		friend class KritiaEngine::Rendering::RenderingProvider;
		friend class KritiaEngine::Rendering::OpenGLRendering;
	public:
		struct Vertex {
			Vector3 Position;
			Vector3 Normal;
			Vector3 Tangent;
			Vector2 TexCoord;
		};
		Mesh() = default;
		Mesh(const std::vector<std::vector<Vertex>>& vertices, const std::vector<std::vector<unsigned int>>& indices, const std::vector<std::shared_ptr<Material>>& materials);
		static Mesh Cube();
		std::vector<std::vector<Vertex>> submeshVertices;
		std::vector<std::vector<unsigned int>> submeshIndices;
		std::vector<std::shared_ptr<Material>> submeshMaterials;
		int submeshSize;
		friend bool operator< (const std::tuple<Mesh, Material, int>& left, const std::tuple<Mesh, Material, int>& right);
	private:
		Mesh(const std::string& path);
		virtual std::string Serialize() override;
		static std::shared_ptr<Mesh> DeserializeFromPath(const std::string& path);
		std::vector<unsigned int> VAOs, VBOs, EBOs;
		bool isSetup = false;
		static std::vector<float> GetDefaultCubeVertices();
		std::string path;
	};

}


