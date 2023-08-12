#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace le
{
    class LineEngine
    {
    public:
        const std::vector<const char*> validationLayers_
        {
            "VK_LAYER_KHRONOS_validation"
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

        std::unique_ptr<GLFWwindow, DestroyGLFWWindow> window_;

        VkInstance vkInstance_;

        void clean();
        void createInstance();
        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
        void initWindow();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        void setupDebugMessenger();

        bool checkValidationLayerSupport();

        std::vector<const char*> getRequiredExtensions();

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

    };
}