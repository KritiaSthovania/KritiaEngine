#pragma once
#include "pch.h"
#include "Object.h"
#include "MathStructs.h"
#include "Material.h"
#include "Utilities.h"
#include "Interface/ISerializable.h"
#include "Interface/IInspectable.h"
#include <json/json.hpp>
#include <array>
#include <vulkan/vulkan.h>
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
		friend class KritiaEngine::Rendering::VulkanRendering;
		friend class KritiaEngine::Editor::AssetDatabase;
		friend class KritiaEngine::Manager::ResourceManager;
	public:
		struct Vertex {
			Vector3 Position;
			Vector3 Normal;
			Vector3 Tangent;
			Vector2 TexCoord;

			// Vulkan
			static VkVertexInputBindingDescription getBindingDescription() {
				VkVertexInputBindingDescription bindingDescription = {};
				bindingDescription.binding = 0;
				bindingDescription.stride = sizeof(Vertex);
				bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
				return bindingDescription;
			}

			static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() {
				std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions = {};
				attributeDescriptions[0].binding = 0;
				attributeDescriptions[0].location = 0;
				attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[0].offset = offsetof(Vertex, Position);

				attributeDescriptions[1].binding = 0;
				attributeDescriptions[1].location = 1;
				attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[1].offset = offsetof(Vertex, Normal);

				attributeDescriptions[2].binding = 0;
				attributeDescriptions[2].location = 2;
				attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[2].offset = offsetof(Vertex, Tangent);

				attributeDescriptions[3].binding = 0;
				attributeDescriptions[3].location = 3;
				attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
				attributeDescriptions[3].offset = offsetof(Vertex, TexCoord);
				return attributeDescriptions;
			}
		};
		// The bound in local space
		Bound bound;
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
		virtual void OnInspector() override;
		/////// Create Primitives/////
		static Mesh Cube();
		static std::vector<float> GetDefaultCubeVertices();

		std::vector<unsigned int> VAOs, VBOs, EBOs;
		bool isSetup = false;
		bool isPrimitive = false;
		std::vector<VkBuffer> submeshVertexBuffers;
		std::vector<VkDeviceMemory> submeshVertexBufferMemories;
		std::vector<VkBuffer> submeshIndexBuffers;
		std::vector<VkDeviceMemory> submeshIndexBufferMemories;
	};

}


