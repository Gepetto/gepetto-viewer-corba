// Copyright (C) 2014 by Mathieu Geisert, LAAS-CNRS.
//
// This file is part of the SceneViewer-corba.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#ifndef SCENEVIEWER_CORBASERVER_SERVER_HH
# define SCENEVIEWER_CORBASERVER_SERVER_HH

#include "gepetto/viewer/corba/fwd.hh"
#include <gepetto/gui/windows-manager.hh>

class QObject;

namespace gepetto {
  namespace viewer {
  namespace corba {
    using gepetto::gui::WindowsManager;
    using gepetto::gui::WindowsManagerPtr_t;

    /// Implementation of Hpp module Corba server.

    ///  This class initializes the Corba server and starts the following Corba interface implementations.
    ///  \li hpp::Robot: to build a model::Device and to insert it in a core::ProblemSolver object,
    ///  \li hpp::Obstacle: to build obstacles and insert them in a core::ProblemSolver object,
    ///  \li hpp::Problem: to define a path planning problem and solve it.

    ///  To use this object, call the constructor

    ///  \code
    ///  int argc=1;
    ///  char *argv[1] = {"program"};
    ///  core::ProblemSolverPtr_t problemSolver = new core::ProblemSolver;
    ///  Server server(problemSolver, argc, argv, isMultiThread);
    ///  \endcode
    ///  where \c isMultiThread specifies whether the server should process
    ///  requests using multi-thread policy of not.

    ///  After starting a name server and configuring your Corba implementation,
    ///  start the servers.
    ///  \code
    ///  server.startCorbaServer();
    ///  \endcode
    ///  Then, enter in the loop that handle the Corba requests
    ///  \code
    ///  server.processRequest(true);
    ///  \endcode
    ///  You can then send request to the servers.
    class /*SCENEVIEWER_CORBASERVER_DLLAPI*/ Server
    {
    public:
      /// Constructor
      /// \param the object that handles CORBA requests.
      /// \param argc, argv parameter to feed ORB initialization.
      /// \param multiThread whether the server may process request using
      ///        multithred policy.
      /// \note It is recommended to configure your Corba implementation
      ///       through environment variables and to set argc to 1 and argv to
      ///       any string.
      /// \note It is highly recommended not to enable multi-thread policy in
      ///       CORBA request processing if this library is run from an openGL
      ///       based GUI, since OpenGL does not support multithreading.
      Server (WindowsManagerPtr_t windowsManager, int argc,
          const char* argv[], bool multiThread = false, bool useNameService = false);

      /// \brief Shutdown CORBA server
      ~Server ();

      /// \brief Initialize CORBA server to process requests from clients
      /// \return 0 if success, -1 if failure.
      void startCorbaServer ();

      /// \brief If ORB work is pending, process it
      /// \param loop if true, the function never returns,
      ///             if false, the function processes pending requests and
      ///             returns.
      int processRequest (bool loop);

      /// Stop processing requests.
      /// \param wait if true, the method waits for the server to be shut down.
      /// \warning From a servant method, set wait to false. Otherwise the
      ///          application will be deadlocked.
      void shutdown (bool wait);

      WindowsManagerPtr_t windowsManager () const
      {
        return windowsManager_;
      }

      void qparent (QObject* parent);

    private:

      ///\name CORBA server initialization
      /// \{
      /// Initialize ORB and CORBA servers.

      /// \param argc, argv parameter to feed ORB initialization.
      /// \param multiThread whether the server may process request using
      ///        multithred policy.
      void initORBandServers (int argc, const char* argv[], bool multiThread, bool useNameService);

      /// \}

      impl::Server* private_;

      WindowsManagerPtr_t windowsManager_;
    };

  } // end of namespace corba.
  } // end of namespace viewer.
} // end of namespace gepetto.
#endif
