// Copyright (C) 14 by Mathieu Geisert LAAS-CNRS.
//
// This file is part of the SceneViewer-corba.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#include <iostream>
#include "gepetto/viewer/corba/server.hh"

using graphics::corbaServer::Server;

int
main (int argc, const char* argv[])
{
  graphics::WindowsManagerPtr_t wm = graphics::WindowsManager::create ();
  Server server (wm, argc, argv, false); // dernier arg : autorise ou non multithread (original = true)

  server.startCorbaServer ();
  server.processRequest(true);
}
