#pragma once
#define GLFW_INCLUDE_VULKAN
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
		static void CreateCommandBuffers();
		static void CreateRenderPass();
		static void CreateSemaphores();
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

		static void Cleanup();

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
		static std::vector<VkCommandBuffer> commandBuffers;

		// Render
		static VkPipelineLayout pipelineLayout;
		static VkPipeline graphicsPipeline;
		static VkSemaphore imageAvailableSemaphore;
		static VkSemaphore renderFinishedSemaphore;

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


