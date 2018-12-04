/// \page gepetto_gui_introduction Gepetto Gui introduction
///
/// Graphical user interface for the gepetto-viewer package.
///
/// \section gui_using_interface Using the interface
///
/// To start the interface, use
/// \code{bash} gepetto-gui \endcode
///
/// To get some help on the command line options, use
/// \code{bash} gepetto-gui --help \endcode
///
/// \section gui_settings Configuration files
///
/// See gepetto::gui::Settings::readSettingFile for more details on configuration files.
///
/// \section gui_extending_interface Extending the interface
///
/// There are two ways of extending the interface: Python and C++. Python
/// plugins are easier to write than C++ plugins, but they do not offer
/// full control of the interface (yet).
///
/// \subsection python_plugins Python plugins
///
/// See the documentation of gepetto::gui::PythonWidget for instructions
/// and examples of Python plugin usage.
///
/// The Python console is a good tool for debugging. The following variables
/// are accessible:
/// \li \c mainWindow: gepetto::gui::MainWindow
/// \li \c osg: the widget responsible for the first created window (class gepetto::gui::OSGWidget)
///
/// \subsection cpp_plugins C++ Plugins
///
/// See documentation of gepetto::gui::PluginInterface,  for details on how to write a plugin.

/// \page gepetto_viewer_corba_introduction Gepetto Viewer server
///
/// This package implements a corba server embedding gepetto-viewer library.
///
/// The idl interface of the server is defined by gepetto::corbaserver::GraphicalInterface.
///
/// To use the graphical interface via python,
/// \li start executable
///     \code{bash} gepetto-gui \endcode.
/// \li in a python terminal, create a client
/// \code{py}
/// from gepetto.corbaserver import Client
/// client = Client ()
/// # open a new window
/// client.gui.createWindow ("w")
/// # refer to the idl interface to control gepetto-viewer.
/// \endcode
