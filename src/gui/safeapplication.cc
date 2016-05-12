#include "gepetto/gui/safeapplication.hh"

#include <QDebug>
#include <omniORB4/CORBA.h>
#include <gepetto/gui/mainwindow.hh>
#include <gepetto/gui/plugin-interface.hh>

namespace gepetto {
  namespace gui {
    SafeApplication::SafeApplication(int& argc, char** argv) :
      QApplication(argc, argv)
    {
    }

    bool SafeApplication::notify(QObject *receiver, QEvent *e)
    {
      try {
        return QApplication::notify(receiver, e);
      } catch (const std::exception& e) {
        qDebug () << e.what();
      } catch (const CORBA::TRANSIENT& e) {
        emit log (QString ("CORBA Exception %1 - %2.\nYou may need to reset the connections (see Tools menu)").arg(e._name()).arg(e._rep_id()));
      } catch (const CORBA::COMM_FAILURE& e) {
        emit log (QString ("CORBA Exception %1 - %2.\nYou may need to reset the connections (see Tools menu)").arg(e._name()).arg(e._rep_id()));
      } catch (const CORBA::Exception& e) {
        bool handled = false;
        foreach (CorbaInterface* errorHandler, MainWindow::instance()->pluginManager()->get <CorbaInterface>()) {
          if (errorHandler->corbaException (0, e)) {
            handled = true;
            break;
          }
        }
        if (!handled)
          emit log (QString ("Unhandled CORBA Exception %1 - %2").arg(e._name()).arg(e._rep_id()));
      } catch (...) {
        qDebug() << "Unknown exception";
        qDebug() << "Catch it in SafeApplication::notify";
      }
      return false;
    }
  } // namespace gui
} // namespace gepetto
