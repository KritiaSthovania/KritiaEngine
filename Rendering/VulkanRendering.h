#pragma once
#define GLFW_INCLUDE_VULKAN
#include "../CoreModule/Mesh.h"
#include <pch.h>
#include <vulkan/vulkan.h>
#include <fstream>
namespace KritiaEngine::Rendering {
	struct QueueFamilyIndices {
		int graphicsFamily = -1;
		int presentFamily = -1;
		bool IsComplete() {
			return graphicsFamily >= 0 && presentFamily >=0;
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VulkanRendering {
		friend class RenderingProvider;
	public:
		static void OnWindowResized(GLFWwindow* window, int width, int height);
	private:

		/// <summary>
		/// Initializing functions
		/// </summary>
		static void Initialize(GLFWwindow* window);
		static void CreateInstance();
		static void CreateSurface();
		static void PickPhysicalDevice();
		static void CreateLogicalDevice();
		static void CreateSwapChain();
		static bool IsDeviceSuitable(VkPhysicalDevice device);
		static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		static bool CheckValidationLayerSupport();
		static bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		static std::vector<const char*> GetRequiredExtensions();
		static void CreateImageViews();
		static void CreateFramebuffers();
		static void CreateCommandPool();
		static void CreateCommandBuffer();
		static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		static void BeginCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		static void BeginRenderPass(uint32_t imageIndex);
		static void CreateRenderPass();
		static void CreateSemaphores();
		static void RecreateSwapChain();
		static void SetupDebugCallback();
		static VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
		static void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);

		/// <summary>
		/// Render
		/// </summary>
		static void CreateGraphicsPipeline();
		static VkShaderModule CreateShaderModule(const std::vector<char>& code);
		static void SetupRenderingFrame();
		static void EndRenderingFrame();
		static void SetupMesh(const std::shared_ptr<Mesh>& mesh);
		static void RenderSubmesh(const std::shared_ptr<MeshFilter>& meshFilter, const std::shared_ptr<Material>& material, int submeshIndex, const Matrix4x4& model, const Vector3& viewPos, const Vector3& pos);
		// Create buffers and allocate memories
		static void SetupSubmesh(const std::shared_ptr<Mesh>& mesh, int submeshIndex);
		static void CreateVertexBuffer(const std::vector<Mesh::Vertex> vertices, VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory);
		static void CreateIndexBuffer(const std::vector<unsigned int> indices, VkBuffer& indexBuffer, VkDeviceMemory& indexBufferMemory);
		static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		static std::vector<char> readFile(const std::string& filename) {
			std::ifstream file(filename, std::ios::ate | std::ios::binary);

			if (!file.is_open()) {
				throw std::runtime_error("failed to open file!");
			}
			size_t fileSize = (size_t)file.tellg();
			std::vector<char> buffer(fileSize);
			file.seekg(0);
			file.read(buffer.data(), fileSize);
			file.close();

			return buffer;
		}

		// Clear
		static void Cleanup();
		static void CleanupSwapChain();

		static int width;
		static int height;
		static GLFWwindow* window;
		static VkInstance instance;
		static VkDebugReportCallbackEXT callback;
		static VkPhysicalDevice physicalDevice;
		static VkDevice device;
		static VkSurfaceKHR surface;
		static VkSwapchainKHR swapChain;
		static VkQueue graphicsQueue;
		static VkQueue presentQueue;
		static std::vector<const char*> validationLayers;
		static std::vector<const char*> deviceExtensions;
		static std::vector<VkImage> swapChainImages;
		static std::vector<VkImageView> swapChainImageViews;
		static std::vector<VkFramebuffer> swapChainFramebuffers;
		static VkFormat swapChainImageFormat;
		static VkExtent2D swapChainExtent;
		static VkPhysicalDeviceFeatures deviceFeatures;
		static VkRenderPass renderPass;
		static VkCommandPool commandPool;
		static VkCommandBuffer commandBuffer;

		// Render
		static VkPipelineLayout pipelineLayout;
		static VkPipeline graphicsPipeline;
		static VkSemaphore imageAvailableSemaphore;
		static VkSemaphore renderFinishedSemaphore;
		static uint32_t currentSwapChainImageIndex;
		static std::vector<VkBuffer*> vertexBuffers;
		static std::vector<VkDeviceMemory*> vertexBufferMemories;

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags,
			VkDebugReportObjectTypeEXT objType,
			uint64_t obj,
			size_t location,
			int32_t code,
			const char* layerPrefix,
			const char* msg,
			void* userData);
	};


}


