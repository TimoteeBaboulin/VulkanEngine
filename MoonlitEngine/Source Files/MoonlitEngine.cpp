//#include "MoonlitEngine.h"
//
//#include "GameObject.h"
//#include "ObjectTransform.h"
//#include "ObjectTest.h"
//
//#include "MoonlitVulkan/ResourceBank.h"
//#include "MoonlitVulkan/Mesh.h"
//
//#include <QtCore/qtimer.h>
//#include "assimp/Importer.hpp"
//#include "assimp/postprocess.h"
//#include "assimp/mesh.h"
//#include "assimp/scene.h"
//
//MeshData ImportMesh(std::string _path);
//
////Mesh GetMesh(std::string _filepath)
////{
////	MeshData data = ImportMesh(_filepath);
////	Mesh mesh;
////	mesh.Load()
////	return Mesh(data);
////}
//
//MoonlitEngine::MoonlitEngine() : m_engine(), m_gameObjects()
//{
//	//TODO: Remove test code
//	m_gameObjects.push_back(GameObject(0));
//	m_gameObjects[0].AddComponent(new ObjectTransform());
//	m_gameObjects[0].AddComponent(new ObjectTest());
//}
//
//MoonlitEngine::~MoonlitEngine()
//{
//}
//
//void MoonlitEngine::SetWindowHandle(HWND _handle)
//{
//	m_windowHandle = _handle;
//}
//
//void MoonlitEngine::Init(const uint32_t _width, const uint32_t _height, std::string _windowTitle)
//{
//	ContextInfo context = {
//		.name = _windowTitle.data(),
//		.width = _width,
//		.height = _height,
//		.windowHandle = m_windowHandle
//	};
//
//	std::vector<const char*> extensions = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
//	m_engine.InitContext(context, extensions.data(), extensions.size());
//	m_engine.InitVulkan();
//}
//
//void MoonlitEngine::ConnectToQTimer(QTimer* _timer)
//{
//	QObject::connect(_timer, &QTimer::timeout, [this]() {
//		this->Update();
//		});
//}
//
//void MoonlitEngine::Update()
//{
//	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
//	{
//		//std::cout << "Updating a game object" << std::endl;
//		try
//		{
//			(*it).Update();
//		}
//		catch (std::exception err)
//		{
//			std::cout << err.what() << std::endl;
//		}
//	}
//	m_engine.Render();
//}
//
//
//
//MoonlitEngine::MoonlitEngine(const MoonlitEngine& _toCopy)
//{
//}