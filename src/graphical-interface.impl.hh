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

#include "server.hh"
#include "gepetto/viewer/corba/graphical-interface.hh"

namespace gepetto {
namespace viewer {
namespace corba {
namespace impl {
  using namespace ::gepetto::corbaserver;

using gepetto::gui::WindowsManagerPtr_t;
class GraphicalInterface :
    public virtual POA_gepetto::corbaserver::GraphicalInterface
{
private:
    WindowsManagerPtr_t windowsManager_;
    QObject* qparent_;
    typedef gepetto::gui::WindowsManager::value_type value_type;

public:
    /**
        \brief Default constructor
        */
    GraphicalInterface (corba::Server* server);

    void qparent (QObject* parent)
    {
      qparent_ = parent;
    }

  virtual Names_t* getNodeList();
  virtual Names_t* getGroupNodeList(const char* group);
  virtual Names_t* getSceneList();
  virtual Names_t* getWindowList();

  virtual void refresh();
  virtual void setRefreshIsSynchronous(bool synchronous);

  virtual char* createWindow(const char* windowNameCorba);
  virtual char* getWindowID (const char* windowNameCorba);

  virtual void createScene(const char* sceneNameCorba);
  virtual void createSceneWithFloor(const char* sceneNameCorba);
  virtual bool addSceneToWindow(const char* sceneNameCorba, const char* windowId);

  virtual bool attachCameraToNode(const char* nodeName, const char* windowId);
  virtual bool detachCamera(const char* windowId);

  virtual bool nodeExists(const char* nodeName);

  virtual bool addFloor(const char* floorName);

  virtual bool addBox(const char* boxName, const float boxSize1, const float boxSize2, const float boxSize3, const Color color);

  virtual bool addCapsule(const char* capsuleName, float radius, float height, const Color color);

  virtual  bool addArrow (const char* arrowNameCorba, float radius, float length,const Color colorCorba);

  virtual bool addRod (const char* rodNameCorba, const Color colorCorba, const float radius, const float length,short maxCapsule);

  virtual bool resizeCapsule(const char* capsuleNameCorba, float newHeight);

  virtual bool resizeArrow(const char* arrowNameCorba ,float newRadius, float newLength);

  virtual bool addMesh(const char* meshNameCorba, const char* meshPathCorba);

  virtual void removeLightSources (const char* meshNameCorba);

  virtual bool addCone(const char* coneName, float radius, float height, const Color color);

  virtual bool addCylinder(const char* cylinderName, float radius, float height, const Color color);

  virtual bool addSphere(const char* sphereName, float radius, const Color color);

  virtual bool addLight(const char* lightName, const char* windowId, float radius, const Color color);

  virtual bool addLine(const char* lineName, const value_type* pos1, const value_type* pos2, const Color color);
  virtual bool setLineStartPoint(const char* lineName, const value_type* pos1);
  virtual bool setLineEndPoint(const char* lineName, const value_type* pos1);
  virtual bool setLineExtremalPoints(const char* lineName, const value_type* pos1, const value_type* pos2);

  virtual bool addCurve(const char* curveName, const PositionSeq& pos, const Color color);
  virtual bool setCurvePoints(const char* curveName, const PositionSeq& pos);

  virtual bool setCurveMode(const char* curveName, const char* modeName);
  virtual bool setCurvePointsSubset(const char* curveName, CORBA::Long first, CORBA::Long count);
  virtual bool setCurveLineWidth(const char* curveName, const float width);

  virtual bool addSquareFace(const char* faceName, const value_type* pos1, const value_type* pos2, const value_type* pos3, const value_type* pos4, const Color color);
  virtual bool setTexture (const char* nodeName, const char* filename);
  virtual bool addTriangleFace(const char* faceName, const value_type* pos1, const value_type* pos2, const value_type* pos3, const Color color);

  virtual bool addXYZaxis (const char* nodeNameCorba, const Color colorCorba, float radius, float sizeAxis);

  virtual bool createRoadmap(const char* nameCorba,const Color colorNodeCorba, float radius, float sizeAxis, const Color colorEdgeCorba);

  virtual bool addEdgeToRoadmap(const char* nameRoadmap, const value_type* posFrom, const value_type* posTo);

  virtual bool addNodeToRoadmap(const char* nameRoadmap, const value_type* configuration);


  virtual bool addURDF(const char* urdfNameCorba, const char* urdfPathCorba);

  virtual bool addUrdfCollision (const char* urdfNameCorba,
				 const char* urdfPathCorba);

  virtual void addUrdfObjects (const char* urdfNameCorba,
			       const char* urdfPathCorba,
			       bool visual);

  virtual bool createGroup(const char* groupNameCorba);
  virtual bool addToGroup(const char* nodeNameCorba, const char* groupNameCorba);
  virtual bool removeFromGroup (const char* nodeNameCorba,const char* groupNameCorba);

  virtual bool applyConfiguration(const char* nodeNameCorba, const value_type* configuration);
  virtual bool applyConfigurations(const Names_t& nodeNameCorba, const TransformSeq& configuration);

  virtual bool addLandmark(const char* nodeNameCorba, float size);
  virtual bool deleteLandmark(const char* nodeNameCorba);

  virtual Transform_slice* getStaticTransform (const char* nodeName);
  virtual bool setStaticTransform (const char* nodeName, const Transform transform);

  virtual bool setVisibility(const char* nodeNameCorba, const char* visibilityModeCorba);
  virtual bool setScale(const char* nodeNameCorba, const value_type* scale);
  virtual bool setColor(const char* nodeNameCorba, const Color color);
  virtual bool setWireFrameMode(const char* nodeNameCorba, const char* wireFrameModeCorba);
  virtual bool setLightingMode(const char* nodeNameCorba, const char* lightingModeCorba);
  virtual bool setHighlight(const char* nodeNameCorba, ::CORBA::Long state);

  virtual void captureFrame (const char* windowId, const char* filename);
  virtual bool startCapture (const char* windowId, const char* filename,
      const char* extension);
  virtual bool stopCapture (const char* windowId);
  virtual bool setCaptureTransform (const char* filename, const Names_t& nodename);
  virtual void captureTransformOnRefresh (bool autoCapture);
  virtual void captureTransform ();
  virtual bool writeBlenderScript (const char* filename, const Names_t& nodeNames);
  virtual bool writeNodeFile (const char* nodename, const char* filename);
  virtual bool writeWindowFile (const char* windowId, const char* filename);

  virtual Transform_slice* getNodeGlobalTransform(const char* nodeName);
  virtual void deleteNode (const char* nodeName, bool all);
  virtual bool setBackgroundColor1(const char* windowId,const Color colorCorba);
  virtual bool setBackgroundColor2(const char* windowId,const Color colorCorba);
  virtual Transform_slice* getCameraTransform(const char* windowId);
  virtual bool setCameraTransform(const char* windowId, const value_type *configurationCorba);

  // ------------- Properties -------------------- //
  virtual Names_t* getPropertyNames(const char* nodeName);
  virtual Names_t* getPropertyTypes(const char* nodeName);
  virtual char* getStringProperty(const char* nodeName, const char* propName);
  virtual void setStringProperty(const char* nodeName, const char* propName, const char* value);
  virtual Color_slice* getColorProperty(const char* nodeName, const char* propName);
  virtual void setColorProperty(const char* nodeName, const char* propName, const Color value);
  virtual Position_slice* getVector3Property(const char* nodeName, const char* propName);
  virtual void setVector3Property(const char* nodeName, const char* propName, const Position value);
  virtual Position_slice* getVector2Property(const char* nodeName, const char* propName);
  virtual void setVector2Property(const char* nodeName, const char* propName, const Position value);
  virtual float getFloatProperty(const char* nodeName, const char* propName);
  virtual void setFloatProperty(const char* nodeName, const char* propName, float value);
  virtual bool getBoolProperty(const char* nodeName, const char* propName);
  virtual void setBoolProperty(const char* nodeName, const char* propName, bool value);
  virtual CORBA::Long getIntProperty(const char* nodeName, const char* propName);
  virtual void setIntProperty(const char* nodeName, const char* propName, CORBA::Long value);
  virtual void callVoidProperty(const char* nodeName, const char* propName);

  // ------------- Callbacks -------------------- //
  bool registerNodeCallback (NodeCallback_ptr cb);
}; // end of class

} /* namespace impl */
} /* namespace corba */
} /* namespace viewer */
} /* namespace gepetto */

#endif /* SCENEVIEWER_CORBASERVER_GRAPHICALINTERFACE_HH */
