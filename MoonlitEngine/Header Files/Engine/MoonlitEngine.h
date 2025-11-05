#ifndef MOONLIT_ENGINE_H
#define MOONLIT_ENGINE_H

#include <string>
#include "Camera.h"
#include "Scene/Scene.h"

class MOONLIT_API MoonlitEngine
{ 
public:
	static MoonlitEngine* GetInstance() { return m_instance; }

	class MoonlitRenderer* Renderer;

	MoonlitEngine(void* _handle);

	void LoadPlugin(std::string _name);

	void Init();
	void Update();

	Scene& GetScene() const { return *m_activeScene; }
private:
	
	class Scene* m_activeScene;
	static MoonlitEngine* m_instance;
};

#endif // !MOONLIT_ENGINE_H
