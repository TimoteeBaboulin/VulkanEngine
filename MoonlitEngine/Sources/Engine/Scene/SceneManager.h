#ifndef MOONLIT_SCENEMANAGER_H
#define MOONLIT_SCENEMANAGER_H
#include "Engine/Events/Event.h"

namespace Moonlit
{
    class Scene;
}

namespace Moonlit::SceneManagement
{
    static Events::Event<void, Scene*> OnSceneLoaded;
    static Events::Event<void, Scene*> OnSceneUnloaded;
}

#endif //MOONLIT_SCENEMANAGER_H