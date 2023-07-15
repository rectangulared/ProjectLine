#pragma once

#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace le
{
    class LineEngine
    {
    public:
        uint32_t windowHeight_;
        uint32_t windowWidth_;

        void initVulkan();

        void update();

        ~LineEngine();
    private:
        struct DestroyGLFWWindow
        {
            void operator()(GLFWwindow*);
        };

        std::unique_ptr<GLFWwindow, DestroyGLFWWindow> window_;

        void initWindow();

        void clean();
    };
}