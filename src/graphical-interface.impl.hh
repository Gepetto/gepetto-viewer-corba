//
//  GraphicalInterface.impl.hh
//  Corbaserver for SceneViewer
//
//  Created by Mathieu Geisert on November 2014.
//  Copyright (c) 2014 LAAS-CNRS. All rights reserved.
//

#ifndef SCENEVIEWER_CORBASERVER_GRAPHICALINTERFACE_HH
#define SCENEVIEWER_CORBASERVER_GRAPHICALINTERFACE_HH

#include <omniORB4/CORBA.h>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <gepetto/viewer/window-manager.h>
#include <gepetto/viewer/node.h>
#include <gepetto/viewer/group-node.h>
#include <gepetto/viewer/leaf-node-box.h>
#include <gepetto/viewer/leaf-node-capsule.h>
#include <gepetto/viewer/leaf-node-cone.h>
#include <gepetto/viewer/leaf-node-cylinder.h>
#include <gepetto/viewer/leaf-node-line.h>
#include <gepetto/viewer/leaf-node-face.h>
#include <gepetto/viewer/leaf-node-sphere.h>
#include <gepetto/viewer/macros.h>
#include <gepetto/viewer/config-osg.h>
#include <gepetto/viewer/leaf-node-ground.h>
#include <gepetto/viewer/leaf-node-collada.h>
#include <gepetto/viewer/urdf-parser.h>

#include "server.hh"
#include "graphical-interface.hh"

namespace graphics {
namespace corbaServer {
namespace impl {

struct NodeConfiguration {
    NodePtr_t node;
    osgVector3 position;
    osgQuat quat;
};

class GraphicalInterface :
    public virtual POA_gepetto::corbaserver::GraphicalInterface
{
private:
    typedef gepetto::Error Error;
    typedef CORBA::ULong WindowID;
    typedef std::vector <WindowManagerPtr_t> WindowManagers_t;
    WindowManagers_t windowManagers_;
    std::map<std::string, NodePtr_t> nodes_;
    std::map<std::string, GroupNodePtr_t> groupNodes_;
    corbaServer::Server* server_;
    boost::mutex mtx_;
    int rate_;
    std::list<NodeConfiguration> newNodeConfigurations_;

    static osgVector4 getColor(const std::string& colorName);
    static osgVector4 getColor(const double* color);
    static std::string parentName(const std::string& name);
    static VisibilityMode getVisibility(const std::string& visibilityName);
    static WireFrameMode getWire(const std::string& wireName);
    static LightingMode getLight(const std::string& lightName);
    void initParent(const std::string& nodeName, NodePtr_t node);
    void addNode(const std::string& nodeName, NodePtr_t node);
    void addGroup(const std::string& groupName, GroupNodePtr_t group);
    void threadRefreshing(WindowManagerPtr_t window);
    static osgQuat corbaConfToOsgQuat(const double* configurationCorba);
    static osgVector3 corbaConfToOsgVec3(const double* configurationCorba);

protected:

public:
    /**
        \brief Default constructor
        */
    GraphicalInterface (corbaServer::Server* server);

  virtual void getNodeList() throw (Error);
  virtual void getWindowList() throw (Error);

  virtual bool setRate(CORBA::Long rate) throw (Error);
  virtual void refresh() throw (Error);

  virtual WindowID createWindow(const char* windowNameCorba) throw (Error);

  virtual void createScene(const char* sceneNameCorba)  throw (Error);
  virtual void createSceneWithFloor(const char* sceneNameCorba)  throw (Error);
  virtual bool addSceneToWindow(const char* sceneNameCorba, const WindowID windowId)  throw (Error);


  virtual bool addBox(const char* boxName, float boxSize1, float boxSize2, float boxSize3, const double* color) throw (Error);

  virtual bool addCapsule(const char* capsuleName, float radius, float height, const double* color) throw (Error);

  virtual bool addMesh(const char* meshNameCorba, const char* meshPathCorba)  throw (Error);

  virtual bool addCone(const char* coneName, float radius, float height, const double* color) throw (Error);

  virtual bool addCylinder(const char* cylinderName, float radius, float height, const double* color) throw (Error);

  virtual bool addSphere(const char* sphereName, float radius, const double* color) throw (Error);

  virtual bool addLine(const char* lineName, const double* pos1, const double* pos2, const double* color) throw (Error);

  virtual bool addSquareFace(const char* faceName, const double* pos1, const double* pos2, const double* pos3, const double* pos4, const double* color) throw (Error);
  virtual bool addTriangleFace(const char* faceName, const double* pos1, const double* pos2, const double* pos3, const double* color) throw (Error);

  virtual bool addURDF(const char* urdfNameCorba, const char* urdfPathCorba, const char* urdfPackagePathCorba) throw (Error);

  virtual bool addUrdfCollision (const char* urdfNameCorba,
				 const char* urdfPathCorba,
				 const char* urdfPackagePathCorba)
    throw (Error);

  virtual void addUrdfObjects (const char* urdfNameCorba,
			       const char* urdfPathCorba,
			       const char* urdfPackagePathCorba,
			       bool visual)
    throw (Error);

  virtual bool createGroup(const char* groupNameCorba)  throw (Error);
  virtual bool addToGroup(const char* nodeNameCorba, const char* groupNameCorba)  throw (Error);

  virtual bool applyConfiguration(const char* nodeNameCorba, const double* configuration)  throw (Error);

  virtual bool addLandmark(const char* nodeNameCorba, float size) throw (Error);
  virtual bool deleteLandmark(const char* nodeNameCorba) throw (Error);

  virtual bool setVisibility(const char* nodeNameCorba, const char* visibilityModeCorba)  throw (Error);
  virtual bool setWireFrameMode(const char* nodeNameCorba, const char* wireFrameModeCorba)  throw (Error);
  virtual bool setLightingMode(const char* nodeNameCorba, const char* lightingModeCorba)  throw (Error);

  virtual bool startCapture (const WindowID windowId, const char* filename,
      const char* extension) throw (Error);
  virtual bool stopCapture (const WindowID windowId) throw (Error);
  virtual bool writeNodeFile (const WindowID windowId, const char* filename) throw (Error);
}; // end of class

} /* namespace impl */
} /* namespace corbaServer */
} /* namespace graphics */

#endif /* SCENEVIEWER_CORBASERVER_GRAPHICALINTERFACE_HH */
