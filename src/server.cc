// Copyright (C) 2014, 2010 by Mathieu Geisert, LAAS-CNRS.
//
// This file is part of the SceneViewer-corba.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#include <errno.h>
#include <pthread.h>
#include <iostream>
#include <stdexcept>

#include "gepetto/viewer/corba/server.hh"
#include "server-private.hh"

namespace graphics
{
  namespace corbaServer
  {
    using CORBA::Exception;
    using CORBA::Object_var;
    using CORBA::SystemException;
    using CORBA::ORB_init;
    using CORBA::PolicyList;
    using omniORB::fatalException;

    //namespace
    //{
      /// \brief Forward logging messages to hpp logging mechanism.
      /// If debug is disabled, CORBA logging will be disabled too.
      ///
      /// Tracing has to be enabled in your ``omniORB.cfg'' to use this
      /// feature.
      /// See ``omniORB configuration and API'' > ``Tracing options''
      /// section of omniORB manual for more information.
      //void logFunction (const char* msg);

    //} // end of anonymous namespace.


    Server::Server(WindowsManagerPtr_t wm, int argc, const char *argv[],
        bool inMultiThread) : windowsManager_ (wm)
    {
      private_ = new impl::Server;

      initORBandServers (argc, argv, inMultiThread);
    }

    /// \brief Shutdown CORBA server
    Server::~Server()
    {
      private_->deactivateAndDestroyServers();
      delete private_;
      private_ = NULL;
    }

    /// CORBA SERVER INITIALIZATION

    void Server::initORBandServers(int argc, const char* argv[],
				   bool inMultiThread)
    {
      Object_var obj;
      PortableServer::ThreadPolicy_var threadPolicy;
      PortableServer::POA_var rootPoa;

      /// ORB init
      private_->orb_ = ORB_init (argc, const_cast<char **> (argv));
      if (is_nil(private_->orb_)) {
	std::string msg ("failed to initialize ORB");
	throw std::runtime_error (msg.c_str ());
      }
      /// ORB init
      obj = private_->orb_->resolve_initial_references("RootPOA");

      /// Create thread policy
      //
      // Make the CORBA object single-threaded to avoid GUI krash
      //
      // Create a sigle threaded policy object
      rootPoa = PortableServer::POA::_narrow(obj);

      if (inMultiThread) {
	threadPolicy = rootPoa->create_thread_policy
	  (PortableServer::ORB_CTRL_MODEL);
      }
      else {
	threadPolicy = rootPoa->create_thread_policy
	  (PortableServer::MAIN_THREAD_MODEL);
      }
      /// Duplicate thread policy
      PolicyList policyList;
      policyList.length(1);
      policyList[0] = PortableServer::ThreadPolicy::_duplicate(threadPolicy);

      try {
        private_->poa_ = rootPoa->create_POA
          ("child", PortableServer::POAManager::_nil(), policyList);
      } catch (PortableServer::POA::AdapterAlreadyExists& /*e*/) {
        private_->poa_ = rootPoa->find_POA ("child", false);
      }
      // Destroy policy object
      threadPolicy->destroy();
      private_->createAndActivateServers(this);
    }

    void Server::startCorbaServer()
    {
      // Obtain a reference to objects, and register them in
      // the naming service.
      Object_var graphicalInterfaceObj = private_->graphicalInterfaceServant_->_this();

      private_->createContext ();
      // Bind graphicalInterfaceObj with name graphicalinterface to the Context:
      CosNaming::Name objectName;
      objectName.length(1);
      objectName[0].id   = (const char*) "corbaserver";   // string copied
      objectName[0].kind = (const char*) "gui"; // string copied

      private_->bindObjectToName(graphicalInterfaceObj, objectName);
      private_->graphicalInterfaceServant_->_remove_ref();

      PortableServer::POAManager_var pman = private_->poa_->the_POAManager();
      pman->activate();
    }

    /// \brief If CORBA requests are pending, process them
    int Server::processRequest (bool loop)
    {
      if (loop)
	{
	  private_->orb_->run();
	}
      else
	{
	  if (private_->orb_->work_pending())
	    private_->orb_->perform_work();
	}
      return 0;
    }

  } // end of namespace corbaServer.
} // end of namespace graphics.
