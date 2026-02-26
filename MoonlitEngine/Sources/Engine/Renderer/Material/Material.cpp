#include "Material.h"
#include <slang/slang-com-ptr.h>
#include "MaterialInstance.h"

#include "../../../Debug/Logger.h"
#include "Engine/Renderer/Helpers/VulkanHelperFunctions.h"

bool TryGetShaderStageFromString(const char* _stageName, vk::ShaderStageFlagBits& _outStage)
{
	if (strcmp(_stageName, "vertex") == 0)
	{
		_outStage = vk::ShaderStageFlagBits::eVertex;
		return true;
	}
	else if (strcmp(_stageName, "fragment") == 0)
	{
		_outStage = vk::ShaderStageFlagBits::eFragment;
		return true;
	}
	else if (strcmp(_stageName, "compute") == 0)
	{
		_outStage = vk::ShaderStageFlagBits::eCompute;
		return true;
	}
	else
	{
		LOG_ERROR("Unsupported shader stage found in shader module.");
		return false; // Default return to avoid compiler warning
	}
}

void ReadAttribute(Moonlit::EntryPoint& _entryPoint, slang::Attribute* _attr)
{
	std::string attrName = _attr->getName();

	if (attrName == "shader")
	{
		size_t nameLength = 0;
		const char* stageName = _attr->getArgumentValueString(0, &nameLength);
		if (!TryGetShaderStageFromString(stageName, _entryPoint.Stage))
		{
			LOG_ERROR("Failed to get shader stage from entry point attribute.");
			return;
		}
	}
	else if (attrName == "RenderPass")
	{
		size_t subpassNameLength = 0;
		const char* subpassName = _attr->getArgumentValueString(0, &subpassNameLength);
		_entryPoint.SubpassNames.push_back(std::string(subpassName, subpassNameLength));
	}
}

void ReadEntryPointData(Moonlit::EntryPoint& _entryPoint, SlangSession* _globalSession, 
	slang::FunctionReflection* _funcPtr)
{
	int attributeCount = _funcPtr->getUserAttributeCount();

	for (int i = 0; i < attributeCount; i++)
	{
		slang::Attribute* attr = _funcPtr->getUserAttributeByIndex(i);

		ReadAttribute(_entryPoint, attr);
	}

	slang::Attribute* stageAttr = _funcPtr->findUserAttributeByName(_globalSession, "shader");
	if (!stageAttr)
	{
		LOG_ERROR("Entry point is missing required 'shader' attribute.");
		return;
	}
	ReadAttribute(_entryPoint, stageAttr);

	_entryPoint.Function.Name = _funcPtr->getName();
}

std::vector<Moonlit::EntryPoint> GetEntryPoints(slang::IModule* _module,
	Slang::ComPtr<slang::ISession> _session,
	SlangSession* _globalSession)
{
	slang::DeclReflection* moduleRefPtr = _module->getModuleReflection();
	auto funcList = moduleRefPtr->getChildrenOfKind<slang::DeclReflection::Kind::Func>();

	std::vector<Moonlit::EntryPoint> entryPoints;

	// Add all entry points alongside the module to generate the code later on
	std::vector<slang::IComponentType*> componentTypes;
	componentTypes.push_back(_module);

	// Get every entry point
	for (auto it = funcList.begin(); it != funcList.end(); ++it)
	{
		//Check if they are entry points
		auto funcRefPtr = (*it)->asFunction();
		Slang::ComPtr<slang::IEntryPoint> entryPoint;
		const char* entryPointName = funcRefPtr->getName();
		bool result = SLANG_SUCCEEDED(_module->findEntryPointByName(funcRefPtr->getName(), entryPoint.writeRef()));
		if (result)
		{
			// Add the data to the vector
			entryPoints.push_back(Moonlit::EntryPoint());
			Moonlit::EntryPoint& currEntryPoint = entryPoints.back();

			ReadEntryPointData(currEntryPoint, _globalSession, funcRefPtr);

			// Add the entry point to the component types for later linking
			componentTypes.push_back(entryPoint);

			// TODO: Find all the parameters to allow material to automatically
			// TODO: allocate descriptors
		}
	}

	// TODO: Cut this function in two, probably cut it here
	// Create the linked program to start generating code for the entry points
	Slang::ComPtr<slang::IComponentType> program;

	_session.get()->createCompositeComponentType(componentTypes.data(),
		componentTypes.size(), program.writeRef());

	Slang::ComPtr<slang::IComponentType> linkedProgram;
	Slang::ComPtr<slang::IBlob> diagnostics;
	program.get()->link(linkedProgram.writeRef(), diagnostics.writeRef());

	// Generate code for every entry point
	//ShaderCode* shaderCodes = new ShaderCode[entryPoints.size()];

	for (size_t index = 0; index < entryPoints.size(); index++)
	{
		Slang::ComPtr<slang::IBlob> blob;
		linkedProgram.get()->getEntryPointCode(index, 0, blob.writeRef(), diagnostics.writeRef());
		if (diagnostics)
		{
			fprintf(stderr, "%s\n", (const char*)diagnostics->getBufferPointer());
		}

		size_t shaderSize = blob.get()->getBufferSize();
		entryPoints[index].Function.Code.Size = shaderSize;
		void* codePtr = malloc(shaderSize);
		if (!codePtr)
		{
			LOG_ERROR("Failed to allocate memory for shader code.");
		}
		else
		{
			memcpy(codePtr, blob.get()->getBufferPointer(), shaderSize);
			entryPoints[index].Function.Code.CodePtr = codePtr;
		}
	}

	return entryPoints;
}

