//
//  client-cpp.hh
//  C++ CORBA client interface for SceneViewer.
//
//  Created by Mathieu Geisert in December 2014.
//  Copyright (c) 2014 LAAS-CNRS. All rights reserved.
//

#ifndef SCENEVIEWER_CORBASERVER_CLIENTCPP_HH
#define SCENEVIEWER_CORBASERVER_CLIENTCPP_HH

#include <iostream>
#include <omniORB4/CORBA.h>
#include <gepetto/viewer/corba/graphical-interface.hh>


namespace gepetto {
namespace viewer {
namespace corba {
  class Client
  {
    public:
      typedef CORBA::ULong WindowID;

      Client (int argc, char* argv[]);

      ~Client ();

      void connect (const std::string& iiop = "corbaloc:iiop:");

      gepetto::corbaserver::GraphicalInterface_var& gui () {
        return gui_;
      }

    private:
      bool createFromDirectLink(const std::string& iiop);
      bool createFromNameService(const std::string& iiop);

      corbaserver::GraphicalInterface_var gui_;

      CORBA::ORB_var orb_;
  };

  /// Initialize the connection to the viewer.
  /// \param dontRaise If True, will not raise if connection failed. It returns NULL instead.
  /// \param url if NULL, url is initialized with \c host and \c port.
  /// \param host, port if not NULL, url is set to = "corbaloc:iiop:host:port"
  void connect (const char* windowName = NULL, bool dontRaise = false,
      const char* url = NULL, const char* host = "localhost",
      const int port = 12321);

  /// Get a client to the GUI.
  corbaserver::GraphicalInterface_var& gui ();

  /// Tells whether the connection to the gui is ok.
  bool connected ()
  {
    return !CORBA::is_nil(gui());
  }

  /// Get a client to the GUI, connecting if necessary.
  /// \param windowName If not NULL, creates a window with this name if it does not exist.
  /// \param dontRaise If True, will not raise if connection failed. It returns NULL instead.
  /// \param url if NULL, url is initialized with \c host and \c port.
  /// \param host, port if not NULL, url is set to = "corbaloc:iiop:host:port"
  /// \return a client to the GUI.
  corbaserver::GraphicalInterface_var& gui (const char* windowName,
      bool dontRaise = false, const char* url = NULL,
      const char* host = "localhost", const int port = 12321)
  {
    connect (windowName, dontRaise, url, host, port);
    return gui();
  }
} //end of namespace corba
} //end of namespace viewer
} //end of namespace gepetto

#endif // SCENEVIEWER_CORBASERVER_CLIENTCPP_HH
