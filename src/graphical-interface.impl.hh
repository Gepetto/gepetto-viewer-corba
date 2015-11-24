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

#include "gepetto/viewer/corba/server.hh"
#include "gepetto/viewer/corba/graphical-interface.hh"

namespace graphics {
namespace corbaServer {
namespace impl {

struct NodeConfiguration {
    NodePtr_t node;
    osgVector3 position;
    osgQuat quat;
};

using graphics::WindowsManagerPtr_t;
class GraphicalInterface :
    public virtual POA_gepetto::corbaserver::GraphicalInterface
{
private:
    WindowsManagerPtr_t windowsManager_;
    typedef gepetto::Error Error;
    typedef CORBA::ULong WindowID;
    typedef graphics::WindowsManager::value_type value_type;

public:
    typedef gepetto::corbaserver::PositionSeq PositionSeq;
    typedef gepetto::corbaserver::Color Color;

    /**
        \brief Default constructor
        */
    GraphicalInterface (corbaServer::Server* server);

  virtual gepetto::corbaserver::Names_t* getNodeList() throw (Error);
  virtual gepetto::corbaserver::Names_t* getGroupNodeList(const char* group) throw (Error);
  virtual gepetto::corbaserver::Names_t* getWindowList() throw (Error);

  virtual bool setRate(CORBA::Long rate) throw (Error);
  virtual void refresh() throw (Error);

  virtual WindowID createWindow(const char* windowNameCorba) throw (Error);
  virtual WindowID getWindowID (const char* windowNameCorba) throw (Error);

  virtual void createScene(const char* sceneNameCorba)  throw (Error);
  virtual void createSceneWithFloor(const char* sceneNameCorba)  throw (Error);
  virtual bool addSceneToWindow(const char* sceneNameCorba, const WindowID windowId)  throw (Error);


  virtual bool addFloor(const char* floorName) throw (Error);

  virtual bool addBox(const char* boxName, float boxSize1, float boxSize2, float boxSize3, const value_type* color) throw (Error);

  virtual bool addCapsule(const char* capsuleName, float radius, float height, const value_type* color) throw (Error);

  virtual  bool addArrow (const char* arrowNameCorba, float radius, float length,const value_type* colorCorba) throw (Error);

  virtual bool addRod (const char* rodNameCorba, const value_type* colorCorba, const float radius, const float length,short maxCapsule) throw (Error);

  virtual bool resizeCapsule(const char* capsuleNameCorba, float newHeight) throw(Error);

  virtual bool resizeArrow(const char* arrowNameCorba ,float newRadius, float newLength) throw(Error);

  virtual bool addMesh(const char* meshNameCorba, const char* meshPathCorba)  throw (Error);

  virtual bool addCone(const char* coneName, float radius, float height, const value_type* color) throw (Error);

  virtual bool addCylinder(const char* cylinderName, float radius, float height, const value_type* color) throw (Error);

  virtual bool addSphere(const char* sphereName, float radius, const value_type* color) throw (Error);

  virtual bool addLight(const char* lightName, const WindowID windowId, float radius, const value_type* color) throw (Error);

  virtual bool addLine(const char* lineName, const value_type* pos1, const value_type* pos2, const value_type* color) throw (Error);

  virtual bool addCurve(const char* curveName, const PositionSeq& pos, const value_type* color) throw (Error);

  virtual bool setCurveMode(const char* curveName, const char* modeName) throw (Error);

  virtual bool addSquareFace(const char* faceName, const value_type* pos1, const value_type* pos2, const value_type* pos3, const value_type* pos4, const value_type* color) throw (Error);
  virtual bool addTriangleFace(const char* faceName, const value_type* pos1, const value_type* pos2, const value_type* pos3, const value_type* color) throw (Error);

  virtual bool addXYZaxis (const char* nodeNameCorba, const value_type* colorCorba, float radius, float sizeAxis) throw (Error);

  virtual bool createRoadmap(const char* nameCorba,const value_type* colorNodeCorba, float radius, float sizeAxis, const value_type* colorEdgeCorba) throw(Error);

  virtual bool addEdgeToRoadmap(const char* nameRoadmap, const value_type* posFrom, const value_type* posTo) throw(Error);

  virtual bool addNodeToRoadmap(const char* nameRoadmap, const value_type* configuration) throw(Error);


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
  virtual bool removeFromGroup (const char* nodeNameCorba,const char* groupNameCorba)  throw (Error);

  virtual bool applyConfiguration(const char* nodeNameCorba, const value_type* configuration)  throw (Error);

  virtual bool addLandmark(const char* nodeNameCorba, float size) throw (Error);
  virtual bool deleteLandmark(const char* nodeNameCorba) throw (Error);

  virtual bool getStaticTransform (const char* nodeName, ::gepetto::corbaserver::Transform transform) throw (Error);

  virtual bool setVisibility(const char* nodeNameCorba, const char* visibilityModeCorba)  throw (Error);
  virtual bool setScale(const char* nodeNameCorba, const value_type* scale)throw (Error);
  virtual bool setColor(const char* nodeNameCorba, const Color color) throw (Error);
  virtual bool setWireFrameMode(const char* nodeNameCorba, const char* wireFrameModeCorba)  throw (Error);
  virtual bool setLightingMode(const char* nodeNameCorba, const char* lightingModeCorba)  throw (Error);
  virtual bool setHighlight(const char* nodeNameCorba, ::CORBA::Long state)  throw (Error);

  virtual bool startCapture (const WindowID windowId, const char* filename,
      const char* extension) throw (Error);
  virtual bool stopCapture (const WindowID windowId) throw (Error);
  virtual bool setCaptureTransform (const char* filename, const char* nodename) throw (Error);
  virtual void captureTransform () throw (Error);
  virtual bool writeNodeFile (const char* nodename, const char* filename) throw (Error);
  virtual bool writeWindowFile (const WindowID windowId, const char* filename) throw (Error);
  virtual gepetto::corbaserver::floatSeq* getNodeGlobalTransform(const char* nodeName) throw (Error);
}; // end of class

} /* namespace impl */
} /* namespace corbaServer */
} /* namespace graphics */

#endif /* SCENEVIEWER_CORBASERVER_GRAPHICALINTERFACE_HH */
