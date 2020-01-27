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

#include <omniORB4/CORBA.h>

#include <gepetto/gui/mainwindow.hh>
#include <gepetto/gui/settings.hh>
#include <gepetto/gui/safeapplication.hh>

#include <omniorbthread.hh>
#include "../../src/server.hh"

using namespace gepetto::gui;

using gepetto::viewer::corba::Server;

class OmniOrbCatch : public SlotExceptionCatch
{
  public:
    bool safeNotify (QApplication* app, QObject* receiver, QEvent* e)
    {
      try {
        return impl_notify (app, receiver, e);
      } catch (const CORBA::Exception& e) {
        qDebug () << e._name() << " : " << e._rep_id();
        MainWindow* main = MainWindow::instance();
        if (main != NULL) {
          QString err (e._name());
          err += " : ";
          err += e._rep_id();
          main->logError (err);
        }
      }
      return false;
    }
};

OmniOrbServerPlugin::OmniOrbServerPlugin ()
  : server_ (NULL)
{}

OmniOrbServerPlugin::~OmniOrbServerPlugin ()
{
  if (server_ != NULL) {
    stopServer ();
    delete server_;
  }
}

void OmniOrbServerPlugin::stopServer ()
{
  if (server_ == NULL) return;
  server_->shutdown ();
  server_->quit ();
  server_->wait(200);
  if (server_->isRunning()) {
    server_->terminate();
    server_->wait();
  }
}

void OmniOrbServerPlugin::init()
{
  SafeApplication* app = dynamic_cast<SafeApplication*>(QApplication::instance());
  if (app) app->addAsLeaf(new OmniOrbCatch);

  MainWindow* main = MainWindow::instance ();
  Settings& settings = *main->settings_;

  int argc;
  const char** argv = settings.makeOmniORBargs (argc);
  server_ = new ViewerServerProcess (
        new Server (main->osg(), argc, argv, true, settings.useNameService));
  server_->start();
}

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
  Q_EXPORT_PLUGIN2 (omniorbserver, OmniOrbServerPlugin)
#endif
