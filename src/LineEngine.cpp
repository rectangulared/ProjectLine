#include "LineEngine.h"

namespace le
{
	LineEngine::LineEngine()
	{

	}

	void LineEngine::DestroyGLFWWindow::operator()(GLFWwindow* ptr)
	{
		glfwDestroyWindow(ptr);
	}

	void LineEngine::initVulkan()
	{
		initWindow();
	}

	void LineEngine::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window_ = std::unique_ptr<GLFWwindow, DestroyGLFWWindow>(glfwCreateWindow(windowHeight_, windowWidth_, "LineEngine", nullptr, nullptr));
	}

	void LineEngine::update()
	{
		while (!glfwWindowShouldClose(window_.get()))
		{
			glfwPollEvents();
		}
	}

	void LineEngine::createInstance()
	{
		VkApplicationInfo appInfo{
			VK_STRUCTURE_TYPE_APPLICATION_INFO,
			nullptr,
			"ProtoLine",
			VK_MAKE_API_VERSION(0, 0, 0, 0),
			"LineEngine",
			VK_MAKE_API_VERSION(0, 0, 0, 0),
			VK_MAKE_API_VERSION(0, 0, 0, 0)
		};

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		VkInstanceCreateInfo createInfo{
			VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			nullptr,
			0,
			&appInfo,
			0,
			nullptr,
			glfwExtensionCount,
			glfwExtensions
		};

		VkResult result = vkCreateInstance(&createInfo, nullptr, &vkInstance_);

#ifdef DEBUG
		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to create instance!");
#endif

	}

	void LineEngine::clean()
	{
		vkDestroyInstance(vkInstance_, nullptr);
		glfwTerminate();
	}

	LineEngine::~LineEngine()
	{
		clean();
	}
}