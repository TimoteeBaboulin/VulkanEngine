#ifndef MOONLIT_ENGINE_H
#define MOONLIT_ENGINE_H

#include "Renderer/Renderer.h"

class MOONLIT_API MoonlitEngine
{ 
public:
	MoonlitEngine(void* _handle);

	void LoadMesh(std::string name) { m_renderer.LoadMesh(name); };
	Renderer& GetRenderer() { return m_renderer; }

	void Init();
	void Update();

private:
	Renderer m_renderer;
};

#endif // !MOONLIT_ENGINE_H
