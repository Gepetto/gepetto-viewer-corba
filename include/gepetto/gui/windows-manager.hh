#ifndef GEPETTO_GUI_WINDOWSMANAGER_HH
#define GEPETTO_GUI_WINDOWSMANAGER_HH

#include <gepetto/gui/fwd.hh>
#include <gepetto/viewer/corba/windows-manager.hh>

#include <gepetto/gui/meta.hh>
#include <QColor>
#include <QVector3D>

namespace gepetto {
  namespace gui {
    template <> struct convertTo<graphics::WindowsManager::Color_t> {
      typedef graphics::WindowsManager::Color_t T;
      typedef typename T::value_type v_t;
      static inline T from (const QColor& in) {
        T v((v_t)in.redF(), (v_t)in.greenF(), (v_t)in.blueF(), (v_t)in.alphaF());
        // in.getRgbF(&v[0],&v[1],&v[2],&v[3]);
        return v;
      }
    };

    class WindowsManager : public graphics::WindowsManager
    {
      public:
        typedef graphics::WindowsManager Parent_t;

        static WindowsManagerPtr_t create ();

        WindowID createWindow(const char *windowNameCorba);
        WindowID createWindow(const char *windowNameCorba,
                              osgViewer::Viewer* viewer,
                              osg::GraphicsContext *gc);

      protected:
        WindowsManager ();
    };
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_WINDOWSMANAGER_HH
