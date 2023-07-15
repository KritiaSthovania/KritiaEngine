#pragma once
#include "Component.h"
#include "../CoreModule/Mesh.h"
#include <vulkan/vulkan.h>
#include <unordered_map>
namespace KritiaEngine::Rendering {
	class RenderingProvider;
	class VulkanRendering;
}

namespace KritiaEngine {
	class MeshFilter : public Component {
		friend class MeshRenderer;
		friend class KritiaEngine::Rendering::VulkanRendering;
		friend class KritiaEngine::Rendering::RenderingProvider;
	public:
		MeshFilter(GameObject* gameObject);
		std::shared_ptr<Mesh> mesh;
	private:
		// Should be called by MeshRenderer
		void SetupMesh();
		// Í¨¹ý Component ¼Ì³Ð
		virtual void OnInspector() override;
		virtual std::string SerializeToJson() override;
		virtual void DeserializeFromJson(const json& json) override;
		virtual std::string GetInspectorLabel() override;
		std::string inspectorLabel = "MeshFilter";
		bool isMeshChanged = false;
		std::unordered_map<Material*, VkBuffer> uniformBufferVertex = std::unordered_map<Material*, VkBuffer>();
		std::unordered_map<Material*, VkDeviceMemory> uniformBufferMemoryVertex = std::unordered_map<Material*, VkDeviceMemory>();
		std::unordered_map<Material*, VkDescriptorSet> descriptorSetVertex = std::unordered_map<Material*, VkDescriptorSet>();
	};
}


