#include "ResourceManager.h"

#include "Banks/MeshBank.h"
#include "Banks/TextureBank.h"
#include "Engine/ResourceManagement/Helpers/FileHelper.h"

#include "../../Debug/Logger.h"

#include <filesystem>

#include "Engine/MoonlitEngine.h"
#include "Engine/Modules/ModuleManager.h"
#include "Engine/Tasks/Task.h"

Moonlit::ResourceManagement::ResourceManager::ResourceManager()
{
	InitBank<MeshBank>();
	InitBank<TextureBank>();
}

void Moonlit::ResourceManagement::ResourceManager::LoadResourcesFromDirectory(const std::string& _path)
{
	ResourceManager& instance = Get();
	std::vector<std::string> resourceFiles = FileHelper::ListFilesInDirectory(_path);
	std::vector<std::shared_ptr<Tasks::Task>> tasks;

	for (const std::string& resourceFile : resourceFiles)
	{
		std::shared_ptr<Tasks::Task> task = std::make_shared<Tasks::Task>([&instance, resourceFile]() {
			instance.LoadFileResource(resourceFile);
		});

		tasks.push_back(task);
	}

	Tasks::WorkerManager& wm = *MoonlitEngine::Get().GetMainWorkerManager();
	wm.addTasks(tasks);
}

void Moonlit::ResourceManagement::ResourceManager::LoadFileResource(std::string _filepath)
{
	std::filesystem::path filePath;
	filePath = _filepath;
	std::string extension = filePath.extension().string();

	if (extension == ".gltf" || extension == ".fbx" || extension == ".obj")
	{
		if (!TryLoadResourceInternal<MeshData>(_filepath))
			LOG_ERROR("ResourceManager::LoadFileResource - Failed to load mesh: " + _filepath);
		else
			LOG_INFO("ResourceManager::LoadFileResource - Loaded mesh: " + _filepath);
	}
	else if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" || extension == ".bmp")
	{
		if (!TryLoadResourceInternal<Image>(_filepath))
			LOG_ERROR("ResourceManager::LoadFileResource - Failed to load texture: " + _filepath);
		else
			LOG_INFO("ResourceManager::LoadFileResource - Loaded texture: " + _filepath);
	}
}

Moonlit::ResourceManagement::ResourceManager& Moonlit::ResourceManagement::ResourceManager::Get()
{
	static Moonlit::ResourceManagement::ResourceManager instance;
	return instance;
}