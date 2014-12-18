"""
Provide a client for CORBA services which initialize CORBA automatically and
create client to wanted HPP services.
"""
from omniORB import CORBA
import CosNaming

import gepetto.corbaserver

class CorbaError(Exception):
  """
  Raised when a CORBA error occurs.
  """
  def __init__(self, value):
    self.value = value
  def __str__(self):
    return repr(self.value)

class Client:
  """
  Connect and create clients for all HPP services.
  """

  defaultClients = [('gui', 'GraphicalInterface')]

  def makeClient(self, serviceName):
    """
    Create a client to a new CORBA service and add it to this class.
    """
    name = [CosNaming.NameComponent ("gepetto", "viewer"),
            CosNaming.NameComponent ("corbaserver", serviceName [0])]

    try:
      obj = self.rootContext.resolve (name)
    except CosNaming.NamingContext.NotFound, ex:
      raise CorbaError (
        'failed to find the service ``{0}\'\''.format (serviceName [0]))

    try:
      client = obj._narrow (gepetto.corbaserver.__dict__
                            [serviceName [1]])
    except KeyError:
      raise CorbaError ('invalid service name ``{0}\'\''.format \
                          (serviceName [0]))

    if client is None:
      # This happens when stubs from client and server are not synchronized.
      raise CorbaError (
        'failed to narrow client for service named ``{0}\'\''.format
        (serviceName [0]))
    self.__dict__[serviceName [0]] = client


  def __init__(self, clients = defaultClients):
    """
    Initialize CORBA and create default clients.
    """
    import sys
    self.orb = CORBA.ORB_init (sys.argv, CORBA.ORB_ID)
    obj = self.orb.resolve_initial_references("NameService")
    self.rootContext = obj._narrow(CosNaming.NamingContext)
    if self.rootContext is None:
      raise CorbaError ('failed to narrow the root context')

    for client in clients:
      self.makeClient (client)
