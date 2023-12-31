#include "LineEngine.h"

int main() 
{
    le::LineEngine engine;
    engine.windowHeight_ = 800;
    engine.windowWidth_ = 600;
    try
    {
        engine.initVulkan();
        engine.update();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "Unknown Error" << std::endl;
    }
    return 0;
}