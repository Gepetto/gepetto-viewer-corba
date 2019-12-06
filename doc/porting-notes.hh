/// \page gepetto_viewer_corba_porting_notes Porting notes
///
/// \section gepetto_viewer_corba_porting_5_to_6 Porting your code from version 5 to version 6
/// \subsection gepetto_viewer_corba_porting_5_to_6_1 Modification in idl API
///
/// \subsubsection gepetto_viewer_corba_porting_5_to_6_1_1 Methods
/// gepetto::corbaserver::GraphicalInterface::addURDF,
/// gepetto::corbaserver::GraphicalInterface::addUrdfCollision, and
/// gepetto::corbaserver::GraphicalInterface::addUrdfObjects
///
/// Argument meshDataRootDir has been removed. This argument was not used
/// anymore.
