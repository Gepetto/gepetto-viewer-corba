//
//  client-cpp.cc
//  C++ CORBA client interface for SceneViewer.
//
//  Created by Mathieu Geisert in December 2014.
//  Copyright (c) 2014 LAAS-CNRS. All rights reserved.
//

#include <gepetto/viewer/corba/client.hh>

namespace graphics { 
namespace corbaServer {

void ClientCpp::se3ToCorba(CORBA::Float* corbaPosition, const se3::SE3& se3position)
{
    Eigen::Quaternion<value_type> q(se3position.rotation());
    corbaPosition[0] = se3position.translation()(0);
    corbaPosition[1] = se3position.translation()(1);
    corbaPosition[2] = se3position.translation()(2);    
    corbaPosition[3] = q.w();
    corbaPosition[4] = q.x();
    corbaPosition[5] = q.y();
    corbaPosition[6] = q.z();        
}

ClientCpp::ClientCpp()
{
    int    argc=0;       // Dummy variables to support following call.
    char** argv=0;
    orb_ = CORBA::ORB_init(argc, argv);

    // Get a reference to the Naming Service
    CORBA::Object_var rootContextObj = 
        orb_->resolve_initial_references("NameService");
    CosNaming::NamingContext_var nc =
	CosNaming::NamingContext::_narrow(rootContextObj.in());

    CosNaming::Name name;
    name.length(2);
    name[0].id = (const char *) "gepetto";
    name[0].kind = (const char *) "viewer";
    name[1].id = (const char *) "corbaserver";
    name[1].kind = (const char *) "gui";
    // Invoke the root context to retrieve the object reference
    CORBA::Object_var managerObj = nc->resolve(name);
    // Narrow the previous object to obtain the correct type
    manager_ =
      gepetto::corbaserver::GraphicalInterface::_narrow(managerObj.in());
}

ClientCpp::~ClientCpp()
{
    //manager_->shutdown();
    if (!CORBA::is_nil(orb_)) {
	try {
	    orb_->destroy();
	    std::cout << "Ending CORBA..." << std::endl;
	} catch(const CORBA::Exception& e) {
	    std::cout << "orb->destroy failed" << std::endl;
	}
    }
}

void ClientCpp::getNodeList()
{
   manager_->getNodeList();
}


void ClientCpp::getWindowList()
{
   manager_->getWindowList();
}

bool ClientCpp::setRate(int  rate)
{
   return manager_->setRate(rate);
}

void ClientCpp::refresh()
{
    manager_->refresh();
}

ClientCpp::WindowID ClientCpp::createWindow(const char* windowName)
{
    return manager_->createWindow(windowName);
}


    //void ClientCpp::createWindow(const char* name, CORBA::ULong x, CORBA::ULong y, CORBA::ULong width, CORBA::ULong height) ;

void ClientCpp::createScene(const char* sceneName)
{
  manager_->createScene(sceneName);
}

void ClientCpp::createSceneWithFloor(const char* sceneName)
{
  manager_->createSceneWithFloor(sceneName);
}

bool ClientCpp::addSceneToWindow(const char* sceneName, const ClientCpp::WindowID windowId)
{
   return  manager_->addSceneToWindow(sceneName, windowId);
}

/*bool ClientCpp::addBox(const char* boxName, float boxSize1, float boxSize2, float boxSize3)
{
    return manager_->addBox(boxName, boxSize1, boxSize2, boxSize3);
}*/

bool ClientCpp::addBox(const char* boxName, const float boxSize1, const float boxSize2, const float boxSize3, const value_type* color)
{
    return manager_->addBox(boxName, boxSize1, boxSize2, boxSize3, color);
}

/*bool ClientCpp::addCapsule(const char* capsuleName, float radius, float height)
{
    return manager_->addCapsule(capsuleName, radius, height);
}*/

bool ClientCpp::addCapsule(const char* capsuleName, const float radius, const float height, const value_type* color)
{
    return manager_->addCapsule(capsuleName, radius, height, color);
}

bool ClientCpp::addMesh(const char* meshName, const char* meshPath)
{
    return manager_->addMesh(meshName, meshPath);
}

/*bool ClientCpp::addCone(const char* coneName, float radius, float height)
{
    return manager_->addCone(coneName, radius, height);
}*/

bool ClientCpp::addCone(const char* coneName, const float radius, const float height, const value_type* color)
{
    return manager_->addCone(coneName, radius, height, color);
}

/*bool ClientCpp::addCylinder(const char* cylinderName, float radius, float height)
{
    return manager_->addCylinder(cylinderName, radius, height);
}*/

bool ClientCpp::addCylinder(const char* cylinderName, const float radius, const float height, const value_type* color)
{
    return manager_->addCylinder(cylinderName, radius, height, color);
}

/*bool ClientCpp::addSphere(const char* sphereName, float radius)
{
    return manager_->addSphere(sphereName, radius);
}*/

bool ClientCpp::addSphere(const char* sphereName, const float radius, const value_type* color)
{
    return manager_->addSphere(sphereName, radius, color);
}

bool ClientCpp::addLine(const char* lineName, const value_type* pos1, const value_type* pos2, const value_type* color)
{
    return manager_->addLine(lineName, pos1, pos2, color);
}

bool ClientCpp::addTriangleFace(const char* faceName, const value_type* pos1, const value_type* pos2, const value_type* pos3, const value_type* color)
{
    return manager_->addTriangleFace(faceName, pos1, pos2, pos3, color);
}

bool ClientCpp::addSquareFace(const char* faceName, const value_type* pos1, const value_type* pos2, const value_type* pos3, const value_type* pos4, const value_type* color)
{
    return manager_->addSquareFace(faceName, pos1, pos2, pos3, pos4, color);
}

bool ClientCpp::addURDF(const char* urdfName, const char* urdfPath, const char* urdfPackagePath)
{
    return manager_->addURDF(urdfName, urdfPath, urdfPackagePath);
}

bool ClientCpp::createGroup(const char* groupName)
{
    return manager_->createGroup(groupName);
}

bool ClientCpp::addToGroup(const char* nodeName, const char* groupName)
{
    return manager_->addToGroup(nodeName, groupName);
}

bool ClientCpp::applyConfiguration(const char* nodeName, const se3::SE3& se3position)
{
    CORBA::Float corbaPosition[7];
    ClientCpp::se3ToCorba(corbaPosition, se3position);
    return manager_->applyConfiguration(nodeName, corbaPosition);
}

bool ClientCpp::setVisibility(const char* nodeName, const char* visibilityMode)
{
    return manager_->setVisibility(nodeName, visibilityMode);
}

bool ClientCpp::setWireFrameMode(const char* nodeName, const char* wireFrameMode)
{
    return manager_->setWireFrameMode(nodeName, wireFrameMode);
}
bool ClientCpp::setLightingMode(const char* nodeName, const char* lightingMode)
{
    return manager_->setLightingMode(nodeName, lightingMode);
}

bool ClientCpp::startCapture (const WindowID windowId, const char* filename, const char* extension)
{
  return manager_->startCapture (windowId, filename, extension);
}

bool ClientCpp::stopCapture (const WindowID windowId)
{
  return manager_->stopCapture (windowId);
}

bool ClientCpp::writeNodeFile (const WindowID windowId, const char* filename)
{
  return manager_->writeNodeFile (windowId, filename);
}

} // end of namespace corbaserver
} // end of namespace graphics
