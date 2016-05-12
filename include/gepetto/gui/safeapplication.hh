#ifndef GEPETTO_GUI_SAFEAPPLICATION_HH
#define GEPETTO_GUI_SAFEAPPLICATION_HH

#include <QApplication>

namespace gepetto {
  namespace gui {
    class SafeApplication : public QApplication
    {
      Q_OBJECT

      public:
        explicit SafeApplication (int& argc, char ** argv);

        virtual bool notify(QObject* receiver, QEvent* e);

      signals:
        void log (QString text);
    };
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_SAFEAPPLICATION_HH
