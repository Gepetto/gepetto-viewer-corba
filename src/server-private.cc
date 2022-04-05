// Copyright (C) 2014 by Mathieu Geisert, LAAS-CNRS.
//
// This file is part of the SceneViewer-corba.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#include "server-private.hh"

#include <iostream>
#include <stdexcept>

#include "graphical-interface.impl.hh"

namespace gepetto {
namespace viewer {
namespace corba {
namespace impl {
using CORBA::COMM_FAILURE;
using CORBA::Exception;
using CORBA::Object_ptr;
using CORBA::Object_var;
using CORBA::ORB_init;
using CORBA::PolicyList;
using CORBA::SystemException;
using omniORB::fatalException;

typedef CORBA::ORB::InvalidName InvalidName;

Server::~Server() {
  // TODO for a reason I do not understand, this SEGV.
  // delete graphicalInterfaceServantid_;
}

void Server::createServant(corba::Server* inServer) {
  graphicalInterfaceServant_ = new GraphicalInterface(inServer);
}

void Server::deactivateAndDestroyServers() {
  // TODO for a reason I do not understand, this SEGV.
  // poa_->deactivate_object(*graphicalInterfaceServantid_);
}

void Server::createContext() {
  CosNaming::NamingContext_var rootContext;
  Object_var localObj;
  CosNaming::Name contextName;

  // Obtain a reference to the root context of the Name service:
  localObj = orb_->resolve_initial_references("NameService");
  try {
    // Narrow the reference returned.
    rootContext = CosNaming::NamingContext::_narrow(localObj);
    if (is_nil(rootContext)) {
      std::string msg("Failed to narrow the root naming context.");
      throw std::runtime_error(msg.c_str());
    }
  } catch (InvalidName& ex) {
    // This should not happen!
    std::string msg("Service required is invalid [does not exist].");
    throw std::runtime_error(msg.c_str());
  }
  // Bind a context called "Graphics" to the root context:
  contextName.length(1);
  contextName[0].id = (const char*)"gepetto";   // string copied
  contextName[0].kind = (const char*)"viewer";  // string copied
  // Note on kind: The kind field is used to indicate the type
  // of the object. This is to avoid conventions such as that used
  // by files (name.type -- e.g. Graphics.ps = postscript etc.)

  try {
    // Bind the context to root.
    Context_ = rootContext->bind_new_context(contextName);
  } catch (CosNaming::NamingContext::AlreadyBound& ex) {
    // If the context already exists, this exception will be raised.
    // In this case, just resolve the name and assign Context
    // to the object returned:
    Object_var localObj;
    localObj = rootContext->resolve(contextName);
    Context_ = CosNaming::NamingContext::_narrow(localObj);
    if (is_nil(Context_)) {
      std::string msg("Failed to narrow naming context.");
      throw std::runtime_error(msg.c_str());
    }
  }
}

void Server::initRootPOA() {
  graphicalInterfaceServantid_ =
      poa_->activate_object(graphicalInterfaceServant_);
}

void Server::initOmniINSPOA() {
  objectId_ = PortableServer::string_to_ObjectId("gepetto-gui");
  poa_->activate_object_with_id(objectId_, graphicalInterfaceServant_);
}

void Server::bindObjectToName(Object_ptr objref, CosNaming::Name objectName) {
  try {
    Context_->bind(objectName, objref);
  } catch (CosNaming::NamingContext::AlreadyBound& ex) {
    Context_->rebind(objectName, objref);
  }

  // Amendment: When using OrbixNames, it is necessary to first
  // try bind and then rebind, as rebind on it's own will throw
  // a NotFoundexception if the Name has not already been
  // bound. [This is incorrect behaviour - it should just bind].
}
}  // end of namespace impl.
}  // end of namespace corba.
}  // end of namespace viewer.
}  // end of namespace gepetto.
