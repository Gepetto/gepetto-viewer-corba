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
#include "server.hh"

using Graphics::corbaServer::Server;

int
main (int argc, const char* argv[])
{
  Server server (argc, argv, true);

  server.startCorbaServer ();
  server.processRequest(true);
}
