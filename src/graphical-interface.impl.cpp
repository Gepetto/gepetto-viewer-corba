//
//  GraphicalInterface.impl.cc
//  Corbaserver for gepetto-viewer
//
//  Author: Mathieu Geisert, Florent Lamiraux
//  Copyright (c) 2014 LAAS-CNRS. All rights reserved.
//

#include <iostream>
#include "graphical-interface.impl.hh"

namespace graphics {
  namespace corbaServer {
    namespace impl {

      GraphicalInterface::GraphicalInterface (corbaServer::Server* server) :
	windowManagers_ (), nodes_ (), groupNodes_ (), server_ (server),
	mtx_ (), rate_ (20), newNodeConfigurations_ ()
      {
      }

      osgVector4 GraphicalInterface::getColor (const std::string& colorName)
      {
	if (colorName == "blue")
	  return osgVector4 (0.f, 0.f, 1.f, 1.f);
	else if (colorName == "green")
	  return osgVector4 (0.f, 1.f, 0.f, 1.f);
	else if (colorName == "red")
	  return osgVector4 (1.f, 0.f, 0.f, 1.f);
	else if (colorName == "white")
	  return osgVector4 (1.f, 1.f, 1.f, 1.f);
	else
	  return osgVector4 (0.f, 0.f, 0.f, 1.f);
      }

      osgVector4 GraphicalInterface::getColor (const double* colorCorba)
      {
	return osgVector4 (colorCorba[0], colorCorba[1], colorCorba[2],
			  colorCorba[3]);
      }

      VisibilityMode GraphicalInterface::getVisibility
 (const std::string& visibilityName)
      {
	if (visibilityName == "OFF")
	  return VISIBILITY_OFF;
	else if (visibilityName == "ALWAYS_ON_TOP")
	  return ALWAYS_ON_TOP;
	else if (visibilityName == "ON")
	  return VISIBILITY_ON;
	else {
	  std::cout << "Visibility mode not known, visibility mode can be"
	    " \"ON\",\"OFF\" or \"ALWAYS_ON_TOP\"." << std::endl;
	  std::cout << "Visibility mode set to default = \"ON\"." << std::endl;
	  return VISIBILITY_ON;
	}
      }

      WireFrameMode GraphicalInterface::getWire (const std::string& wireName)
      {
	if (wireName == "WIREFRAME")
	  return WIREFRAME;
	else if (wireName == "FILL_AND_WIREFRAME")
	  return FILL_AND_WIREFRAME;
	else if (wireName == "FILL")
	  return FILL;
	else {
	  std::cout << "Wire mode not known, wire mode can be \"FILL\","
	    "\"WIREFRAME\" or \"FILL_AND_WIREFRAME\"." << std::endl;
	  std::cout << "Wire mode set to default = \"FILL\"." << std::endl;
	  return FILL;
	}
      }

      LightingMode GraphicalInterface::getLight (const std::string& lightName)
      {
	if (lightName == "OFF")
	  return LIGHT_INFLUENCE_OFF;
	else if (lightName == "ON")
	  return LIGHT_INFLUENCE_ON;
	else {
	  std::cout << "Lighting mode not known, lighting mode can be "
	    "\"ON\" or \"OFF\"." << std::endl;
	  std::cout << "Lighting mode set to default = \"ON\"." << std::endl;
	  return LIGHT_INFLUENCE_ON;
	}
      }

      std::string GraphicalInterface::parentName (const std::string& name)
      {
	std::string Name (name);
	std::string::iterator parentNameIt;
	for (std::string::iterator i = Name.end (); (*i) != char ('/') &&
	       i != Name.begin (); i--) {
	  parentNameIt = i;
	}
	parentNameIt--;

	std::string parentName;
	for (std::string::iterator i = Name.begin (); i != parentNameIt; i++) {
	  parentName.push_back (*i);
	}

	return parentName;
      }

      void GraphicalInterface::initParent (const std::string& nodeName,
					  NodePtr_t node)
      {
	GroupNodePtr_t groupNode = groupNodes_
	  [GraphicalInterface::parentName (nodeName)];
	if (groupNode) {
	  groupNode->addChild (node);
	}
      }

