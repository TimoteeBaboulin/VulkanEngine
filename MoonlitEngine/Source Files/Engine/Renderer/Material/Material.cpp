#include "Engine/Renderer/Material/Material.h"
#include <slang/slang-com-ptr.h>
#include "Engine/Renderer/Material/MaterialInstance.h"

#include "Debug/Logger.h"


//ShaderResource GetShaderResource(slang::VariableReflection* _var)
//{
//	ShaderResource resource;
//
//	auto type = _var->getType();
//	auto name = _var->getName();
//
//	resource.Name = name;
//
//	slang::TypeReflection::Kind elementKind;
//	slang::TypeReflection* elementType;
//
//	if (type->isArray())
//	{
//		resource.IsArray = true;
//		resource.ArraySize = type->getTotalArrayElementCount();
//		elementKind = type->getElementType()->getKind();
//		elementType = type->getElementType();
//	}
//	else
//	{
//		resource.IsArray = false;
//		resource.ArraySize = 0;
//		elementKind = type->getKind();
//		elementType = type;
//	}
//
//	switch (elementKind)
//	{
//	case slang::TypeReflection::Kind::Resource:
//		//TODO: Currently the engine expects any resource to be generic
//		//TODO: Handle non-generic resources
//
//		switch (elementType->getResourceShape())
//		{
//		case SlangResourceShape::SLANG_TEXTURE_2D:
//			resource.Type = ResourceType::Texture;
//			break;
//		default:
//			break;
//		}
//		break;
//		case slang::TypeReflection::Kind::TextureBuffer:
//			resource.Type = ResourceType::Texture;
//			break;
//	default:
//		break;
//	}
//
//
//	//Check if it's a resource type
//	if (type->getKind() == slang::TypeReflection::Kind::)
//	{
//		auto resType = type->asResourceType();
//		auto resShape = resType->getResourceShape();
//		auto resAccess = resType->getResourceAccess();
//		auto resElementType = resType->getElementType();
//	}
//}

std::vector<EntryPoint> GetEntryPoints(slang::IModule* _module,
	Slang::ComPtr<slang::ISession> _session,
	SlangSession* _globalSession)
{
	slang::DeclReflection* moduleRefPtr = _module->getModuleReflection();
	auto funcList = moduleRefPtr->getChildrenOfKind<slang::DeclReflection::Kind::Func>();

	std::vector<EntryPoint> entryPoints;

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
			entryPoints.push_back(EntryPoint());
			EntryPoint& currEntryPoint = entryPoints.back();

			slang::Attribute* shaderAttr = funcRefPtr->findAttributeByName(_globalSession, "shader");
			size_t nameLength = 0;
			const char* stageName = shaderAttr->getArgumentValueString(0, &nameLength);
			
			if (strcmp(stageName, "vertex") == 0)
			{
				currEntryPoint.Stage = vk::ShaderStageFlagBits::eVertex;
			}
			else if (strcmp(stageName, "fragment") == 0)
			{
				currEntryPoint.Stage = vk::ShaderStageFlagBits::eFragment;
			}
			else if (strcmp(stageName, "compute") == 0)
			{
				currEntryPoint.Stage = vk::ShaderStageFlagBits::eCompute;
			}
			else
			{
				LOG_ERROR("Unsupported shader stage found in shader module.");
			}
			
			ShaderFunction* func = new ShaderFunction();
			func->Name = funcRefPtr->getName();

			// Add the entry point to the component types for later linking
			componentTypes.push_back(entryPoint);

			//TODO: Find all the parameters to allow material to automatically
		}
	}

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
		memcpy(codePtr, blob.get()->getBufferPointer(), shaderSize);
		entryPoints[index].Function.Code.CodePtr = codePtr;
	}

	return entryPoints;
}

/// <summary>
/// Function used to load a slang module and compile the shaders inside it to SPIR-V
/// </summary>
ShaderData ReadShaderData(const char* filepath)
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

	ShaderData shaderData;
	shaderData.EntryPoints = GetEntryPoints(module, sessionPtr, globalSession);

	return shaderData;
}

Material::Material(std::string _shaderPath)
{
	m_shaderPath = _shaderPath;
	m_shaderData = ReadShaderData(_shaderPath.c_str());
	std::sort(m_shaderData.EntryPoints.begin(), m_shaderData.EntryPoints.end(),
		[](const EntryPoint& a, const EntryPoint& b)
		{
			return a.Stage < b.Stage;
		});
}

Material::~Material()
{
}

MaterialInstance* Material::CreateInstance(RenderTarget& _target)
{
	MaterialInstance* instance = new MaterialInstance(_target, this);
	m_instances.push_back(instance);

	return instance;
}

ShaderData Material::GetShaderData() const
{
	return m_shaderData;
}

void Material::RemoveInstance(MaterialInstance* _instance)
{
	auto it = std::find(m_instances.begin(), m_instances.end(), _instance);
	m_instances.erase(it);
}
