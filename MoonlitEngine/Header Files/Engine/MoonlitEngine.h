#ifndef MOONLIT_ENGINE_H
#define MOONLIT_ENGINE_H

#include <string>
#include "Camera.h"

class MOONLIT_API MoonlitEngine
{ 
public:
	static MoonlitEngine* GetInstance() { return m_instance; }

	MoonlitEngine(void* _handle);

	void LoadPlugin(std::string _name);
	void LoadMesh(std::string name);
	class Renderer& GetRenderer() { return *m_renderer; }

	void Init();
	void Update();

	void AddMeshInstance(struct MeshInstance& _meshInstance);
	void AddRenderTarget(void* _handle, Camera* _camera);
private:
	class Renderer* m_renderer;
	class Scene* m_activeScene;
	static MoonlitEngine* m_instance;
};

#endif // !MOONLIT_ENGINE_H
