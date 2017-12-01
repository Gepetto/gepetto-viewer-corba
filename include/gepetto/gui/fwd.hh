#ifndef GEPETTO_GUI_FWD_HH
#define GEPETTO_GUI_FWD_HH

#include <gepetto/viewer/macros.h>
#include <gepetto/gui/config-dep.hh>

namespace gepetto {
  namespace gui {
    class MainWindow;
    class OSGWidget;
    class PickHandler;
    class BodyTreeWidget;
    class BodyTreeItem;
    class ShortcutFactory;
    class SelectionHandler;
    class SelectionEvent;
    class ActionSearchBar;

    class ViewerCorbaServer;

    class WindowsManager;
    typedef graphics::shared_ptr <WindowsManager> WindowsManagerPtr_t;

#if GEPETTO_GUI_HAS_PYTHONQT
    class PythonWidget;
#endif
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_FWD_HH
