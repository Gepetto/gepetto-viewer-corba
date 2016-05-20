/// \page gepetto_viewer_corba_introduction Gepetto Viewer server
///
/// This package implements a corba server embedding gepetto-viewer library.
///
/// The idl interface of the server is defined by gepetto::corbaserver::GraphicalInterface.
///
/// To use the graphical interface via python,
/// \li start executable
///     \code{bash} gepetto-viewer-server \endcode.
/// \li in a python terminal, create a client
/// \code{py}
/// from gepetto.corbaserver import Client
/// client = Client ()
/// # open a new window
/// client.gui.createWindow ("w")
/// # refer to the idl interface to control gepetto-viewer.
/// \endcode
