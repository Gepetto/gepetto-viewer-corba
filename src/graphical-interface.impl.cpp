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

      using gepetto::Names_t;

      GraphicalInterface::GraphicalInterface (corbaServer::Server* server) :
	windowsManager_ (server->windowsManager ())
      {
      }

      bool GraphicalInterface::setRate (CORBA::Long rate) throw (Error)
      {
	try {
          return windowsManager_->setRate (rate);
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      GraphicalInterface::WindowID GraphicalInterface::createWindow (const char* windowNameCorba)
	throw (Error)
      {
	try {
          return windowsManager_->createWindow (windowNameCorba);
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      GraphicalInterface::WindowID GraphicalInterface::getWindowID (const char* windowNameCorba)
	throw (Error)
      {
	try {
          return windowsManager_->getWindowID (windowNameCorba);
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      void GraphicalInterface::refresh () throw (Error)
      {
	try {
          return windowsManager_->refresh ();
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      void GraphicalInterface::createScene (const char* sceneNameCorba)
          throw (Error)
      {
	try {
          return windowsManager_->createScene (sceneNameCorba);
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      void GraphicalInterface::createSceneWithFloor (const char* sceneNameCorba)
          throw (Error)
          {
              try {
                  return windowsManager_->createSceneWithFloor (sceneNameCorba);
              } catch (const std::exception& exc) {
                  throw Error (exc.what ());
              }
          }

      bool GraphicalInterface::addSceneToWindow (const char* sceneNameCorba,
              WindowID windowId) throw (Error)
      {
          try {
              return windowsManager_->addSceneToWindow (sceneNameCorba, windowId);
          } catch (const std::exception& exc) {
              throw Error (exc.what ());
          }
      }

      bool GraphicalInterface::addFloor (const char* floorNameCorba)
          throw (Error)
      {
          try {
            return windowsManager_->addFloor (floorNameCorba);
          } catch (const std::exception& exc) {
            throw Error (exc.what ());
          }
      }


      bool GraphicalInterface::addBox (const char* boxNameCorba,
				       const float boxSize1,
				       const float boxSize2,
				       const float boxSize3,
                       const value_type* colorCorba) throw (Error)
      {
          try {
              return windowsManager_->addBox (boxNameCorba, boxSize1, boxSize2, boxSize3,
                      colorCorba);
          } catch (const std::exception& exc) {
              throw Error (exc.what ());
          }
      }

      bool GraphicalInterface::addCapsule (const char* capsuleNameCorba,
              const float radius,
              const float height,
              const value_type* colorCorba) throw (Error)
      {
          try {
              return windowsManager_->addCapsule (capsuleNameCorba, radius, height,
                      colorCorba);
          } catch (const std::exception& exc) {
              throw Error (exc.what ());
          }
      }

      bool GraphicalInterface::addArrow (const char* arrowNameCorba,
              float radius,
              float length,
              const value_type* colorCorba) throw (Error)
      {
          try {
              return windowsManager_->addArrow (arrowNameCorba, radius, length,
                      colorCorba);
          } catch (const std::exception& exc) {
              throw Error (exc.what ());
          }
      }

      bool GraphicalInterface::addRod (const char* rodNameCorba,
              const value_type* colorCorba,
              const float radius,
              const float length,
              short maxCapsule
              ) throw (Error)
      {
          try {
              return windowsManager_->addRod (rodNameCorba, colorCorba,radius, length,
                      maxCapsule);
          } catch (const std::exception& exc) {
              throw Error (exc.what ());
          }
      }

      bool GraphicalInterface::resizeCapsule(const char* capsuleNameCorba, float newHeight) throw(Error){
          try{
              return windowsManager_->resizeCapsule(capsuleNameCorba,newHeight);
          } catch (const std::exception& exc) {
              throw Error (exc.what ());
          }
      }

      bool GraphicalInterface::resizeArrow(const char* arrowNameCorba, float newRadius, float newLength) throw(Error){
          try{
              return windowsManager_->resizeArrow(arrowNameCorba,newRadius, newLength);
          } catch (const std::exception& exc) {
              throw Error (exc.what ());
          }
      }

      bool GraphicalInterface::addMesh (const char* meshNameCorba,
					const char* meshPathCorba) throw (Error)
      {
          try {
              return windowsManager_->addMesh (meshNameCorba, meshPathCorba);
          } catch (const std::exception& exc) {
              throw Error (exc.what ());
          }
      }

      bool GraphicalInterface::addCone (const char* coneNameCorba,
					const float radius, const float height,
					const value_type* colorCorba) throw (Error)
      {
          try {
              return windowsManager_->addCone (coneNameCorba, radius, height,
                      colorCorba);
          } catch (const std::exception& exc) {
              throw Error (exc.what ());
          }
      }

      bool GraphicalInterface::addCylinder (const char* cylinderNameCorba,
					    const float radius,
					    const float height,
                        const value_type* colorCorba)
          throw (Error)
      {
          try {
              return windowsManager_->addCylinder (cylinderNameCorba, radius, height,
                      colorCorba);
          } catch (const std::exception& exc) {
              throw Error (exc.what ());
          }
      }

      bool GraphicalInterface::addSphere (const char* sphereNameCorba,
					  const float radius,
                      const value_type* colorCorba)
          throw (Error)
      {
          try {
              return windowsManager_->addSphere (sphereNameCorba, radius, colorCorba);
          } catch (const std::exception& exc) {
              throw Error (exc.what ());
          }
      }

      bool GraphicalInterface::addLight (const char* lightNameCorba,
                                         const WindowID windowId,
                                         const float radius,
                                         const value_type* colorCorba)
          throw (Error)
      {
          try {
              return windowsManager_->addLight (lightNameCorba, windowId,
                  radius, colorCorba);
          } catch (const std::exception& exc) {
              throw Error (exc.what ());
          }
      }

      bool GraphicalInterface::addLine (const char* lineNameCorba,
					const value_type* posCorba1,
					const value_type* posCorba2,
					const value_type* colorCorba) throw (Error)
      {
          try {
              return windowsManager_->addLine ( lineNameCorba, posCorba1, posCorba2, colorCorba) ;
          } catch (const std::exception& exc) {
              throw Error (exc.what ());
          }
      }

      bool GraphicalInterface::addCurve (const char* curveNameCorba,
					 const PositionSeq& pos,
					 const value_type* colorCorba) throw (Error)
      {
          try {
              return windowsManager_->addCurve ( curveNameCorba, pos, colorCorba) ;
          } catch (const std::exception& exc) {
              throw Error (exc.what ());
          }
      }

      bool GraphicalInterface::setCurveMode(const char* curveName,
          const char* modeName) throw (Error)
      {
          try {
              GLenum mode;
              if      (strcasecmp (modeName, "lines") == 0) mode = GL_LINES;
              else if (strcasecmp (modeName, "line_strip") == 0) mode = GL_LINE_STRIP;
              else if (strcasecmp (modeName, "polygon") == 0) mode = GL_POLYGON;
              else throw Error ("Unknown mode name");
              return windowsManager_->setCurveMode (curveName, mode) ;
          } catch (const std::exception& exc) {
              throw Error (exc.what ());
          }
      }

      bool GraphicalInterface::addTriangleFace (const char* faceNameCorba,
						const value_type* posCorba1,
						const value_type* posCorba2,
						const value_type* posCorba3,
						const value_type* colorCorba) throw (Error)
      {
	try {
      return windowsManager_->addTriangleFace ( faceNameCorba, posCorba1, posCorba2, posCorba3, colorCorba) ;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::addSquareFace (const char* faceNameCorba,
					      const value_type* posCorba1,
					      const value_type* posCorba2,
					      const value_type* posCorba3,
					      const value_type* posCorba4,
					      const value_type* colorCorba) throw (Error)
      {
	try {
      return windowsManager_->addSquareFace ( faceNameCorba, posCorba1, posCorba2, posCorba3, posCorba4, colorCorba) ;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::addXYZaxis(const char* nodeNameCorba, const value_type* colorCorba, float radius, float sizeAxis)
          throw (Error)
      {
          try {
              return windowsManager_->addXYZaxis (nodeNameCorba,colorCorba,radius,sizeAxis);
          } catch (const std::exception& exc) {
              throw Error (exc.what ());
          }
      }

      /** initialise the roadmap (graphical roadmap)*/
      bool GraphicalInterface::createRoadmap(const char* nameCorba,const value_type* colorNodeCorba, float radius, float sizeAxis, const value_type* colorEdgeCorba) throw(Error){
          try {
              return windowsManager_->createRoadmap (nameCorba,colorNodeCorba,radius,sizeAxis,colorEdgeCorba);
          } catch (const std::exception& exc) {
              throw Error (exc.what ());
          }
      }


      bool GraphicalInterface::addEdgeToRoadmap(const char* nameRoadmap, const value_type* posFrom, const value_type* posTo) throw(Error){
          try {
              return windowsManager_->addEdgeToRoadmap (nameRoadmap,posFrom,posTo);
          } catch (const std::exception& exc) {
              throw Error (exc.what ());
          }
      }

      bool GraphicalInterface::addNodeToRoadmap(const char* nameRoadmap, const value_type* configuration) throw(Error){
          try {
              return windowsManager_->addNodeToRoadmap (nameRoadmap,configuration);
          } catch (const std::exception& exc) {
              throw Error (exc.what ());
          }
      }


      gepetto::corbaserver::Names_t* fromStringVector(const std::vector<std::string>& input)
      {
          ULong size = input.size ();
          char** nameList = gepetto::corbaserver::Names_t::allocbuf(size);
          gepetto::corbaserver::Names_t *jointNames = new gepetto::corbaserver::Names_t (size, size, nameList);
          for (std::size_t i = 0; i < input.size (); ++i)
          {
              std::string name = input[i];
              nameList [i] =
                      (char*) malloc (sizeof(char)*(name.length ()+1));
              strcpy (nameList [i], name.c_str ());
          }
          return jointNames;
      }

      gepetto::corbaserver::Names_t *GraphicalInterface::getNodeList() throw (Error)
      {
        try {
          return fromStringVector(windowsManager_->getNodeList ());
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      gepetto::corbaserver::Names_t *GraphicalInterface::getGroupNodeList(const char* group) throw (Error)
      {
        try {
          return fromStringVector(windowsManager_->getGroupNodeList(group));
    } catch (const std::exception& exc) {
      throw Error (exc.what ());
    }
      }

      gepetto::corbaserver::Names_t* GraphicalInterface::getWindowList () throw (Error)
      {
	try {
          return fromStringVector(windowsManager_->getWindowList ());
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::createGroup (const char* groupNameCorba) throw (Error)
      {
	try {
      return windowsManager_->createGroup ( groupNameCorba) ;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::addURDF (const char* urdfNameCorba,
					const char* urdfPathCorba,
					const char* urdfPackagePathCorba) throw (Error)
      {
	try {
      return windowsManager_->addURDF ( urdfNameCorba, urdfPathCorba, urdfPackagePathCorba) ;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::addUrdfCollision (const char* urdfNameCorba, const char* urdfPathCorba,
       const char* urdfPackagePathCorba) throw (Error)
      {
	try {
      return windowsManager_->addUrdfCollision ( urdfNameCorba, urdfPathCorba, urdfPackagePathCorba) ;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      void GraphicalInterface::addUrdfObjects (const char* urdfNameCorba,
					       const char* urdfPathCorba,
					       const char* urdfPackagePathCorba,
					       bool visual) throw (Error)
      {
	try {
      return windowsManager_->addUrdfObjects ( urdfNameCorba, urdfPathCorba, urdfPackagePathCorba, visual) ;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::addToGroup (const char* nodeNameCorba,
					   const char* groupNameCorba) throw (Error)
      {
	try {
      return windowsManager_->addToGroup ( nodeNameCorba, groupNameCorba) ;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::removeFromGroup (const char* nodeNameCorba,
                       const char* groupNameCorba) throw (Error)
      {
    try {
      return windowsManager_->removeFromGroup ( nodeNameCorba, groupNameCorba) ;
    } catch (const std::exception& exc) {
      throw Error (exc.what ());
    }
      }

      bool GraphicalInterface::applyConfiguration (const char* nodeNameCorba, const value_type* configurationCorba) throw (Error)
      {
	try {
      return windowsManager_->applyConfiguration ( nodeNameCorba, configurationCorba) ;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::addLandmark (const char* nodeNameCorba,
					    float size) throw (Error)
      {
	try {
      return windowsManager_->addLandmark ( nodeNameCorba, size) ;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }


      bool GraphicalInterface::deleteLandmark (const char* nodeNameCorba) throw (Error)
      {
	try {
      return windowsManager_->deleteLandmark ( nodeNameCorba) ;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::getStaticTransform (const char* nodeName,  ::gepetto::corbaserver::Transform transform) throw (Error)
      {
        try {
          return windowsManager_->getStaticTransform ( nodeName, transform) ;
        } catch (const std::exception& exc) {
          throw Error (exc.what ());
        }
      }

      bool GraphicalInterface::setVisibility (const char* nodeNameCorba,
					      const char* visibilityModeCorba) throw (Error)
      {
	try {
      return windowsManager_->setVisibility ( nodeNameCorba, visibilityModeCorba) ;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::setScale (const char* nodeNameCorba,
                          const value_type* scale) throw (Error)
      {
    try {
      return windowsManager_->setScale ( nodeNameCorba, scale) ;
    } catch (const std::exception& exc) {
      throw Error (exc.what ());
    }
      }

      bool GraphicalInterface::setWireFrameMode (const char* nodeNameCorba,
						 const char* wireFrameModeCorba) throw (Error)
      {
	try {
      return windowsManager_->setWireFrameMode ( nodeNameCorba, wireFrameModeCorba) ;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::setLightingMode (const char* nodeNameCorba,
						const char* lightingModeCorba) throw (Error)
      {
	try {
      return windowsManager_->setLightingMode ( nodeNameCorba, lightingModeCorba) ;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::setHighlight (const char* nodeNameCorba,
                                             ::CORBA::Long state) throw (Error)
      {
	try {
          return windowsManager_->setHighlight ( nodeNameCorba, state) ;
        } catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::startCapture (const WindowID windowId, const char* filename,
          const char* extension) throw (Error)
      {
	try {
      return windowsManager_->startCapture ( windowId, filename, extension) ;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::stopCapture (const WindowID windowId) throw (Error)
      {
	try {
      return windowsManager_->stopCapture ( windowId) ;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }

      bool GraphicalInterface::writeNodeFile (const WindowID windowId, const char* filename) throw (Error)
      {
	try {
      return windowsManager_->writeNodeFile ( windowId, filename) ;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }
      
      gepetto::corbaserver::floatSeq* GraphicalInterface::getNodeGlobalTransform(const char* nodeName) throw (Error)
      {
	try {
      WindowsManager::configuration_t config = windowsManager_->getNodeGlobalTransform(nodeName);
      if(config.size() != 7) throw std::runtime_error("No Node name ." + std::string(nodeName));
      gepetto::corbaserver::floatSeq *dofArray;
      std::size_t dim = config.size();
      dofArray = new gepetto::corbaserver::floatSeq();
      dofArray->length(dim);
      for(std::size_t i=0; i<dim; i++)
          (*dofArray)[i] = config[i];
      return dofArray;
	} catch (const std::exception& exc) {
	  throw Error (exc.what ());
	}
      }
    } //end namespace impl
  } //end namespace corbaServer
} //end namespace graphics
