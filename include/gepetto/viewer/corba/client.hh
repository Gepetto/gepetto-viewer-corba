//
//  client-cpp.hh
//  C++ CORBA client interface for SceneViewer.
//
//  Created by Mathieu Geisert in December 2014.
//  Copyright (c) 2014 LAAS-CNRS. All rights reserved.
//

#ifndef SCENEVIEWER_CORBASERVER_CLIENTCPP_HH
#define SCENEVIEWER_CORBASERVER_CLIENTCPP_HH

#include <omniORB4/CORBA.h>

#include <gepetto/viewer/corba/graphical-interface.hh>
#include <iostream>

namespace gepetto {
namespace viewer {
namespace corba {
class Client {
 public:
  typedef CORBA::ULong WindowID;

  Client(int argc, char* argv[]);

  ~Client();

  void connect(const std::string& iiop = "corbaloc:iiop:");

  gepetto::corbaserver::GraphicalInterface_var& gui() { return gui_; }

 private:
  bool createFromDirectLink(const std::string& iiop);
  bool createFromNameService(const std::string& iiop);

  corbaserver::GraphicalInterface_var gui_;

  CORBA::ORB_var orb_;
};

/// Initialize the connection to the viewer.
/// \param dontRaise If True, will not raise if connection failed. It returns
/// NULL instead. \param url if NULL, url is initialized with \c host and \c
/// port. \param host, port if not NULL, url is set to =
/// "corbaloc:iiop:host:port"
void connect(const char* windowName = NULL, bool dontRaise = false,
             const char* url = NULL, const char* host = "localhost",
             const int port = 12321);

/// Get a client to the GUI.
corbaserver::GraphicalInterface_var& gui();

/// Tells whether the connection to the gui is ok.
inline bool connected() { return !CORBA::is_nil(gui()); }

/// Get a client to the GUI, connecting if necessary.
/// \param windowName If not NULL, creates a window with this name if it does
/// not exist. \param dontRaise If True, will not raise if connection failed. It
/// returns NULL instead. \param url if NULL, url is initialized with \c host
/// and \c port. \param host, port if not NULL, url is set to =
/// "corbaloc:iiop:host:port" \return a client to the GUI.
inline corbaserver::GraphicalInterface_var& gui(const char* windowName,
                                                bool dontRaise = false,
                                                const char* url = NULL,
                                                const char* host = "localhost",
                                                const int port = 12321) {
  connect(windowName, dontRaise, url, host, port);
  return gui();
}

#if __cplusplus >= 201103L
constexpr corbaserver::Color white{1.0f, 1.0f, 1.0f, 1.0f},
    lightWhite{0.9f, 0.9f, 0.9f, 1.0f}, green{0.0f, 1.0f, 0.0f, 1.0f},
    lightGreen{0.0f, 0.9f, 0.0f, 1.0f}, yellow{1.0f, 1.0f, 0.0f, 1.0f},
    lightYellow{0.9f, 0.9f, 0.0f, 1.0f}, blue{0.0f, 0.0f, 1.0f, 1.0f},
    lightBlue{0.0f, 0.0f, 0.9f, 1.0f}, grey{0.7f, 0.7f, 0.7f, 1.0f},
    lightGrey{0.7f, 0.7f, 0.7f, 0.7f}, red{1.0f, 0.0f, 0.0f, 1.0f},
    lightRed{0.9f, 0.0f, 0.0f, 1.0f}, black{0.0f, 0.0f, 0.0f, 1.0f},
    lightBlack{0.1f, 0.1f, 0.1f, 1.0f}, brown{0.85f, 0.75f, 0.15f, 1.0f},
    lightBrown{0.75f, 0.65f, 0.10f, 1.0f};
#endif

}  // end of namespace corba
}  // end of namespace viewer
}  // end of namespace gepetto

#endif  // SCENEVIEWER_CORBASERVER_CLIENTCPP_HH
