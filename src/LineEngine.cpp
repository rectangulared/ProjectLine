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
		createInstance();
		setupDebugMessenger();
		createSurface();
		selectPhysicalDevice();
		createLogicalDevice();
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
		if (isValidationLayersEnabled_ && !checkValidationLayerSupport())
		{
			throw std::runtime_error("Validation layers requested, but not available");
		}

		VkApplicationInfo appInfo
		{
			VK_STRUCTURE_TYPE_APPLICATION_INFO,
			nullptr,
			"ProtoLine",
			VK_MAKE_API_VERSION(0, 0, 0, 0),
			"LineEngine",
			VK_MAKE_API_VERSION(0, 0, 0, 0),
			VK_MAKE_API_VERSION(0, 0, 0, 0)
		};

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (isValidationLayersEnabled_)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers_.size());
			createInfo.ppEnabledLayerNames = validationLayers_.data();

			populateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance_);

		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create instance");
		}
	}

	void LineEngine::clean()
	{
		if (isValidationLayersEnabled_) 
		{
			DestroyDebugUtilsMessengerEXT(instance_, debugUtilsMessenger_, nullptr);
		}

		vkDestroyDevice(device_, nullptr);
		vkDestroySurfaceKHR(instance_, surface_, nullptr);
		vkDestroyInstance(instance_, nullptr);
		glfwTerminate();
	}

	LineEngine::~LineEngine()
	{
		clean();
	}

	bool LineEngine::checkValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers_)
		{
			bool isLayerFound = false;
			for (const auto& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					isLayerFound = true;
				}
			}

			if (!isLayerFound)
			{
				return false;
			}
		}

		return true;
	}

	bool LineEngine::checkDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions_.begin(), deviceExtensions_.end());

		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	std::vector<const char*> LineEngine::getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (isValidationLayersEnabled_) 
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	void LineEngine::setupDebugMessenger()
	{
		if (!isValidationLayersEnabled_) return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		populateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(instance_, &createInfo, nullptr, &debugUtilsMessenger_) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to set up debug messenger");
		}
	}

	void LineEngine::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
	}

	VkResult LineEngine::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void LineEngine::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL LineEngine::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	void LineEngine::createSurface()
	{
		if (glfwCreateWindowSurface(instance_, window_.get(), nullptr, &surface_) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create window surface");
		}
	}

	void LineEngine::selectPhysicalDevice()
	{
		uint32_t physicalDeviceCount = 0;
		vkEnumeratePhysicalDevices(instance_, &physicalDeviceCount, nullptr);

		if (physicalDeviceCount == 0)
		{
			throw std::runtime_error("Failed to find GPU with Vulkan Support");
		}

		std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
		vkEnumeratePhysicalDevices(instance_, &physicalDeviceCount, physicalDevices.data());

		std::multimap<int, VkPhysicalDevice> candidates;
		for (const auto& device : physicalDevices)
		{
			int score = ratePhysicalDevice(device);
			candidates.insert(std::make_pair(score, device));
		}

		if (candidates.rbegin()->first > 0 && isDeviceSuitable(candidates.rbegin()->second))
		{
			physicalDevice_ = candidates.rbegin()->second;
		}
		else
		{
			throw std::runtime_error("Failed to find GPU with required features");
		}
	}

	int LineEngine::ratePhysicalDevice(VkPhysicalDevice device)
	{
		VkPhysicalDeviceProperties deviceProperties{};
		vkGetPhysicalDeviceProperties(device, &deviceProperties);

		VkPhysicalDeviceFeatures deviceFeatures{};
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		int score = 0;

		if (deviceProperties.deviceType = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			score += 1;
		}

		score += deviceProperties.limits.maxImageDimension2D;

		if (!deviceFeatures.tessellationShader)
		{
			return 0;
		}

		return score;
	}

	bool LineEngine::isDeviceSuitable(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices = findQueueFamilies(device);

		bool isExtensionsSupported = checkDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (isExtensionsSupported)
		{
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return indices.isComplete() && isExtensionsSupported && swapChainAdequate;
	}

	bool LineEngine::QueueFamilyIndices::isComplete()
	{
		return graphicsFamily_.has_value() && presentFamily_.has_value();
	}

	LineEngine::QueueFamilyIndices LineEngine::findQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily_ = i;
			}

			VkBool32 isPresentSupported = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface_, &isPresentSupported);
			if (isPresentSupported)
			{
				indices.presentFamily_ = i;
			}

			if (indices.isComplete())
			{
				break;
			}

			i++;
		}

		return indices;
	}

	void LineEngine::createLogicalDevice()
	{
		QueueFamilyIndices indices = findQueueFamilies(physicalDevice_);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily_.value(), indices.presentFamily_.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions_.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions_.data();

		if (isValidationLayersEnabled_)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers_.size());
			createInfo.ppEnabledLayerNames = validationLayers_.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(physicalDevice_, &createInfo, nullptr, &device_) != VK_SUCCESS) 
		{
			throw std::runtime_error("Failed to create logical device");
		}

		vkGetDeviceQueue(device_, indices.graphicsFamily_.value(), 0, &graphicsQueue_);
		vkGetDeviceQueue(device_, indices.presentFamily_.value(), 0, &presentQueue_);
	}

	LineEngine::SwapChainSupportDetails LineEngine::querySwapChainSupport(VkPhysicalDevice device)
	{
		SwapChainSupportDetails details;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface_, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &formatCount, nullptr);
		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &presentModeCount, nullptr);
		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &presentModeCount, details.presentModes.data());
		}

		return details;

	}

	VkSurfaceFormatKHR LineEngine::selectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_R8G8B8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}
	}

	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		return VK_PRESENT_MODE_FIFO_KHR;
	}
}