      void GraphicalInterface::addNode (const std::string& nodeName,
				       NodePtr_t node)
      {
	nodes_[nodeName] = node;
      }

      void GraphicalInterface::addGroup (const std::string& groupName,
					 GroupNodePtr_t group)
      {
	groupNodes_[groupName] = group;
	nodes_[groupName] = group;
      }

      void GraphicalInterface::threadRefreshing (WindowManagerPtr_t window)
      {
	while (!window->done ())
	  {
	    mtx_.lock ();
	    window->frame ();
	    mtx_.unlock ();
	    boost::this_thread::sleep (boost::posix_time::milliseconds (rate_));
	  }
      }

      osgQuat GraphicalInterface::corbaConfToOsgQuat
      (const double* configurationCorba)
      {
	// configurationCorba = trans (x, y, z), quat (w, x, y, z)
	// osgQuat = quat (x, y, z, w)
	return osgQuat (configurationCorba[4], configurationCorba[5],
			configurationCorba[6], configurationCorba[3]);
      }

      osgVector3 GraphicalInterface::corbaConfToOsgVec3
      (const double* configurationCorba)
      {
	return osgVector3 (configurationCorba[0], configurationCorba[1],
			   configurationCorba[2]);
      }


      //Public functions

      bool GraphicalInterface::setRate (CORBA::Long rate) throw (Error)
      {
	try {
	  if (rate <= 0) {
	    std::cout << "You should specify a positive rate" << std::endl;
	    return false;
	  }
	  else {
	    rate_ = rate;
	    return true;
	  }
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      GraphicalInterface::WindowID GraphicalInterface::createWindow (const char* windowNameCorba)
	throw (Error)
      {
	try {
	  std::string windowName (windowNameCorba);
          WindowManagerPtr_t newWindow = WindowManager::create ();
          WindowID windowId = windowManagers_.size ();
          windowManagers_.push_back (newWindow);
          boost::thread refreshThread (boost::bind
              (&GraphicalInterface::threadRefreshing,
               this, newWindow));
          return windowId;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      void GraphicalInterface::refresh () throw (Error)
      {
	try {
	  mtx_.lock ();
	  //refresh scene with the new configuration
	  for (std::list<NodeConfiguration>::iterator it =
		 newNodeConfigurations_.begin ();
	       it != newNodeConfigurations_.end (); it++) {
	 (*it).node->applyConfiguration ( (*it).position, (*it).quat);
	  }
	  mtx_.unlock ();
	  newNodeConfigurations_.clear ();
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      void GraphicalInterface::createScene (const char* sceneNameCorba)
	throw (Error)
      {
	try {
	  std::string sceneName (sceneNameCorba);
	  if (nodes_.find (sceneName) != nodes_.end ()) {
	    std::ostringstream oss;
	    oss << "A scene with name, \"" << sceneName << "\" already exists.";
	      throw std::runtime_error (oss.str ());
	  }
	  else {
	    GroupNodePtr_t mainNode = GroupNode::create (sceneName);
	    addGroup (sceneName, mainNode);
	  }
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      void GraphicalInterface::createSceneWithFloor
      (const char* sceneNameCorba) throw (Error)

      {
	try {
	  std::string sceneName (sceneNameCorba);
	  if (nodes_.find (sceneName) != nodes_.end ()) {
	    std::ostringstream oss;
	    oss << "A scene with name, \"" << sceneName << "\" already exists.";
	      throw std::runtime_error (oss.str ());
	  }
	  else {
	    GroupNodePtr_t mainNode = GroupNode::create (sceneName);
	    addGroup (sceneName, mainNode);
	    std::string floorName = sceneName + "/floor";
	    LeafNodeGroundPtr_t floor = LeafNodeGround::create (floorName);
	    addNode (floorName, floor);
	    mainNode->addChild (floor);
	  }
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::addSceneToWindow (const char* sceneNameCorba,
						 WindowID windowId)
	throw (Error)
      {
	try {
	  std::string sceneName (sceneNameCorba);
	  if ((windowId >= 0 || windowId < windowManagers_.size ()) &&
	      groupNodes_.find (sceneName) != groupNodes_.end () ) {
	    windowManagers_[windowId]->addNode (groupNodes_[sceneName]);
	    return true;
	  }
	  else {
	    std::cout << "Window ID \"" << windowId
		      << "\" and/or scene name \"" << sceneName
		      << "\" doesn't exist." << std::endl;
	    return false;
	  }
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::addBox (const char* boxNameCorba,
				       const float boxSize1,
				       const float boxSize2,
				       const float boxSize3,
				       const double* colorCorba) throw (Error)
      {
	try {
	  std::string boxName (boxNameCorba);
	  if (nodes_.find (boxName) != nodes_.end ()) {
	    std::cout << "You need to chose an other name, \"" << boxName
		      << "\" already exist." << std::endl;
	    return false;
	  }
	  else {
	    LeafNodeBoxPtr_t box = LeafNodeBox::create
	      (boxName, osgVector3 (boxSize1, boxSize2, boxSize3),
	       getColor (colorCorba));
	    GraphicalInterface::initParent (boxName, box);
	    addNode (boxName, box);
	    return true;
	  }
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::addCapsule (const char* capsuleNameCorba,
					   const float radius,
					   const float height,
					   const double* colorCorba)
	throw (Error)
      {
	try {
	  const std::string capsuleName (capsuleNameCorba);
	  if (nodes_.find (capsuleName) != nodes_.end ()) {
	    std::cout << "You need to chose an other name, \""
		      << capsuleName << "\" already exist." << std::endl;
	    return false;
	  }
	  else {
	    LeafNodeCapsulePtr_t capsule = LeafNodeCapsule::create
	      (capsuleName, radius, height, getColor (colorCorba));
	    GraphicalInterface::initParent (capsuleName, capsule);
	    addNode (capsuleName, capsule);
	    return true;
	  }
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::addMesh (const char* meshNameCorba,
					const char* meshPathCorba) throw (Error)
      {
	try {
	  std::string meshName (meshNameCorba);
	  std::string meshPath (meshPathCorba);
	  if (nodes_.find (meshName) != nodes_.end ()) {
	    std::cout << "You need to chose an other name, \"" << meshName
		      << "\" already exist." << std::endl;
	    return false;
	  }
	  else {
	    try {
	      LeafNodeColladaPtr_t mesh = LeafNodeCollada::create
		(meshName, meshPath);
	      GraphicalInterface::initParent (meshName, mesh);
	      addNode (meshName, mesh);
	      return true;
	    } catch (const std::exception& exc) {
	      std::cout << "Mesh \"" << meshPath << "\" not found." << std::endl;
	      return false;
	    }
	  }
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::addCone (const char* coneNameCorba,
					const float radius, const float height,
					const double*) throw (Error)
      {
	try {
	  std::string coneName (coneNameCorba);
	  if (nodes_.find (coneName) != nodes_.end ()) {
	    std::cout << "You need to chose an other name, \"" << coneName
		      << "\" already exist." << std::endl;
	    return false;
	  }
	  else {
	    LeafNodeConePtr_t cone = LeafNodeCone::create
	      (coneName, radius, height);
	    GraphicalInterface::initParent (coneName, cone);
	    addNode (coneName, cone);
	    return true;
	  }
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::addCylinder (const char* cylinderNameCorba,
					    const float radius,
					    const float height,
					    const double* colorCorba)
	throw (Error)
      {
	try {
	  std::string cylinderName (cylinderNameCorba);
	  if (nodes_.find (cylinderName) != nodes_.end ()) {
	    std::cout << "You need to chose an other name, \"" << cylinderName
		      << "\" already exist." << std::endl;
	    return false;
	  }
	  else {
	    LeafNodeCylinderPtr_t cylinder = LeafNodeCylinder::create
	      (cylinderName, radius, height, getColor (colorCorba));
	    GraphicalInterface::initParent (cylinderName, cylinder);
	    addNode (cylinderName, cylinder);
	    return true;
	  }
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::addSphere (const char* sphereNameCorba,
					  const float radius,
					  const double* colorCorba)
	throw (Error)
      {
	try {
	  std::string sphereName (sphereNameCorba);
	  if (nodes_.find (sphereName) != nodes_.end ()) {
	    std::cout << "You need to chose an other name, \"" << sphereName
		      << "\" already exist." << std::endl;
	    return false;
	  }
	  else {
	    LeafNodeSpherePtr_t sphere = LeafNodeSphere::create
	      (sphereName, radius, getColor (colorCorba));
	    GraphicalInterface::initParent (sphereName, sphere);
	    addNode (sphereName, sphere);
	    return true;
	  }
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::addLine (const char* lineNameCorba,
					const double* posCorba1,
					const double* posCorba2,
					const double* colorCorba) throw (Error)
      {
	try {
	  std::string lineName (lineNameCorba);
	  if (nodes_.find (lineName) != nodes_.end ()) {
	    std::cout << "You need to chose an other name, \"" << lineName
		      << "\" already exist." << std::endl;
	    return false;
	  }
	  else {
	    osgVector3 pos1 (posCorba1[0], posCorba1[1], posCorba1[2]);
	    osgVector3 pos2 (posCorba2[0], posCorba2[1], posCorba2[2]);
	    LeafNodeLinePtr_t line = LeafNodeLine::create
	      (lineName, pos1, pos2, getColor (colorCorba));
	    GraphicalInterface::initParent (lineName, line);
	    addNode (lineName, line);
	    return true;
	  }
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::addTriangleFace (const char* faceNameCorba,
						const double* posCorba1,
						const double* posCorba2,
						const double* posCorba3,
						const double* colorCorba)
	throw (Error)
      {
	try {
	  std::string faceName (faceNameCorba);
	  if (nodes_.find (faceName) != nodes_.end ()) {
	    std::cout << "You need to chose an other name, \"" << faceName
		      << "\" already exist." << std::endl;
	    return false;
	  }
	  else {
	    osgVector3 pos1 (posCorba1[0], posCorba1[1], posCorba1[2]);
	    osgVector3 pos2 (posCorba2[0], posCorba2[1], posCorba2[2]);
	    osgVector3 pos3 (posCorba3[0], posCorba3[1], posCorba3[2]);
	    LeafNodeFacePtr_t face = LeafNodeFace::create
	      (faceName, pos1, pos2, pos3, getColor (colorCorba));
	    GraphicalInterface::initParent (faceName, face);
	    addNode (faceName, face);
	    return true;
	  }
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::addSquareFace (const char* faceNameCorba,
					      const double* posCorba1,
					      const double* posCorba2,
					      const double* posCorba3,
					      const double* posCorba4,
					      const double* colorCorba)
	throw (Error)
      {
	try {
	  std::string faceName (faceNameCorba);
	  if (nodes_.find (faceName) != nodes_.end ()) {
	    std::cout << "You need to chose an other name, \"" << faceName
		      << "\" already exist." << std::endl;
	    return false;
	  }
	  else {
	    osgVector3 pos1 (posCorba1[0], posCorba1[1], posCorba1[2]);
	    osgVector3 pos2 (posCorba2[0], posCorba2[1], posCorba2[2]);
	    osgVector3 pos3 (posCorba3[0], posCorba3[1], posCorba3[2]);
	    osgVector3 pos4 (posCorba4[0], posCorba4[1], posCorba4[2]);
	    LeafNodeFacePtr_t face = LeafNodeFace::create
	      (faceName, pos1, pos2, pos3, pos3, getColor (colorCorba));
	    GraphicalInterface::initParent (faceName, face);
	    addNode (faceName, face);
	    return true;
	  }
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      void GraphicalInterface::getNodeList () throw (Error)
      {
	try {
	  std::cout << "List of Nodes :" << std::endl;
	  for (std::map<std::string, NodePtr_t>::iterator it=nodes_.begin ();
	       it!=nodes_.end (); ++it)
	    std::cout << "   " << it->first << std::endl;
	  std::cout << "List of GroupNodes :" << std::endl;
	  for (std::map<std::string, GroupNodePtr_t>::iterator it=
		 groupNodes_.begin (); it!=groupNodes_.end (); ++it)
	    std::cout << "   " << it->first << std::endl;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      void GraphicalInterface::getWindowList () throw (Error)
      {
	try {
	  std::cout << "List of Windows :" << std::endl;
          size_t rank = 0;
	  for (WindowManagers_t::iterator it = windowManagers_.begin ();
              it!=windowManagers_.end (); ++it)
	    std::cout << rank << " - " << (*it)->getViewerClone ()->getSlave (0)._camera->getGraphicsContext ()->getTraits ()->windowName << std::endl;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::createGroup (const char* groupNameCorba) throw (Error)
      {
	try {
	  const std::string groupName (groupNameCorba);
	  if (nodes_.find (groupName) != nodes_.end ()) {
	    std::cout << "You need to chose an other name, \"" << groupName
		      << "\" already exist." << std::endl;
	    return false;
	  }
	  else {
	    GroupNodePtr_t groupNode = GroupNode::create (groupName);
	    GraphicalInterface::initParent (groupName, groupNode);
	    addGroup (groupName, groupNode);
	    return true;
	  }
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::addURDF (const char* urdfNameCorba,
					const char* urdfPathCorba,
					const char* urdfPackagePathCorba)
	throw (Error)
      {
	try {
	  const std::string urdfName (urdfNameCorba);
	  const std::string urdfPath (urdfPathCorba);
	  const std::string urdfPackagePath (urdfPackagePathCorba);
	  if (nodes_.find (urdfName) != nodes_.end ()) {
	    std::cout << "You need to chose an other name, \"" << urdfName
		      << "\" already exist." << std::endl;
	    return false;
	  }
	  else {
	    GroupNodePtr_t urdf = urdfParser::parse
	 (urdfName, urdfPath, urdfPackagePath);
	    NodePtr_t link;
	    for (int i=0; i< urdf->getNumOfChildren (); i++) {
	      link = urdf->getChild (i);
	      nodes_[link->getID ()] = link;
	    }
	    GraphicalInterface::initParent (urdfName, urdf);
	    addGroup (urdfName, urdf);
	    return true;
	  }
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::addUrdfCollision
      (const char* urdfNameCorba, const char* urdfPathCorba,
       const char* urdfPackagePathCorba) throw (Error)
      {
	try {
	  const std::string urdfName (urdfNameCorba);
	  const std::string urdfPath (urdfPathCorba);
	  const std::string urdfPackagePath (urdfPackagePathCorba);
	  if (nodes_.find (urdfName) != nodes_.end ()) {
	    std::cout << "You need to chose an other name, \"" << urdfName
		      << "\" already exist." << std::endl;
	    return false;
	  }
	  else {
	    GroupNodePtr_t urdf = urdfParser::parse
	      (urdfName, urdfPath, urdfPackagePath, "collision");
	    NodePtr_t link;
	    for (int i=0; i< urdf->getNumOfChildren (); i++) {
	      link = urdf->getChild (i);
	      nodes_[link->getID ()] = link;
	    }
	    GraphicalInterface::initParent (urdfName, urdf);
	    addGroup (urdfName, urdf);
	    return true;
	  }
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      void GraphicalInterface::addUrdfObjects (const char* urdfNameCorba,
					       const char* urdfPathCorba,
					       const char* urdfPackagePathCorba,
					       bool visual)
	throw (Error)
      {
	try {
	  const std::string urdfName (urdfNameCorba);
	  const std::string urdfPath (urdfPathCorba);
	  const std::string urdfPackagePath (urdfPackagePathCorba);
	  if (urdfName == "") {
	    throw std::runtime_error ("Parameter nodeName cannot be empty in "
				      "idl request addUrdfObjects.");
	  }
	  if (nodes_.find (urdfName) != nodes_.end ()) {
	    std::ostringstream oss;
	    oss << "You need to chose an other name, \"" << urdfName
		<< "\" already exist.";
	    throw std::runtime_error (oss.str ());
	  }
	  GroupNodePtr_t urdf = urdfParser::parse
	    (urdfName, urdfPath, urdfPackagePath,
	     visual ? "visual" : "collision", "object");
	  NodePtr_t link;
	  for (int i=0; i< urdf->getNumOfChildren (); i++) {
	    link = urdf->getChild (i);
	    nodes_[link->getID ()] = link;
	  }
	  GraphicalInterface::initParent (urdfName, urdf);
	  addGroup (urdfName, urdf);
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::addToGroup (const char* nodeNameCorba,
					   const char* groupNameCorba)
	throw (Error)
      {
	try {
	  const std::string nodeName (nodeNameCorba);
	  const std::string groupName (groupNameCorba);
	  if (nodes_.find (nodeName) == nodes_.end () ||
	      groupNodes_.find (groupName) == groupNodes_.end ()) {
	    std::cout << "Node name \"" << nodeName << "\" and/or groupNode \""
		      << groupName << "\" doesn't exist." << std::endl;
	    return false;
	  }
	  else {
	    groupNodes_[groupName]->addChild (nodes_[nodeName]);
	    return true;
	  }
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::applyConfiguration
      (const char* nodeNameCorba, const double* configurationCorba)
	throw (Error)
      {
	try {
	  const std::string nodeName (nodeNameCorba);
	  if (nodes_.find (nodeName) == nodes_.end ()) {
	    //no node named nodeName
	    std::cout << "No Node named \"" << nodeName << "\"" << std::endl;
	    return false;
	  }
	  else {
	    NodeConfiguration newNodeConfiguration;
	    newNodeConfiguration.node = nodes_[nodeName];
	    try {
	      newNodeConfiguration.position =
		GraphicalInterface::corbaConfToOsgVec3 (configurationCorba);
	      newNodeConfiguration.quat =
		GraphicalInterface::corbaConfToOsgQuat (configurationCorba);
	    } catch (const std::exception& exc) {
	      std::cout << "Error when converting configuration, configuration "
		"should be DOUBLE[7] = [transX, transY, transZ, quatW, quatX,"
		"quatY, quatZ]" <<std::endl;
	      return false;
	    }
	    newNodeConfigurations_.push_back (newNodeConfiguration);
	    return true;
	  }
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::addLandmark (const char* nodeNameCorba,
					    float size) throw (Error)
      {
	try {
	  const std::string nodeName (nodeNameCorba);
	  if (nodes_.find (nodeName) == nodes_.end ()) {
	    std::cout << "Node \"" << nodeName << "\" doesn't exist."
		      << std::endl;
	    return false;
	  }
	  nodes_[nodeName]->addLandmark (size);
	  return true;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }


      bool GraphicalInterface::deleteLandmark (const char* nodeNameCorba)
	throw (Error)
      {
	try {
	  const std::string nodeName (nodeNameCorba);
	  if (nodes_.find (nodeName) == nodes_.end ()) {
	    std::cout << "Node \"" << nodeName << "\" doesn't exist."
		      << std::endl;
	    return false;
	  }
	  nodes_[nodeName]->deleteLandmark ();
	  return true;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::setVisibility (const char* nodeNameCorba,
					      const char* visibilityModeCorba)
	throw (Error)
      {
	try {
	  const std::string nodeName (nodeNameCorba);
	  const std::string visibilityMode (visibilityModeCorba);
	  VisibilityMode visibility =  getVisibility (visibilityMode);
	  if (nodes_.find (nodeName) == nodes_.end ()) {
	    std::cout << "Node \"" << nodeName << "\" doesn't exist."
		      << std::endl;
	    return false;
	  }
	  nodes_[nodeName]->setVisibilityMode (visibility);
	  return true;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::setWireFrameMode (const char* nodeNameCorba,
						 const char* wireFrameModeCorba)
	throw (Error)
      {
	try {
	  const std::string nodeName (nodeNameCorba);
	  const std::string wireFrameMode (wireFrameModeCorba);
	  WireFrameMode wire = getWire (wireFrameMode);
	  if (nodes_.find (nodeName) == nodes_.end ()) {
	    std::cout << "Node \"" << nodeName << "\" doesn't exist."
		      << std::endl;
	    return false;
	  }
	  nodes_[nodeName]->setWireFrameMode (wire);
	  return true;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::setLightingMode (const char* nodeNameCorba,
						const char* lightingModeCorba)
	throw (Error)
      {
	try {
	  const std::string nodeName (nodeNameCorba);
	  const std::string lightingMode (lightingModeCorba);
	  LightingMode light = getLight (lightingMode);
	  if (nodes_.find (nodeName) == nodes_.end ()) {
	    std::cout << "Node \"" << nodeName << "\" doesn't exist."
		      << std::endl;
	    return false;
	  }
	  nodes_[nodeName]->setLightingMode (light);
	  return true;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }
    } //end namespace impl
  } //end namespace corbaServer
} //end namespace graphics
