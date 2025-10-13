#include "Renderer/Material.h"

#include "Renderer/VulkanHelperFunctions.h"
#include "Renderer/Renderer.h"
#include "slang/slang-com-ptr.h"

#include "ResourceManagement/FileHelper.h"
#include "Renderer/MaterialInstance.h"


ShaderResource GetShaderResource(slang::VariableReflection* _var)
{
	ShaderResource resource;

	auto type = _var->getType();
	auto name = _var->getName();

	resource.Name = name;

	slang::TypeReflection::Kind elementKind;
	slang::TypeReflection* elementType;

	if (type->isArray())
	{
		resource.IsArray = true;
		resource.ArraySize = type->getTotalArrayElementCount();
		elementKind = type->getElementType()->getKind();
		elementType = type->getElementType();
	}
	else
	{
		resource.IsArray = false;
		resource.ArraySize = 0;
		elementKind = type->getKind();
		elementType = type;
	}

	switch (elementKind)
	{
	case slang::TypeReflection::Kind::Resource:
		//TODO: Currently the engine expects any resource to be generic
		//TODO: Handle non-generic resources

		switch (elementType->getResourceShape())
		{
		case SlangResourceShape::SLANG_TEXTURE_2D:
			resource.Type = ResourceType::Texture;
			break;
		default:
			break;
		}
		break;
		case slang::TypeReflection::Kind::TextureBuffer:
			resource.Type = ResourceType::Texture;
			break;
	default:
		break;
	}


	//Check if it's a resource type
	if (type->getKind() == slang::TypeReflection::Kind::)
	{
		auto resType = type->asResourceType();
		auto resShape = resType->getResourceShape();
		auto resAccess = resType->getResourceAccess();
		auto resElementType = resType->getElementType();
	}
}

/// <summary>
/// Function used to load a slang module and compile the shaders inside it to SPIR-V
/// </summary>
ShaderCode* GetModules(const char* filepath)
{
	Slang::ComPtr<slang::IGlobalSession> globalSession;
	SlangGlobalSessionDesc globalSessionDesc;
	slang::createGlobalSession(&globalSessionDesc, globalSession.writeRef());

	slang::SessionDesc sessionDesc;
	sessionDesc.structureSize = sizeof(slang::SessionDesc);
	//sessionDesc.defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_ROW_MAJOR;

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

	Slang::ComPtr<slang::IEntryPoint> vertEntryPoint;
	Slang::ComPtr<slang::IEntryPoint> fragEntryPoint;

	//Reflect to find all functions in the module
	slang::DeclReflection* moduleRefPtr = module->getModuleReflection();
	auto funcList = moduleRefPtr->getChildrenOfKind<slang::DeclReflection::Kind::Func>();

	std::vector<Slang::ComPtr<slang::IEntryPoint>> entryPoints;
	ShaderData shaderData;

	for (auto it = funcList.begin(); it != funcList.end(); ++it)
	{
		//Check if they are entry points
		auto funcRefPtr = (*it)->asFunction();
		Slang::ComPtr<slang::IEntryPoint> entryPoint;
		if (module->findEntryPointByName(funcRefPtr->getName(), entryPoint.writeRef()))
		{
			entryPoints.push_back(entryPoint);
			shaderData.EntryPoints.push_back(ShaderFunction());
			ShaderFunction& currEntryPoint = shaderData.EntryPoints.back();

			//TODO: Find all the parameters to allow material to automatically
			//TODO: Create the sets and their layouts
			unsigned int paramCount = funcRefPtr->getParameterCount();

			for (auto index = 0; index < paramCount; index++)
			{
				auto param = funcRefPtr->getParameterByIndex(index);
				ShaderResource resource = GetShaderResource(param);
			}
		}
	}

	module->findEntryPointByName("vertexMain", vertEntryPoint.writeRef());
	slang::FunctionReflection* vertFuncRef = vertEntryPoint.get()->getFunctionReflection();

	module->findEntryPointByName("fragmentMain", fragEntryPoint.writeRef());

	slang::IComponentType* componentTypes[] = { module, vertEntryPoint, fragEntryPoint };
	Slang::ComPtr<slang::IComponentType> program;
	sessionPtr.get()->createCompositeComponentType(componentTypes, 3, program.writeRef());


	Slang::ComPtr<slang::IComponentType> linkedProgram;
	program.get()->link(linkedProgram.writeRef(), diagnostics.writeRef());

	Slang::ComPtr<slang::IBlob> blobs[2];

	linkedProgram.get()->getEntryPointCode(0, 0, blobs[0].writeRef(), diagnostics.writeRef());
	if (diagnostics)
	{
		fprintf(stderr, "%s\n", (const char*)diagnostics->getBufferPointer());
	}
	linkedProgram.get()->getEntryPointCode(1, 0, blobs[1].writeRef(), diagnostics.writeRef());
	if (diagnostics)
	{
		fprintf(stderr, "%s\n", (const char*)diagnostics->getBufferPointer());
	}

	ShaderCode* shaderCodes = new ShaderCode[2];

	shaderCodes[0].size = blobs[0].get()->getBufferSize();
	shaderCodes[0].code = malloc(shaderCodes[0].size);
	memcpy(shaderCodes[0].code, blobs[0].get()->getBufferPointer(), shaderCodes[0].size);

	shaderCodes[1].size = blobs[1].get()->getBufferSize();
	shaderCodes[1].code = malloc(shaderCodes[1].size);
	memcpy(shaderCodes[1].code, blobs[1].get()->getBufferPointer(), shaderCodes[1].size);

	return shaderCodes;
}

Material::Material(std::string _shaderPath)
{
	m_shaderPath = _shaderPath;
	m_shaderCode = GetModules(_shaderPath.c_str());
}

Material::~Material()
{
	delete[] m_shaderCode;
}

MaterialInstance* Material::CreateInstance(RenderTarget& _target)
{
	MaterialInstance* instance = new MaterialInstance(_target, m_shaderCode, this);
	m_instances.push_back(instance);

	return instance;
}
