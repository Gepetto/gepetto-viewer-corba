#ifndef GEPETTO_GUI_PICK_HANDLER_HH
#define GEPETTO_GUI_PICK_HANDLER_HH

#include <osgGA/GUIEventHandler>
#include <gepetto/viewer/node.h>

#include <QObject>
#include <QModelIndex>

#include <gepetto/gui/fwd.hh>
#include <gepetto/gui/qt-osg-keyboard.hh>

namespace gepetto {
  namespace gui {
    class PickHandler : public QObject, public osgGA::GUIEventHandler
    {
      Q_OBJECT

    public:
      PickHandler (OSGWidget* parent, WindowsManagerPtr_t wsm);

      virtual ~PickHandler();

      virtual bool handle( const osgGA::GUIEventAdapter&  ea,
                                 osgGA::GUIActionAdapter& aa );

      void getUsage (osg::ApplicationUsage &usage);

    private:
      std::list <graphics::NodePtr_t> computeIntersection (osgGA::GUIActionAdapter& aa,
                                                           const float& x, const float& y,
							   int modMask);

      void setCameraToSelected (osgGA::GUIActionAdapter& aa, bool zoom);

      QtOsgKeyboard mapper_;
      WindowsManagerPtr_t wsm_;
      OSGWidget* parent_;
      bool pushed_;
      float lastX_, lastY_;
    };
  }
}

#endif // GEPETTO_GUI_PICK_HANDLER_HH