/// <summary>
/// Function used to load a slang module and compile the shaders inside it to SPIR-V
/// </summary>
Moonlit::ShaderData ReadShaderData(const char* filepath)
{
	Slang::ComPtr<slang::IGlobalSession> globalSession;
	SlangGlobalSessionDesc globalSessionDesc;
	slang::createGlobalSession(&globalSessionDesc, globalSession.writeRef());

	slang::SessionDesc sessionDesc;
	sessionDesc.structureSize = sizeof(slang::SessionDesc);

	//Targets
	slang::TargetDesc targets[1];
	targets[0].format = SLANG_SPIRV;
	targets[0].profile = globalSession.get()->findProfile("glsl_450");
	sessionDesc.targets = targets;
	sessionDesc.targetCount = 1;

	sessionDesc.searchPaths = nullptr;
	sessionDesc.searchPathCount = 0;

	sessionDesc.preprocessorMacroCount = 0;
	sessionDesc.preprocessorMacros = nullptr;

	Slang::ComPtr<slang::ISession> sessionPtr;
	globalSession.get()->createSession(sessionDesc, sessionPtr.writeRef());

	Slang::ComPtr<slang::IBlob> diagnostics;

	slang::IModule* module = sessionPtr.get()->loadModule(filepath, diagnostics.writeRef());

	if (diagnostics)
	{
		fprintf(stderr, "%s\n", (const char*)diagnostics->getBufferPointer());
	}

	Moonlit::ShaderData shaderData;
	shaderData.EntryPoints = GetEntryPoints(module, sessionPtr, globalSession);

	return shaderData;
}

Moonlit::Material::Material(std::string _shaderPath)
{
	m_shaderPath = _shaderPath;
	m_shaderData = ReadShaderData(_shaderPath.c_str());

	// Store all the included subpasses
	// Make it easier to invalidate draw buffers for specific subpasses
	// Or when creating a material instance
	for (auto& entryPoint : m_shaderData.EntryPoints)
	{
		for (const auto& subpassName : entryPoint.SubpassNames)
		{
			auto it = std::find(m_includedSubpasses.begin(), m_includedSubpasses.end(), subpassName);
			if (it == m_includedSubpasses.end())
			{
				m_includedSubpasses.push_back(subpassName);
			}
		}
	}

	std::sort(m_shaderData.EntryPoints.begin(), m_shaderData.EntryPoints.end(),
		[](const EntryPoint& a, const EntryPoint& b)
		{
			return a.Stage < b.Stage;
		});
}

Moonlit::Material::~Material()
{
}

std::shared_ptr<Moonlit::MaterialInstance> Moonlit::Material::GetOrCreateInstance(Moonlit::Renderer::DeviceData& _deviceData)
{
	auto it = m_instanceMap.find(_deviceData.Device);
	if (it != m_instanceMap.end())
	{
		return it->second;
	}

	MaterialInstance* instance = CreateInstance(_deviceData);
	if (instance)
	{
		std::shared_ptr<MaterialInstance> sharedInstance(instance);
		m_instanceMap[_deviceData.Device] = sharedInstance;
		return sharedInstance;
	}

	throw std::runtime_error("Failed to create MaterialInstance.");
}

Moonlit::ShaderData Moonlit::Material::GetShaderData() const
{
	return m_shaderData;
}

void Moonlit::Material::RemoveInstance(MaterialInstance* _instance)
{
	// This shouldn't be needed with the use of shared_ptr
	// TODO: Add a check to remove unused instances from the map

	/*auto it = std::find(m_instances.begin(), m_instances.end(), _instance);
	m_instances.erase(it);*/
}

Moonlit::MaterialInstance* Moonlit::Material::CreateInstance(Moonlit::Renderer::DeviceData _deviceData)
{
	return new Moonlit::MaterialInstance(_deviceData.Device, this, _deviceData.SurfaceFormat.format);
}
