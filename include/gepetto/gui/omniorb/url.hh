#ifndef GEPETTO_GUI_OMNIORB_URL_HH
#define GEPETTO_GUI_OMNIORB_URL_HH

#include <QString>

namespace gepetto {
  namespace gui {
    namespace omniOrb {
      static inline QString IIOPurl (const QString& host, const QString& port)
      {
        QString hp;
        if (host.isEmpty ()) {
          if (!port.isEmpty ()) hp = QString ("localhost:%1").arg (port);
        } else {
          if (port.isEmpty ()) hp = QString ("%1:2809").arg (host);
          else hp = QString ("%1:%2").arg(host).arg(port);
        }
        return QString ("corbaloc:iiop:%1/NameService").arg (hp);
      }
    } // namespace omniOrb
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_OMNIORB_URL_HH
