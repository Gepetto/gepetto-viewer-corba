//
//  client-cpp.hh
//  C++ CORBA client interface for SceneViewer.
//
//  Created by Mathieu Geisert in December 2014.
//  Copyright (c) 2014 LAAS-CNRS. All rights reserved.
//

#ifndef SCENEVIEWER_CORBASERVER_CLIENTCPP_HH
#define SCENEVIEWER_CORBASERVER_CLIENTCPP_HH

#include <iostream>
#include <omniORB4/CORBA.h>
#include <gepetto/viewer/corba/se3.hh>
#include <gepetto/viewer/corba/graphical-interface.hh>


namespace graphics {
namespace corbaServer {

class ClientCpp
{
private:
    CORBA::ORB_var orb_;
  gepetto::corbaserver::GraphicalInterface_var manager_;

    static void se3ToCorba(CORBA::Double* corbaPosition, const se3::SE3& se3position);

protected:

public:
    // Default constructor
    ClientCpp();
    // Default destructor
    ~ClientCpp();

    void getNodeList();
    void getWindowList();

    bool setRate(int rate);
    void refresh();

    bool createWindow(const char* windowName);


    //void createWindow(const char* name, CORBA::ULong x, CORBA::ULong y, CORBA::ULong width, CORBA::ULong height) ;

    bool createScene(const char* sceneName) ;
    bool createSceneWithFloor(const char* sceneName) ;
    bool addSceneToWindow(const char* sceneName, const char* windowName) ;


    bool addBox(const char* boxName, const float boxSize1, const float boxSize2, const float boxSize3, const double* color);
    //bool addBox(const char* boxName, float boxSize1, float boxSize2, float boxSize3) ;

    //bool addCapsule(const char* capsuleName, float radius, float height) ;
    bool addCapsule(const char* capsuleName, const float radius, const float height, const double* color);

    bool addMesh(const char* meshName, const char* meshPath) ;

    //bool addCone(const char* coneName, float radius, float height) ;
    bool addCone(const char* coneName, const float radius, const float height, const double* color);

    //bool addCylinder(const char* cylinderName, float radius, float height) ;
    bool addCylinder(const char* cylinderName, const float radius, const float height, const double* color);

    //bool addSphere(const char* sphereName, float radius) ;
    bool addSphere(const char* sphereName, const float radius, const double* color);

    bool addLine(const char* lineName, const double* pos1, const double* pos2, const double* color);

    bool addTriangleFace(const char* lineName, const double* pos1, const double* pos2, const double* pos3, const double* color);
    bool addSquareFace(const char* lineName, const double* pos1, const double* pos2, const double* pos3, const double* pos4, const double* color);

    bool addURDF(const char* urdfName, const char* urdfPath, const char* urdfPackagePath);

    bool createGroup(const char* groupName) ;
    bool addToGroup(const char* nodeName, const char* groupName) ;

    bool applyConfiguration(const char* nodeName, const se3::SE3& se3position) ;

    bool setVisibility(const char* nodeName, const char* visibilityMode) ;
    bool setWireFrameMode(const char* nodeName, const char* wireFrameMode) ;
    bool setLightingMode(const char* nodeName, const char* lightingMode) ;

}; //end of class ClientCpp

} //end of namespace corbaserver
} //end of namespace graphics

#endif // SCENEVIEWER_CORBASERVER_CLIENTCPP_HH
