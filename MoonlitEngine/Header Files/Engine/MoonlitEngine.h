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

	/// <summary>
	/// Method to load a plugin by path and register its behaviours <para/>
	/// If the method throws an exception with error type of "REGISTER_FUNCTION_MISSING", <para/>
	/// the module handle will not be disposed of by the method to allow debugging it manually.
	/// </summary>
	/// <exception cref="PluginLoadException">  </exception>
	void LoadPlugin(std::string _name);

	void Init();
	void Update();

	Scene& GetScene() const { return *m_activeScene; }
private:
	
	class Scene* m_activeScene;
	static MoonlitEngine* m_instance;
};

#endif // !MOONLIT_ENGINE_H
