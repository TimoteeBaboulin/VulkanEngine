//#define WIN32_LEAN_AND_MEAN
//
//#include "Renderer/Renderer.h"
//
//
//
//#include <QtCore/qapplicationstatic.h>
//#include <QtWidgets/qwidget.h>
//#include <QtWidgets/qapplication.h>
//#include <QtCore/qtimer.h>
//
//#include "MoonlitEngine.h"
//
//constexpr int WindowWidth = 1920;
//constexpr int WindowHeight = 1080;
//
//void falsemain(int argc, char** argv)
//{
//    MoonlitEngine engine;
//    QApplication* application = new QApplication(argc, argv);
//    QWidget* window = new QWidget();
//    window->resize(WindowWidth, WindowHeight);
//    window->show();
//    HWND winHandle = (HWND)window->effectiveWinId();
//    QTimer* timer = new QTimer(0);
//    timer->setSingleShot(false);
//    timer->moveToThread(application->thread());
//
//    engine.SetWindowHandle(winHandle);
//    engine.Init(WindowWidth, WindowHeight, "Vulkan Engine");
//    engine.ConnectToQTimer(timer);
//    timer->start();
//
//    application->exec();
//}
