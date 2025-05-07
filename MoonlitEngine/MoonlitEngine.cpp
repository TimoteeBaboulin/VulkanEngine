#include "MoonlitEngine.h"

#include "GameObject.h"
#include "ObjectTransform.h"
#include "ObjectTest.h"

#include "MoonlitVulkan/ResourceBank.h"
#include "MoonlitVulkan/Mesh.h"

#include <QtCore/qtimer.h>
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/mesh.h"


Mesh GetMesh(std::string _filepath)
{
	MeshData data = ImportMesh(_filepath);
	Mesh mesh;
	mesh.Load()
	return Mesh(data);
}

MeshData ImportMesh(std::string _path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(_path, aiPostProcessSteps::aiProcess_OptimizeMeshes | aiPostProcessSteps::aiProcess_Triangulate | aiPostProcessSteps::aiProcess_FlipUVs);
	if (scene == nullptr)
	{
		std::string errorMessage = importer.GetErrorString();
		throw new std::runtime_error(errorMessage);
	}
	MeshData mesh = GetMesh(scene->mMeshes[0]);
	return mesh;
}

MeshData GetMesh(aiMesh* _mesh)
{
	MeshData mesh;
	mesh.vertexCount = _mesh->mNumVertices;
	mesh.vertices = new Vertex[mesh.vertexCount];

	for (int i = 0; i < _mesh->mNumVertices; i++)
	{
		mesh.vertices[i] = Vertex(_mesh->mVertices[i].x, _mesh->mVertices[i].y, _mesh->mVertices[i].z);

		mesh.vertices[i].uvX = _mesh->mTextureCoords[0][i].x;
		mesh.vertices[i].uvY = _mesh->mTextureCoords[0][i].y;
	}
	mesh.triangleCount = _mesh->mNumFaces;
	mesh.indices = new int[mesh.triangleCount * 3];

	for (int i = 0; i < mesh.triangleCount; i++)
	{
		mesh.indices[i * 3] = _mesh->mFaces[i].mIndices[0];
		mesh.indices[i * 3 + 1] = _mesh->mFaces[i].mIndices[1];
		mesh.indices[i * 3 + 2] = _mesh->mFaces[i].mIndices[2];
	}

	return mesh;
}

MoonlitEngine::MoonlitEngine() : m_engine(), m_gameObjects()
{
	//TODO: Remove test code
	m_gameObjects.push_back(GameObject(0));
	m_gameObjects[0].AddComponent(new ObjectTransform());
	m_gameObjects[0].AddComponent(new ObjectTest());
	
	
}

MoonlitEngine::~MoonlitEngine()
{
}

void MoonlitEngine::SetWindowHandle(HWND _handle)
{
	m_windowHandle = _handle;
}

void MoonlitEngine::Init(const uint32_t _width, const uint32_t _height, std::string _windowTitle)
{
	ContextInfo context = {
		.name = _windowTitle.data(),
		.width = _width,
		.height = _height,
		.windowHandle = m_windowHandle
	};

	std::vector<const char*> extensions = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
	m_engine.InitContext(context, extensions.data(), extensions.size());
	m_engine.InitVulkan();
}

void MoonlitEngine::ConnectToQTimer(QTimer* _timer)
{
	QObject::connect(_timer, &QTimer::timeout, [this]() {
		this->Update();
		});
}

void MoonlitEngine::Update()
{
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		//std::cout << "Updating a game object" << std::endl;
		try
		{
			(*it).Update();
		}
		catch (std::exception err)
		{
			std::cout << err.what() << std::endl;
		}
	}
	m_engine.Render();
}



MoonlitEngine::MoonlitEngine(const MoonlitEngine& _toCopy)
{
}