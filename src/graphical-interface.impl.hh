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

using gepetto::gui::WindowsManagerPtr_t;
class GraphicalInterface :
    public virtual POA_gepetto::corbaserver::GraphicalInterface
{
private:
    WindowsManagerPtr_t windowsManager_;
    typedef gepetto::Error Error;
    typedef gepetto::gui::WindowsManager::value_type value_type;
    typedef gepetto::corbaserver::Transform_slice Transform_slice;

public:
    typedef CORBA::ULong WindowID;
    typedef gepetto::corbaserver::Transform Transform;
    typedef gepetto::corbaserver::TransformSeq TransformSeq;
    typedef gepetto::corbaserver::PositionSeq PositionSeq;
    typedef gepetto::corbaserver::Position Position;
    typedef gepetto::corbaserver::Position_slice Position_slice;
    typedef gepetto::corbaserver::Color Color;
    typedef gepetto::corbaserver::Color_slice Color_slice;
    typedef gepetto::corbaserver::Names_t Names_t;

    /**
        \brief Default constructor
        */
    GraphicalInterface (corbaServer::Server* server);

  virtual Names_t* getNodeList() throw (Error);
  virtual Names_t* getGroupNodeList(const char* group) throw (Error);
  virtual Names_t* getSceneList() throw (Error);
  virtual Names_t* getWindowList() throw (Error);

  virtual void refresh() throw (Error);
  virtual void setRefreshIsSynchronous(bool synchronous) throw (Error);

  virtual WindowID createWindow(const char* windowNameCorba) throw (Error);
  virtual WindowID getWindowID (const char* windowNameCorba) throw (Error);

  virtual void createScene(const char* sceneNameCorba)  throw (Error);
  virtual void createSceneWithFloor(const char* sceneNameCorba)  throw (Error);
  virtual bool addSceneToWindow(const char* sceneNameCorba, const WindowID windowId)  throw (Error);

  virtual bool attachCameraToNode(const char* nodeName, const WindowID windowId) throw (Error);
  virtual bool detachCamera(const WindowID windowId) throw (Error);

  virtual bool nodeExists(const char* nodeName) throw (Error);

  virtual bool addFloor(const char* floorName) throw (Error);

  virtual bool addBox(const char* boxName, const float boxSize1, const float boxSize2, const float boxSize3, const Color color) throw (Error);

  virtual bool addCapsule(const char* capsuleName, float radius, float height, const Color color) throw (Error);

  virtual  bool addArrow (const char* arrowNameCorba, float radius, float length,const Color colorCorba) throw (Error);

  virtual bool addRod (const char* rodNameCorba, const Color colorCorba, const float radius, const float length,short maxCapsule) throw (Error);

  virtual bool resizeCapsule(const char* capsuleNameCorba, float newHeight) throw(Error);

  virtual bool resizeArrow(const char* arrowNameCorba ,float newRadius, float newLength) throw(Error);

  virtual bool addMesh(const char* meshNameCorba, const char* meshPathCorba)  throw (Error);

  virtual bool addCone(const char* coneName, float radius, float height, const Color color) throw (Error);

  virtual bool addCylinder(const char* cylinderName, float radius, float height, const Color color) throw (Error);

  virtual bool addSphere(const char* sphereName, float radius, const Color color) throw (Error);

  virtual bool addLight(const char* lightName, const WindowID windowId, float radius, const Color color) throw (Error);

  virtual bool addLine(const char* lineName, const value_type* pos1, const value_type* pos2, const Color color) throw (Error);
  virtual bool setLineStartPoint(const char* lineName, const value_type* pos1) throw (Error);
  virtual bool setLineEndPoint(const char* lineName, const value_type* pos1) throw (Error);
  virtual bool setLineExtremalPoints(const char* lineName, const value_type* pos1, const value_type* pos2) throw (Error);

  virtual bool addCurve(const char* curveName, const PositionSeq& pos, const Color color) throw (Error);
  virtual bool setCurvePoints(const char* curveName, const PositionSeq& pos) throw (Error);

  virtual bool setCurveMode(const char* curveName, const char* modeName) throw (Error);
  virtual bool setCurvePointsSubset(const char* curveName, CORBA::Long first, CORBA::Long count) throw (Error);
  virtual bool setCurveLineWidth(const char* curveName, const float width) throw (Error);

  virtual bool addSquareFace(const char* faceName, const value_type* pos1, const value_type* pos2, const value_type* pos3, const value_type* pos4, const Color color) throw (Error);
  virtual bool setTexture (const char* nodeName, const char* filename) throw (Error);
  virtual bool addTriangleFace(const char* faceName, const value_type* pos1, const value_type* pos2, const value_type* pos3, const Color color) throw (Error);

  virtual bool addXYZaxis (const char* nodeNameCorba, const Color colorCorba, float radius, float sizeAxis) throw (Error);

  virtual bool createRoadmap(const char* nameCorba,const Color colorNodeCorba, float radius, float sizeAxis, const Color colorEdgeCorba) throw(Error);

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
  virtual bool applyConfigurations(const Names_t& nodeNameCorba, const TransformSeq& configuration)  throw (Error);

  virtual bool addLandmark(const char* nodeNameCorba, float size) throw (Error);
  virtual bool deleteLandmark(const char* nodeNameCorba) throw (Error);

  virtual Transform_slice* getStaticTransform (const char* nodeName) throw (Error);
  virtual bool setStaticTransform (const char* nodeName, const ::gepetto::corbaserver::Transform transform) throw (Error);

  virtual bool setVisibility(const char* nodeNameCorba, const char* visibilityModeCorba)  throw (Error);
  virtual bool setScale(const char* nodeNameCorba, const value_type* scale)throw (Error);
  virtual bool setColor(const char* nodeNameCorba, const Color color) throw (Error);
  virtual bool setWireFrameMode(const char* nodeNameCorba, const char* wireFrameModeCorba)  throw (Error);
  virtual bool setLightingMode(const char* nodeNameCorba, const char* lightingModeCorba)  throw (Error);
  virtual bool setHighlight(const char* nodeNameCorba, ::CORBA::Long state)  throw (Error);

  virtual void captureFrame (const WindowID windowId, const char* filename) throw (Error);
  virtual bool startCapture (const WindowID windowId, const char* filename,
      const char* extension) throw (Error);
  virtual bool stopCapture (const WindowID windowId) throw (Error);
  virtual bool setCaptureTransform (const char* filename, const Names_t& nodename) throw (Error);
  virtual void captureTransformOnRefresh (bool autoCapture) throw (Error);
  virtual void captureTransform () throw (Error);
  virtual bool writeBlenderScript (const char* filename, const Names_t& nodeNames) throw (Error);
  virtual bool writeNodeFile (const char* nodename, const char* filename) throw (Error);
  virtual bool writeWindowFile (const WindowID windowId, const char* filename) throw (Error);

  virtual Transform_slice* getNodeGlobalTransform(const char* nodeName) throw (Error);
  virtual void deleteNode (const char* nodeName, bool all) throw (Error);
  virtual bool setBackgroundColor1(const WindowID windowId,const Color colorCorba) throw (Error);
  virtual bool setBackgroundColor2(const WindowID windowId,const Color colorCorba) throw (Error);
  virtual Transform_slice* getCameraTransform(const WindowID windowId) throw (Error);
  virtual bool setCameraTransform(const WindowID windowId, const value_type *configurationCorba)throw (Error);

  // ------------- Properties -------------------- //
  virtual Names_t* getPropertyNames(const char* nodeName) throw (Error);
  virtual Names_t* getPropertyTypes(const char* nodeName) throw (Error);
  virtual char* getStringProperty(const char* nodeName, const char* propName) throw (Error);
  virtual void setStringProperty(const char* nodeName, const char* propName, const char* value) throw (Error);
  virtual Color_slice* getColorProperty(const char* nodeName, const char* propName) throw (Error);
  virtual void setColorProperty(const char* nodeName, const char* propName, const Color value) throw (Error);
  virtual Position_slice* getVector3Property(const char* nodeName, const char* propName) throw (Error);
  virtual void setVector3Property(const char* nodeName, const char* propName, const Position value) throw (Error);
  virtual float getFloatProperty(const char* nodeName, const char* propName) throw (Error);
  virtual void setFloatProperty(const char* nodeName, const char* propName, float value) throw (Error);
  virtual bool getBoolProperty(const char* nodeName, const char* propName) throw (Error);
  virtual void setBoolProperty(const char* nodeName, const char* propName, bool value) throw (Error);
  virtual CORBA::Long getIntProperty(const char* nodeName, const char* propName) throw (Error);
  virtual void setIntProperty(const char* nodeName, const char* propName, CORBA::Long value) throw (Error);
}; // end of class

} /* namespace impl */
} /* namespace corbaServer */
} /* namespace graphics */

#endif /* SCENEVIEWER_CORBASERVER_GRAPHICALINTERFACE_HH */
