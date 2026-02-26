#include "RendererContext.h"
#include <iostream>

#ifdef NDEBUG
bool Moonlit::Renderer::RendererContext::ValidationLayersEnabled = false;
#else
bool Moonlit::Renderer::RendererContext::ValidationLayersEnabled = true;
#endif // NDEBUG

#include "../../Debug/Logger.h"

const std::vector<const char*> requiredLayers = {
	"VK_LAYER_KHRONOS_validation"
};

Moonlit::Renderer::RendererContext::~RendererContext()
{
	Cleanup();
}

void Moonlit::Renderer::RendererContext::Init()
{
}

void Moonlit::Renderer::RendererContext::Cleanup()
{
	m_instance.destroy();
}

bool CheckValidationLayerSupport()
{
	uint32_t layerCount = 0;
	vk::Result result;

	result = vk::enumerateInstanceLayerProperties(&layerCount, nullptr);

	if (result != vk::Result::eSuccess)
	{
		throw new std::runtime_error("Couldn't enumerate layer properties.");
	}

	std::vector<vk::LayerProperties> availableLayers(layerCount);
	result = vk::enumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	if (result != vk::Result::eSuccess)
	{
		throw new std::runtime_error("Couldn't enumerate layer properties on second run.");
	}

	for (const char* layerName : requiredLayers)
	{
		bool layerFound = false;
		for (const auto layer : availableLayers)
		{
			if (strcmp(layer.layerName, layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound) return false;
	}

	return true;
}

vk::Instance* Moonlit::Renderer::RendererContext::CreateInstance(vk::ApplicationInfo _appInfo, const char ** requiredExtensions, int extensionCount)
{
	vk::ApplicationInfo appInfo = _appInfo;

	if (ValidationLayersEnabled && !CheckValidationLayerSupport())
	{
		
		Moonlit::Debug::Logger::LogError("Couldn't activate validation layers.");
		ValidationLayersEnabled = false;
	}

	vk::InstanceCreateInfo createInfo{};
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = extensionCount;
	createInfo.ppEnabledExtensionNames = requiredExtensions;

	if (RendererContext::ValidationLayersEnabled)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
		createInfo.ppEnabledLayerNames = requiredLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	m_instance = vk::createInstance(createInfo);
	return &m_instance;
}