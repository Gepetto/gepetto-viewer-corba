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
#include <osg/DisplaySettings>
#include <osg/ArgumentParser>
#include "gepetto/viewer/corba/server.hh"

using graphics::corbaServer::Server;

int
main (int argc, char* argv[])
{
  osg::ArgumentParser arguments(&argc, argv);
  osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();

  // This should not be done here. Two osg built-in way:
  // - Environment variable OSG_MULTI_SAMPLES  : a bit tedious
  // - Command line option --samples           : slight better with an alias
#ifndef DISABLE_SAMPLING
  ds->setNumMultiSamples(4);
#else
  ds->setNumMultiSamples(0);
#endif

  // set up the usage document, in case we need to print out how to use this program.
  arguments.getApplicationUsage()->setApplicationName(arguments.getApplicationName());
  // arguments.getApplicationUsage()->setDescription(arguments.getApplicationName()+" is an OpenSceneGraph example that shows how to use the accumulation buffer to achieve a simple motion blur effect.");
  arguments.getApplicationUsage()->setCommandLineUsage(arguments.getApplicationName()+" [options]");
  arguments.getApplicationUsage()->addCommandLineOption("-h or --help","Display this information");
  ds->readCommandLine (arguments); // populate the help message.

  // if user request help write it out to cout.
  if (arguments.read("-h") || arguments.read("--help")) {
    arguments.getApplicationUsage()->write(std::cout);
    return 1;
  }

  graphics::WindowsManagerPtr_t wm = graphics::WindowsManager::create ();
  Server server (wm, argc, (const char**)argv, false); // dernier arg : autorise ou non multithread (original = true)

  server.startCorbaServer ();
  server.processRequest(true);
}
