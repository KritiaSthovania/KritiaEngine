#pragma once
#include "pch.h"
#include "Object.h"
#include "MathStructs.h"
#include "Material.h"
#include "Interface/ISerializable.h"
#include "Interface/IInspectable.h"
#include <json/json.hpp>
using json = nlohmann::ordered_json;

namespace KritiaEngine::Editor {
	class AssetDatabase;
}

namespace KritiaEngine::Manager {
	class ResourceManager;
}

namespace KritiaEngine {
	class Mesh : public Object, IJsonSerializable, IFileSerializable, IInspectable{
		friend class MeshFilter;
		friend class KritiaEngine::Rendering::RenderingProvider;
		friend class KritiaEngine::Rendering::OpenGLRendering;
		friend class KritiaEngine::Editor::AssetDatabase;
		friend class KritiaEngine::Manager::ResourceManager;
	public:
		struct Vertex {
			Vector3 Position;
			Vector3 Normal;
			Vector3 Tangent;
			Vector2 TexCoord;
		};
		Mesh() = default;
		Mesh(const std::vector<std::vector<Vertex>>& vertices, const std::vector<std::vector<unsigned int>>& indices, const std::vector<std::shared_ptr<Material>>& materials);
		std::vector<std::vector<Vertex>> submeshVertices;
		std::vector<std::vector<unsigned int>> submeshIndices;
		std::vector<std::shared_ptr<Material>> submeshMaterials;
		int submeshSize = 0;
		friend bool operator< (const std::tuple<Mesh, Material, int>& left, const std::tuple<Mesh, Material, int>& right);
		std::string path;
	private:
		virtual std::string SerializeToJson() override;
		virtual void SerializeToFile() override;
		std::string SubmeshSerialize(int index);
		std::string VertexSerialize(const Vertex& v, int vertexIndex);
		void SubmeshDeserialize(const json& json, int index);
		static Vertex VertexDeserialize(const json& json);
		static std::shared_ptr<Mesh> DeserializeFromJson(const json& json);

		/////// Create Primitives/////
		static Mesh Cube();
		static std::vector<float> GetDefaultCubeVertices();
		std::vector<unsigned int> VAOs, VBOs, EBOs;
		bool isSetup = false;
		bool isPrimitive = false;

		virtual void OnInspector() override;
	};

}


