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

class ViewerServerProcess : public QThread
{
  Q_OBJECT

  public:
    ViewerServerProcess (gepetto::viewer::corba::Server* server, QObject* parent = NULL);

    ~ViewerServerProcess ();

    void shutdown ();

  protected:
    void run ();

  private:
    gepetto::viewer::corba::Server* server_;
};

#endif // GEPETTO_GUI_OMNIORBTHREAD_HH
