#ifndef GEPETTO_GUI_WINDOWSMANAGER_HH
#define GEPETTO_GUI_WINDOWSMANAGER_HH

#include <gepetto/gui/fwd.hh>
#include <gepetto/viewer/corba/windows-manager.hh>

#include <gepetto/gui/meta.hh>
#include <QColor>
#include <QVector3D>

namespace gepetto {
  namespace gui {
    template <> struct Traits <gepetto::corbaserver::Color_var> {
      typedef gepetto::corbaserver::Color_var type;
      static inline type from (const float* in) {
        type out = new float[4];
        convertSequence <float, float> (in, out.inout(), 4);
        return out;
      }
      static inline type from (const double* in) {
        type out = new float[4];
        convertSequence <double, float> (in, out.inout(), 4);
        return out;
      }
      static inline type from (const QColor& in) {
        qreal v[4];
        in.getRgbF(&v[0],&v[1],&v[2],&v[3]);
        return from (v);
      }
    };
    template <> struct Traits <gepetto::corbaserver::Position_var> {
      typedef gepetto::corbaserver::Position_var type;
      static inline type from (const float* in) {
        type out = new float[4];
        convertSequence <float, float> (in, out.inout(), 4);
        return out;
      }
      static inline type from (const double* in) {
        type out = new float[4];
        convertSequence <double, float> (in, out.inout(), 4);
        return out;
      }
      static inline type from (const QVector3D& in) {
        qreal v[3]; v[0] = in.x(); v[1] = in.y(); v[2] = in.z();
        return from (v);
      }
      static inline type from (const int& in) {
        qreal v[3]; v[0] = in / (qreal)50; v[1] = v[0]; v[2] = v[0];
        return from (v);
      }

    };

    template <> struct Traits <int> {
      struct type {
          float value;
          const float& in() { return value; }
      };

      static inline type from(const int& in) {
          type out;
          out.value = (float)(in / 100.0);
          return out;
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
