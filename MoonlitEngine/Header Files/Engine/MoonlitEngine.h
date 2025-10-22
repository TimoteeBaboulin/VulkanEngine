#ifndef MOONLIT_ENGINE_H
#define MOONLIT_ENGINE_H

#include "Renderer/Renderer.h"

class MOONLIT_API MoonlitEngine
{ 
public:
	static MoonlitEngine* GetInstance() { return m_instance; }

	MoonlitEngine(void* _handle);

	void LoadMesh(std::string name) { m_renderer.LoadMesh(name); };
	Renderer& GetRenderer() { return m_renderer; }

	void Init();
	void Update();

	void AddRenderTarget(void* _handle, Camera* _camera) { m_renderer.AddRenderTarget(_handle, _camera); };
private:
	Renderer m_renderer;
	class Scene* m_activeScene;
	static MoonlitEngine* m_instance;
};

#endif // !MOONLIT_ENGINE_H
