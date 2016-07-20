#ifndef GEPETTO_GUI_FWD_HH
#define GEPETTO_GUI_FWD_HH

#include <boost/shared_ptr.hpp>
#include <gepetto/gui/config-dep.hh>

namespace gepetto {
  namespace gui {
    class MainWindow;
    class OSGWidget;
    class PickHandler;
    class BodyTreeWidget;
    class BodyTreeItem;
    class ShortcutFactory;

    class ViewerCorbaServer;

    class WindowsManager;
    typedef boost::shared_ptr <WindowsManager> WindowsManagerPtr_t;

#if GEPETTO_GUI_HAS_PYTHONQT
    class PythonWidget;
#endif
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_FWD_HH
