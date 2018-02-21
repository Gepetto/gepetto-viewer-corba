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


  def __init__(self, clients = defaultClients, url = None, host = None):
    """
    Initialize CORBA and create default clients.
    :param url: URL in the IOR, corbaloc, corbalocs, and corbanames formats.
                For a remote corba server, use
                url = "corbaloc:iiop:<host>:<port>/NameService".
                If None, url is initialized with param host, or alternatively with _getIIOPurl
    :param host: if not None, url is set to = "corbaloc:iiop:" + str(host) + "/NameService"
    """
    if host is not None:
        url = "corbaloc:iiop:" + str(host) + "/NameService"
    elif url is None:
        url = _getIIOPurl()
    import sys
    self.orb = CORBA.ORB_init (sys.argv, CORBA.ORB_ID)
    obj = self.orb.string_to_object (url)
    self.rootContext = obj._narrow(CosNaming.NamingContext)
    if self.rootContext is None:
      raise CorbaError ('failed to narrow the root context')

    for client in clients:
      self.makeClient (client)

    # In the python interpreter of gepetto-gui, gui.createWindow
    # crashes for an obscure reason. This hack makes it work.
    try:
      self.gui.createWindow = lambda x: mainWindow.createView(x).wid()
      # At this point, we are in the python interpreter of gepetto-gui
    except:
      # At this point, we are NOT in the python interpreter of gepetto-gui
      pass

def _getIIOPurl ():
  """
  Returns "corbaloc:iiop:<host>:<port>/NameService"
  where host and port are, in this order of priority:
  - GEPETTO_VIEWER_HOST, GEPETTO_VIEWER_PORT environment variables
  - /gepetto_viewer/host, /gepetto_viewer/port ROS parameters
  - use default values ("localhost", 2809)
  """
  host = "localhost"
  port = 2809
  import os
  try:
    import rospy
    # Check is ROS master is reachable.
    if rospy.client.get_master().target is not None:
      host = rospy.get_param("/gepetto_viewer/host", host)
      port = rospy.get_param("/gepetto_viewer/port", port)
  except:
    pass
  host = os.getenv ("GEPETTO_VIEWER_HOST", host)
  port = os.getenv ("GEPETTO_VIEWER_PORT", port)
  if host is None and port is None:
      url = "corbaloc:iiop:/NameService"
  else:
      url = "corbaloc:iiop:{}:{}/NameService".format(host, port)
  return url
