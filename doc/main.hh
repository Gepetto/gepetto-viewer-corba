/**

\mainpage CORBA interface of Gepetto Viewer

This package implements a CORBA interface to \ref gepetto_viewer_index_html
"gepetto-viewer". Refers to its doc for further details on the interface itself.

\par Basic usage

The IDL interface of the viewer is defined by
gepetto::corbaserver::GraphicalInterface.

To create a client to the interface, you may use the following code.
\code{py}
from gepetto.corbaserver import gui_client
# Create a client to the GUI, a window name "w" and a group node "w".
# dont_raise if the GUI is not available (in which case gui will be None).
gui = gui_client(windowName = "w", dont_raise=True)
# refer to the idl interface to control gepetto-viewer.
\endcode

Extra features can be found in gepetto.corbaserver.tools.

*/
