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

  def __init__(self, url = None, host = None, port = None):
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
            self.gui = _initWithDirectLink (self.orb, url)
        except CorbaError:
            pass
        if self.gui is None:
            self.gui = _initWithNameService (self.orb, url)
    else:
        urlNameService = _getIIOPurl(service="NameService", host=host,
                port = port if port else 2809)
        urlGepettoGui = _getIIOPurl(service="gepetto-gui", host=host,
                port = port if port else 12321)
        try:
            self.gui = _initWithDirectLink (self.orb, urlGepettoGui)
        except CorbaError as e:
            print(e)
            pass
        if self.gui is None:
            self.gui = _initWithNameService (self.orb, urlNameService)

    # In the python interpreter of gepetto-gui, gui.createWindow
    # crashes for an obscure reason. This hack makes it work.
    try:
      from PythonQt.gepetto import MainWindow
      # At this point, we are in the python interpreter of gepetto-gui
      self.gui.createWindow = lambda x: MainWindow.instance().createView(x).windowID()
    except ImportError:
      # At this point, we are NOT in the python interpreter of gepetto-gui
      pass

class _GhostGraphicalInterface:
    def __init__ (self):
        def nofunc(*args):
            """Connection to the GUI failed. Docstring not available."""
            pass

        from .gepetto.corbaserver import GraphicalInterface
        for d_func in filter(lambda x:x.startswith("_d_"), GraphicalInterface.__dict__.keys()):
            self.__dict__[d_func[3:]] = nofunc

def gui_client(window_name = None, dont_raise = False, ghost=False, url = None, host = None, port = None):
  """
  Initialize CORBA and create default clients.
  :param window_name: If provided, creates a window with this name if it does not exist.
  :param dont_raise: If True, will not raise if connection failed. It returns None instead.
  :param ghost: If True and dont_raise is True and the connection failed, an object with
                the same API as the client is returned. This is most useful when you don't rely
                on values returned by the GUI.
  :param url: URL in the IOR, corbaloc, corbalocs, and corbanames formats.
              For a remote corba server, use
              url = "corbaloc:iiop:<host>:<port>/NameService".
              If None, url is initialized with param host, or alternatively with _getIIOPurl
  :param host: if not None, url is set to = "corbaloc:iiop:" + str(host) + "/NameService"

  :return: a client to the GUI.
  """
  try:
    gui = Client(url, host, port).gui
  except Exception as e:
    if not dont_raise:
      raise e
    else:
      print("Failed to connect to the viewer.")
      print("Check whether gepetto-gui is properly started.")
      if ghost:
          return _GhostGraphicalInterface()
    return
  if window_name is not None:
    gui.createWindow(window_name)
  return gui

def _initWithNameService (orb, urlNameService):
  import CosNaming
  from .gepetto.corbaserver import GraphicalInterface

  obj = orb.string_to_object (urlNameService)
  rootContext = obj._narrow(CosNaming.NamingContext)
  if rootContext is None:
    raise CorbaError ('Failed to narrow the root context')

  name = [CosNaming.NameComponent ("gepetto", "viewer"),
          CosNaming.NameComponent ("corbaserver", "gui")]

  try:
    obj = rootContext.resolve (name)
  except CosNaming.NamingContext.NotFound:
    raise CorbaError ('Failed to find the service "gui"')

  try:
    client = obj._narrow (GraphicalInterface)
  except KeyError:
    raise CorbaError ('Invalid service name "gui"')

  if client is None:
    # This happens when stubs from client and server are not synchronized.
    raise CorbaError ( 'Failed to narrow client for service named "gui"')

  return client

def _initWithDirectLink (orb, url):
  from .gepetto.corbaserver import GraphicalInterface
  obj = orb.string_to_object (url)
  client = obj._narrow(GraphicalInterface)

  if client is None:
    # This happens when stubs from client and server are not synchronized.
    raise CorbaError ( 'Failed to narrow client for service named "gui"')

  return client

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
