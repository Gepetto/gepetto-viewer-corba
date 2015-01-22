/// \mainpage
/// \section gepetto_viewer_corba_introduction
///
/// This package implements a corba server embedding gepetto-viewer library.
///
/// The idl interface of the server is defined by gepetto::corbaserver::GraphicalInterface.
///
/// To use the graphical interface via python,
/// \li start executable
///     \code gepetto-viewer-server \endcode.
/// \li in a python terminal, create a client
/// \code
/// from gepetto.corbaserver import Client
/// client = Client ()
/// # open a new window
/// client.gui.createWindow ("w")
/// # refer to the idl interface to control gepetto-viewer.
/// \endcode
