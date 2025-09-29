#ifdef EDITOR_BUILD

#define VK_USE_PLATFORM_WIN32_KHR

#include "MoonlitEditor.h"

int main(int argc, char** argv)
{
	MoonlitEditor editor;
 //   QApplication* application = new QApplication(argc, argv);
 //   QMainWindow* mainWindow = new QMainWindow();
 //   mainWindow->resize(WindowWidth, WindowHeight);
 //   mainWindow->show();

	//QDockWidget* gameViewDock = new QDockWidget("Game View", mainWindow);
	//mainWindow->addDockWidget(Qt::TopDockWidgetArea, gameViewDock);

 //   gameViewDock->setAttribute(Qt::WidgetAttribute::WA_NativeWindow);
 //   gameViewDock->show();
 //   gameViewDock->createWinId();

	//QDockWidget* secondDock = new QDockWidget("Second Dock", mainWindow);
 //   mainWindow->addDockWidget(Qt::BottomDockWidgetArea, secondDock);
	//secondDock->setFixedHeight(200);
 //   //gameViewDock->setBackgroundRole(QPalette::Highlight);
 //   secondDock->show();

 //   application->processEvents();
 //   HWND winHandle = (HWND)gameViewDock->effectiveWinId();
	//HWND mainHandle = (HWND)mainWindow->effectiveWinId();

	//std::cout << IsWindow(winHandle) << std::endl;

 //   MoonlitEngine engine(winHandle);
 //   QTimer* timer = new QTimer(0);
 //   timer->setSingleShot(false);
 //   std::function<void(void)> updateFunction = std::bind(&MoonlitEngine::Update, &engine);
 //   timer->connect(timer, &QTimer::timeout, updateFunction);
 //   //timer->setParent(application);
 //   timer->start();

 //   engine.Init();

 //   //InputManager::GetInstance()->SubscribeMouseEvent(KEY_STATE::PRESSED, MousePressedCallback);
 //   //InputManager::GetInstance()->SubscribeGamepadEvent(KEY_STATE::PRESSED, GamepadPressedCallback);
 //   //InputManager::GetInstance()->SubscribeGamepadAxisEvent(GamepadAxisCallback);

 //   return application->exec();
}

#endif //EDITOR_BUILD