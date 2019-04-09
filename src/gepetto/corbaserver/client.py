"""
Provide a client for CORBA services which initialize CORBA automatically and
create client to wanted HPP services.
"""

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

  def initWithNameService (self, urlNameService):
    import CosNaming
    from .gepetto.corbaserver import GraphicalInterface

    obj = self.orb.string_to_object (urlNameService)
    self.rootContext = obj._narrow(CosNaming.NamingContext)
    if self.rootContext is None:
      raise CorbaError ('Failed to narrow the root context')

    name = [CosNaming.NameComponent ("gepetto", "viewer"),
            CosNaming.NameComponent ("corbaserver", "gui")]

    try:
      obj = self.rootContext.resolve (name)
    except CosNaming.NamingContext.NotFound:
      raise CorbaError ('Failed to find the service "gui"')

    try:
      client = obj._narrow (GraphicalInterface)
    except KeyError:
      raise CorbaError ('Invalid service name "gui"')

    if client is None:
      # This happens when stubs from client and server are not synchronized.
      raise CorbaError ( 'Failed to narrow client for service named "gui"')

    self.gui = client

  def initWithDirectLink (self, url):
    from .gepetto.corbaserver import GraphicalInterface
    obj = self.orb.string_to_object (url)
    client = obj._narrow(GraphicalInterface)

    if client is None:
      # This happens when stubs from client and server are not synchronized.
      raise CorbaError ( 'Failed to narrow client for service named "gui"')

    self.gui = client

  def __init__(self, clients = defaultClients, url = None, host = None, port = None):
    """
    Initialize CORBA and create default clients.
    :param url: URL in the IOR, corbaloc, corbalocs, and corbanames formats.
                For a remote corba server, use
                url = "corbaloc:iiop:<host>:<port>/NameService".
                If None, url is initialized with param host, or alternatively with _getIIOPurl
    :param host: if not None, url is set to = "corbaloc:iiop:" + str(host) + "/NameService"
    """
    from omniORB import CORBA
    import sys
    self.orb = CORBA.ORB_init (sys.argv, CORBA.ORB_ID)

    if url is not None:
        try:
            self.initWithDirectLink (url)
        except CorbaError:
            pass
        if self.gui is None:
            self.initWithNameService (url)
    else:
        urlNameService = _getIIOPurl(service="NameService", host=host,
                port = port if port else 2809)
        urlGepettoGui = _getIIOPurl(service="gepetto-gui", host=host,
                port = port if port else 12321)
        try:
            self.initWithDirectLink (urlGepettoGui)
        except CorbaError as e:
            print(e)
            pass
        if self.gui is None:
            self.initWithNameService (urlNameService)

    # In the python interpreter of gepetto-gui, gui.createWindow
    # crashes for an obscure reason. This hack makes it work.
    try:
      from PythonQt.gepetto import MainWindow
      # At this point, we are in the python interpreter of gepetto-gui
      self.gui.createWindow = lambda x: MainWindow.instance().createView(x).windowID()
    except ImportError:
      # At this point, we are NOT in the python interpreter of gepetto-gui
      pass

def _getIIOPurl (service="NameService", host=None, port=None):
  """
  Returns "corbaloc:iiop:<host>:<port>/NameService"
  where host and port are, in this order of priority:
  - GEPETTO_VIEWER_HOST, GEPETTO_VIEWER_PORT environment variables
  - /gepetto_viewer/host, /gepetto_viewer/port ROS parameters
  - use default values ("localhost", 2809)
  """
  _host = "localhost"
  _port = 2809
  import os
  try:
    import rospy
    # Check is ROS master is reachable.
    if rospy.client.get_master().target is not None:
      _host = rospy.get_param("/gepetto_viewer/host", _host)
      _port = rospy.get_param("/gepetto_viewer/port", _port)
  except:
    pass
  _host = os.getenv ("GEPETTO_VIEWER_HOST", _host)
  _port = os.getenv ("GEPETTO_VIEWER_PORT", _port)
  if host: _host = host
  if port: _port = port
  if _host is None and _port is None:
      url = "corbaloc:iiop:"
  else:
      url = "corbaloc:iiop:{}:{}".format(_host, _port)
  return url + "/" + service
