// Copyright (c) 2018 CNRS
// Authors: Joseph Mirabel
//
//
// This file is part of gepetto-viewer-corba
// gepetto-viewer-corba is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
//
// gepetto-viewer-corba is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// gepetto-viewer-corba  If not, see
// <http://www.gnu.org/licenses/>.

#include <plugin.hh>

#include <gepetto/gui/mainwindow.hh>
#include <gepetto/gui/settings.hh>
#include <gepetto/viewer/corba/server.hh>

#include <omniorbthread.hh>

using namespace gepetto::gui;

using graphics::corbaServer::Server;

OmniOrbServerPlugin::OmniOrbServerPlugin ()
  : server_ (NULL)
{}

OmniOrbServerPlugin::~OmniOrbServerPlugin ()
{
  if (server_ != NULL) {
    server_->wait();
    delete server_;
  }
}

void OmniOrbServerPlugin::init() {
  MainWindow* main = MainWindow::instance ();
  Settings& settings = *main->settings_;

  int argc;
  const char** argv = settings.makeOmniORBargs (argc);
  server_ = new CorbaServer (new ViewerServerProcess (
        new Server (main->osg(), argc, argv, true, settings.useNameService)));
  server_->start();
}

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
  Q_EXPORT_PLUGIN2 (omniorbserver, OmniOrbServerPlugin)
#endif
