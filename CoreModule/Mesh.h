#pragma once
#include "pch.h"
#include "Object.h"
#include "MathStructs.h"
#include "Material.h"

namespace KritiaEngine {
	class Mesh : public Object{
		friend class MeshFilter;
		friend class RenderingProvider;
	public:
		struct Vertex {
			Vector3 Position;
			Vector3 Normal;
			Vector2 TexCoord;
		};
		Mesh() = default;
		Mesh(const std::vector<std::vector<Vertex>>& vertices, const std::vector<std::vector<unsigned int>>& indices, const std::vector<std::shared_ptr<Material>>& materials);
		~Mesh() {
		}
		static Mesh Cube();
		std::vector<std::vector<Vertex>> submeshVertices;
		std::vector<std::vector<unsigned int>> submeshIndices;
		std::vector<std::shared_ptr<Material>> submeshMaterials;
		int submeshSize;
	private:
		std::vector<unsigned int> VAOs, VBOs, EBOs;
		bool isSetup = false;
		static std::vector<float> GetDefaultCubeVertices();
	};

}


