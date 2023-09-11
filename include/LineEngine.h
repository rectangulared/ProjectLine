#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <stdexcept>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <shaderc/shaderc.hpp>

namespace le
{
    class LineEngine
    {
    public:
        const std::vector<const char*> validationLayers_
        {
            "VK_LAYER_KHRONOS_validation"
        };

        const std::vector<const char*> deviceExtensions_
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        #ifdef NDEBUG
        const bool isValidationLayersEnabled_ = false;
        #else 
        const bool isValidationLayersEnabled_ = true;
        #endif
        uint32_t windowHeight_;
        uint32_t windowWidth_;

        VkDebugUtilsMessengerEXT debugUtilsMessenger_;

        LineEngine();

        LineEngine(const LineEngine&) = delete;
        LineEngine& operator=(const LineEngine&) = delete;

        LineEngine(LineEngine&&) = delete;
        LineEngine& operator=(LineEngine&&) = delete;

        void initVulkan();

        void update();

        ~LineEngine();
    private:
        struct DestroyGLFWWindow
        {
            void operator()(GLFWwindow*);
        };

        struct QueueFamilyIndices
        {
            std::optional<uint32_t> graphicsFamily_;
            std::optional<uint32_t> presentFamily_;

            bool isComplete();
        };

        struct SwapChainSupportDetails
        {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        std::unique_ptr<GLFWwindow, DestroyGLFWWindow> window_;

        VkDevice device_;
        VkInstance instance_;
        VkQueue graphicsQueue_;
        VkQueue presentQueue_;
        VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
        VkSurfaceKHR surface_;
        VkSwapchainKHR swapChain_;
        VkFormat swapChainImageFormat_;
        VkExtent2D swapChainExtent_;
        VkPipelineLayout pipelineLayout_;

        std::vector<VkImage> swapChainImages_;
        std::vector<VkImageView> swapChainImageViews_;

        void clean();
        void createGraphicsPipeline();
        void createImageViews();
        void createInstance();
        void createLogicalDevice();
        void createSurface();
        void createSwapChain();
        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
        void initWindow();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        void selectPhysicalDevice();
        void setupDebugMessenger();

        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        bool checkValidationLayerSupport();
        bool isDeviceSuitable(VkPhysicalDevice device);

        int ratePhysicalDevice(VkPhysicalDevice device);

        std::vector<const char*> getRequiredExtensions();

        std::vector<char> readFile(const std::string_view& fileName);

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

        VkSurfaceFormatKHR selectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

        VkExtent2D selectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        VkShaderModule createShaderModule(const std::vector<char>& sourceCode);
    };
}