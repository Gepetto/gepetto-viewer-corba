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

#include <Graphics/WindowManager.h>
#include <Graphics/Node.h>
#include <Graphics/GroupNode.h>
#include <Graphics/LeafNodeBox.h>
#include <Graphics/LeafNodeCapsule.h>
#include <Graphics/LeafNodeCone.h>
#include <Graphics/LeafNodeCylinder.h>
#include <Graphics/LeafNodeLine.h>
#include <Graphics/LeafNodeSphere.h>
#include <Graphics/macros.h>
#include <Graphics/config.h>
#include <Graphics/LeafNodeGround.h>
#include <Graphics/LeafNodeCollada.h>
#include <Graphics/URDFparser.h>

#include "server.hh"
#include "graphicalinterface.hh"

namespace Graphics {
namespace corbaServer {
namespace impl {

struct NodeConfiguration {
    NodePtr_t node;
    osgVector3 position;
    osgQuat quat;
};

class GraphicalInterface : public virtual POA_Graphics::corbaserver::Graphicalinterface
{
private:
    std::map<std::string, WindowManagerPtr_t> windowManagers_;
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

    virtual void getNodeList();
    virtual void getWindowList();

    virtual bool setRate(CORBA::Long rate);
    virtual void refresh();

    virtual bool createWindow(const char* windowNameCorba);
    //virtual void createWindow(const char* name, CORBA::ULong x, CORBA::ULong y, CORBA::ULong width, CORBA::ULong height) ;

    virtual bool createScene(const char* sceneNameCorba) ;
    virtual bool createSceneWithFloor(const char* sceneNameCorba) ;
    virtual bool addSceneToWindow(const char* sceneNameCorba, const char* windowNameCorba) ;


    virtual bool addBox(const char* boxName, float boxSize1, float boxSize2, float boxSize3, const double* color);
    //virtual bool addBox(const char* boxNameCorba, float boxSize1, float boxSize2, float boxSize3) ;

    //virtual bool addCapsule(const char* capsuleNameCorba, float radius, float height) ;
    virtual bool addCapsule(const char* capsuleName, float radius, float height, const double* color);

    virtual bool addMesh(const char* meshNameCorba, const char* meshPathCorba) ;

    //virtual bool addCone(const char* coneNameCorba, float radius, float height) ;
    virtual bool addCone(const char* coneName, float radius, float height, const double* color);

    //virtual bool addCylinder(const char* cylinderNameCorba, float radius, float height) ;
    virtual bool addCylinder(const char* cylinderName, float radius, float height, const double* color);

    //virtual bool addSphere(const char* sphereNameCorba, float radius) ;
    virtual bool addSphere(const char* sphereName, float radius, const double* color);

    virtual bool addLine(const char* lineName, const double* pos1, const double* pos2, const double* color);

    virtual bool addURDF(const char* urdfNameCorba, const char* urdfPathCorba, const char* urdfPackagePathCorba);

    virtual bool createGroup(const char* groupNameCorba) ;
    virtual bool addToGroup(const char* nodeNameCorba, const char* groupNameCorba) ;

    virtual bool applyConfiguration(const char* nodeNameCorba, const double* configuration) ;

    virtual bool setVisibility(const char* nodeNameCorba, const char* visibilityModeCorba) ;
    virtual bool setWireFrameMode(const char* nodeNameCorba, const char* wireFrameModeCorba) ;
    virtual bool setLightingMode(const char* nodeNameCorba, const char* lightingModeCorba) ;

}; // end of class

} /* namespace impl */
} /* namespace corbaServer */
} /* namespace Graphics */

#endif /* SCENEVIEWER_CORBASERVER_GRAPHICALINTERFACE_HH */
