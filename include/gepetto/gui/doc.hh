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
/// \section gui_extending_interface Extending the interface
///
/// There are two ways of extending the interface: Python and C++. Python
/// plugins are easier to write than C++ plugins, but they do not offer
/// full control of the interface (yet).
///
/// \subsection python_plugins Python plugins
///
/// See the documentation of gepetto::gui::PythonWidget to know how to write
/// plugins in Python. You may have a look at the file
/// \ref pyplugins/gepetto/gui/pythonwidget.py which is an example.
///
/// The Python console is a good tool for debugging. The following variables
/// are accessible:
/// \li \c mainWindow: gepetto::gui::MainWindow
/// \li \c osg: the widget responsible for the first created window (class gepetto::gui::OSGWidget)
///
/// \subsection cpp_plugins C++ Plugins
///
/// See documentation of gepetto::gui::PluginInterface,  for details on how to write a plugin.
