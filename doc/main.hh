/**

\mainpage Gepetto Viewer

This package provides a graphical user interface to visualize robots. There are
two compatible interfaces to interact with the interface.
\li the CORBA interface with Python and C++ clients.
\li the graphical user interface.

To start the interface, use
\code{bash} gepetto-gui \endcode

To get some help on the command line options, use
\code{bash} gepetto-gui --help \endcode

To get command line completion for `gepetto-gui` command, see package
https://github.com/humanoid-path-planner/hpp-tools.

\section gepetto_gui_corba CORBA interface

The IDL interface of the server is defined by gepetto::corbaserver::GraphicalInterface.

\code{py}
from gepetto.corbaserver import Client
client = Client ()
# open a new window
client.gui.createWindow ("w")
# refer to the idl interface to control gepetto-viewer.
\endcode

\section gepetto_gui_ui Graphical user interface

\par Configuration files

The interface can be customized using a few setting files.
See gepetto::gui::Settings::readSettingFile for more details on configuration files.

To generate a new configuration use:
\code{bash} gepetto-gui -c newConfigName -g \endcode

To use the configuration *config*:
\code{bash} gepetto-gui -c newConfigName \endcode

\par Extending the interface

There are two ways of extending the interface: Python and C++. Python
plugins are easier to write than C++ plugins, but they do not offer
full control of the interface (yet).

See the \ref pluginlist for some examples.
Building and installing plugins is made easy with \ref gepetto_viewer_corba_cmake_macros.

- *Python plugins*
  See the documentation of gepetto::gui::PythonWidget for instructions
  and examples of Python plugin usage.

  The Python console is a good tool for debugging. The following variables
  are accessible:
  + \c mainWindow: gepetto::gui::MainWindow
  + \c osg: the widget responsible for the first created window (class gepetto::gui::OSGWidget)

- *C++ Plugin*
  See documentation of gepetto::gui::PluginInterface,  for details on how to write a plugin.

\defgroup pluginlist List of available plugins

This is the list of available plugins. See each plugin to know how to activate it.

\defgroup plugin Plugin interfaces
\{
Interface of C++ and Python plugins.

  \defgroup plugin_cpp C++ Plugin API
  Descriptions of the available interfaces.

  \defgroup plugin_python Python plugin API
  These slots are available for Python scripting in plugins.
\}

*/
