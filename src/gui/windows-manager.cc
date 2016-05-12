#include "gepetto/gui/windows-manager.hh"

#include <gepetto/viewer/window-manager.h>

#include "gepetto/gui/osgwidget.hh"
#include "gepetto/gui/mainwindow.hh"

namespace gepetto {
  namespace gui {
    WindowsManagerPtr_t WindowsManager::create()
    {
      return WindowsManagerPtr_t (new WindowsManager());
    }

    WindowsManager::WindowID WindowsManager::createWindow(const char *windowNameCorba)
    {
      return MainWindow::instance()->delayedCreateView(QString (windowNameCorba))->windowID();
    }

    WindowsManager::WindowID WindowsManager::createWindow(const char *windowNameCorba,
                                                          osgViewer::Viewer *viewer,
                                                          osg::GraphicsContext *gc)
    {
      std::string wn (windowNameCorba);
      graphics::WindowManagerPtr_t newWindow = graphics::WindowManager::create (viewer, gc);
      WindowID windowId = addWindow (wn, newWindow);
      return windowId;
    }

    WindowsManager::WindowsManager()
      : Parent_t ()
    {
    }
  } // namespace gui
} // namespace gepetto
