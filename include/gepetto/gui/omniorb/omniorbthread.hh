// Copyright (c) 2015-2018, LAAS-CNRS
// Authors: Joseph Mirabel (joseph.mirabel@laas.fr)
//
// This file is part of gepetto-viewer-corba.
// gepetto-viewer-corba is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
//
// gepetto-viewer-corba is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Lesser Public License for more details. You should have
// received a copy of the GNU Lesser General Public License along with
// gepetto-viewer-corba. If not, see <http://www.gnu.org/licenses/>.

#ifndef GEPETTO_GUI_OMNIORBTHREAD_HH
#define GEPETTO_GUI_OMNIORBTHREAD_HH

#include <assert.h>

#include <QThread>
#include <QMutex>
#include <QTimerEvent>
#include <QElapsedTimer>

#include "gepetto/gui/fwd.hh"
#include "gepetto/viewer/corba/fwd.hh"

namespace gepetto {
  namespace gui {
    class ServerProcess : public QObject
    {
      Q_OBJECT

      public:
        ServerProcess () ;
        void waitForInitDone ();

signals:
        void done ();

        public slots:
          virtual void init ();
        virtual void processRequest (bool loop) = 0;

      protected:
        QMutex initDone_;
    };

    class ViewerServerProcess : public ServerProcess
    {
      Q_OBJECT

      public:
        ViewerServerProcess (graphics::corbaServer::Server* server);

        ~ViewerServerProcess ();


        public slots:
          void init ();
        void processRequest (bool loop);

      private:
        graphics::corbaServer::Server* server_;
    };

    class CorbaServer : public QObject
    {
      Q_OBJECT

      public:
        CorbaServer(ServerProcess* process);

        ~CorbaServer ();

        void wait ();

        void waitForInitDone ();

        void start ();

signals:
        void process (bool);

        protected slots:
          void timerEvent(QTimerEvent *event);
        void processed ();

      private:
        ServerProcess* control_;
        QThread worker_;
        int timerId_, interval_;
    };
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_OMNIORBTHREAD_HH
