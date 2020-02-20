// Copyright (c) 2015, Joseph Mirabel
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
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// gepetto-viewer-corba. If not, see <http://www.gnu.org/licenses/>.

#include "gepetto/viewer/corba/client.hh"

#include <iostream>

namespace gepetto {
  namespace viewer {
  namespace corba {
    using CORBA::Exception;
    using CORBA::Object_var;
    using CORBA::SystemException;
    using CORBA::ORB_init;
    using CORBA::PolicyList;
    using omniORB::fatalException;

    Client::Client(int argc, char *argv[]) :
      orb_ (CORBA::ORB_init (argc, argv))
    {
    }

    bool Client::createFromDirectLink (const std::string& iiop)
    {
      std::string url = iiop + "/gepetto-gui";

      CORBA::Object_var obj = orb_->string_to_object(url.c_str());
      gui_ = gepetto::corbaserver::GraphicalInterface::_narrow(obj.in());
      return !CORBA::is_nil(gui_);
    }

    bool Client::createFromNameService (const std::string& iiop)
    {
      std::string url = iiop + "/NameService";

      CORBA::Object_var obj = orb_->string_to_object(url.c_str());
      if (CORBA::is_nil(obj)) return false;
      CosNaming::NamingContext_var nc =
        CosNaming::NamingContext::_narrow(obj.in());
      if (CORBA::is_nil(nc)) return false;

      CosNaming::Name name;
      name.length(2);
      name[0].id = (const char *) "gepetto";
      name[0].kind = (const char *) "viewer";
      name[1].id = (const char *) "corbaserver";
      name[1].kind = (const char *) "gui";
      // Invoke the root context to retrieve the object reference
      CORBA::Object_var managerObj = nc->resolve(name);
      // Narrow the previous object to obtain the correct type
      gui_ = gepetto::corbaserver::GraphicalInterface::_narrow(managerObj.in());

      return CORBA::is_nil(gui_);
    }

    void Client::connect (const std::string& iiop)
    {
      bool ok = createFromDirectLink(iiop);
      if (!ok) ok = createFromNameService(iiop);
      if (!ok) throw std::runtime_error ("Could not connect to gepetto-viewer "
          "GUI at " + iiop);
    }

    /// \brief Shutdown CORBA server
    Client::~Client()
    {
      if (!CORBA::is_nil(orb_)) {
        try {
          orb_->destroy();
          std::cout << "Ending CORBA..." << std::endl;
        } catch(const CORBA::Exception& e) {
          std::cout << "orb->destroy failed" << std::endl;
        }
      }
    }
  } // end of namespace corba.
  } // end of namespace viewer.
} // end of namespace gepetto.
