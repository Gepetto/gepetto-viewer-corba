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


namespace graphics {
namespace corbaServer {
  class Client
  {
    public:
      typedef CORBA::ULong WindowID;

      Client (int argc, char* argv[]);

      ~Client ();

      void connect (const char* iiop = "corbaloc:rir:/NameService");

      gepetto::corbaserver::GraphicalInterface_var& gui () {
        return gui_;
      }

    private:
      gepetto::corbaserver::GraphicalInterface_var gui_;

      CORBA::ORB_var orb_;
  };
} //end of namespace corbaserver
} //end of namespace graphics

#endif // SCENEVIEWER_CORBASERVER_CLIENTCPP_HH
