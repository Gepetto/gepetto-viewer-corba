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

#include "omniorbthread.hh"

#include <gepetto/viewer/corba/server.hh>
#include <QDebug>

#include <gepetto/gui/mainwindow.hh>
#include <gepetto/gui/plugin-interface.hh>

ViewerServerProcess::ViewerServerProcess (gepetto::viewer::corba::Server *server)
  : server_ (server), timerId_ (-1), interval_ (100)
{}

ViewerServerProcess::~ViewerServerProcess()
{
  delete server_;
}

void ViewerServerProcess::init()
{
  server_->qparent (this);
  server_->startCorbaServer ();

  timerId_ = startTimer(interval_);
}

void ViewerServerProcess::timerEvent(QTimerEvent* event)
{
  if (event->timerId () == timerId_)
    processRequest (false);
}

void ViewerServerProcess::processRequest(bool loop)
{
  server_->processRequest (loop);
}

CorbaServer::CorbaServer (ViewerServerProcess* process) :
  QObject (), control_ (process), worker_ ()
{
  control_->moveToThread(&worker_);

  connect (&worker_, SIGNAL (started()), control_, SLOT (init()));
}

CorbaServer::~CorbaServer()
{
  if (control_)
    delete control_;
}

void CorbaServer::wait ()
{
  worker_.quit ();
  worker_.wait(200);
  if (worker_.isRunning()) {
    worker_.terminate();
    worker_.wait();
  }
}

void CorbaServer::start()
{
  worker_.start();
}
