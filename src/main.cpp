#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "LineEngine.h"

int main() {
    le::LineEngine engine;
    engine.windowHeight_ = 800;
    engine.windowWidth_ = 600;
    engine.initVulkan();
    engine.update();
    return 0;
}