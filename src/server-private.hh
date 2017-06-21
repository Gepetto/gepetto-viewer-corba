// Copyright (C) 2014 Mathieu Geisert, LAAS-CNRS.
//
// This file is part of the SceneViewer-corba.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#ifndef SCENEVIEWER_CORBASERVER_SERVER_PRIVATE_HH
# define SCENEVIEWER_CORBASERVER_SERVER_PRIVATE_HH

# include "graphical-interface.impl.hh"
# include "gepetto/viewer/corba/fwd.hh"

namespace graphics
{
  namespace corbaServer
  {
    namespace impl
    {
      class Server
      {
      public:
	~Server ();

	/// \brief Create and activate the Corba servers.
	void createAndActivateServers (graphics::corbaServer::Server* server);

      private:
	CORBA::ORB_var orb_;
	PortableServer::POA_var poa_;
	GraphicalInterface* graphicalInterfaceServant_;

	/// \brief It seems that we need to store this object to
	/// deactivate the server.
	PortableServer::ObjectId* graphicalInterfaceServantid_;

	/// \brief Corba context.
	CosNaming::NamingContext_var Context_;

	/// \brief Create context.
    void createContext (std::string nb_server);

	/// \brief Store objects in Corba name service.
	void bindObjectToName
	(CORBA::Object_ptr objref, CosNaming::Name objectName);


	/// \brief Deactivate and destroy servers
	///
	/// Destroying active servers raises a Corba exception.
	void deactivateAndDestroyServers ();

	friend class corbaServer::Server;
      };

    } // end of namespace impl.
  } // end of namespace corbaServer.
} // end of namespace graphics.

#endif // SCENEVIEWER_CORBASERVER_SERVER_PRIVATE_HH
