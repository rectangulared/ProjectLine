#include "LineEngine.h"

namespace le
{
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

	void LineEngine::clean()
	{
		glfwTerminate();
	}

	LineEngine::~LineEngine()
	{
		clean();
	}
}