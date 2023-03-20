#pragma once
#include "pch.h"
#include "Object.h"
#include "MathStructs.h"
#include "Material.h"
#include "Interface/SerializableAndDeserializable.h"
#include <json/json.hpp>
using json = nlohmann::ordered_json;

namespace KritiaEngine::Editor {
	class AssetDatabase;
}

namespace KritiaEngine::Manager {
	class ResourceManager;
}

namespace KritiaEngine {
	class Mesh : public Object, PathDeserializable, JsonSerializable, JsonDeserializable, FileSerializable{
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
	private:
		virtual std::string SerializeToJson() override;
		std::string SubmeshSerialize(int index);
		std::string VertexSerialize(const Vertex& v, int vertexIndex);
		virtual void DeserializeFromPath(const std::string& path) override;
		void SubmeshDeserialize(const json& json, int index);
		static Vertex VertexDeserialize(const json& json);
		/////// Create Primitives/////
		static Mesh Cube();

		std::vector<unsigned int> VAOs, VBOs, EBOs;
		bool isSetup = false;
		static std::vector<float> GetDefaultCubeVertices();
		std::string path;

		// 通过 JsonDeserializable 继承
		virtual void DeserializeFromJson(const json& json) override;

		// 通过 FileSerializable 继承
		virtual void SerializeToFile() override;
	};

}


