//
//  GraphicalInterface.impl.cc
//  Corbaserver for gepetto-viewer
//
//  Author: Mathieu Geisert, Florent Lamiraux
//  Copyright (c) 2014 LAAS-CNRS. All rights reserved.
//

#include <iostream>
#include "graphical-interface.impl.hh"
#include "conversions.hh"
#include "node-callback.hh"

namespace gepetto {
  namespace viewer {
  namespace corba {
    namespace impl {
#define CAT(a, ...) PRIMITIVE_CAT(a, __VA_ARGS__)
#define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

#define IIF(c) PRIMITIVE_CAT(IIF_, c)
#define IIF_0(t, ...) __VA_ARGS__
#define IIF_1(t, ...) t

#define PROBE(x) x, 1

#define CHECK(...) CHECK_N(__VA_ARGS__, 0, 0)
#define CHECK_N(x, n, ...) n

#define TYPE_VOID () // Detect VOID

#define TYPE_PROBE(type)            TYPE_PROBE_PROXY( TYPE_##type )  // concatenate prefix with user name
#define TYPE_PROBE_PROXY(val)       TYPE_PROBE_PRIMIVIE(val)         // expand arguments
#define TYPE_PROBE_PRIMIVIE(x)      TYPE_PROBE_COMBINE_ x            // merge
#define TYPE_PROBE_COMBINE_()       PROBE(~)                         // if merge successful, expand to probe

#define IS_VOID(type) CHECK(TYPE_PROBE(type))


#define WRITE_RET_T(T)  traits<T>::Ret_t
#define WRITE_RET_OP(T) IIF( IS_VOID(T) ) ( ;/* nothing */, return traits<T>::ret)

#define WRITE_INPUT_ARG(T, n) traits< T >::In_t arg##n
#define WRITE_INPUT_ARGS_1(T0               ) WRITE_INPUT_ARG(T0, 0)
#define WRITE_INPUT_ARGS_2(T0,T1            ) WRITE_INPUT_ARGS_1(T0            ), WRITE_INPUT_ARG(T1,1)
#define WRITE_INPUT_ARGS_3(T0,T1,T2         ) WRITE_INPUT_ARGS_2(T0,T1         ), WRITE_INPUT_ARG(T2,2)
#define WRITE_INPUT_ARGS_4(T0,T1,T2,T3      ) WRITE_INPUT_ARGS_3(T0,T1,T2      ), WRITE_INPUT_ARG(T3,3)
#define WRITE_INPUT_ARGS_5(T0,T1,T2,T3,T4   ) WRITE_INPUT_ARGS_4(T0,T1,T2,T3   ), WRITE_INPUT_ARG(T4,4)
#define WRITE_INPUT_ARGS_6(T0,T1,T2,T3,T4,T5) WRITE_INPUT_ARGS_5(T0,T1,T2,T3,T4), WRITE_INPUT_ARG(T5,5)

#define WRITE_OP_ARG(T,n) traits<T>::op(arg##n)
#define WRITE_OP_ARGS_1(T0               ) WRITE_OP_ARG(T0,0)
#define WRITE_OP_ARGS_2(T0,T1            ) WRITE_OP_ARGS_1(T0            ), WRITE_OP_ARG(T1,1)
#define WRITE_OP_ARGS_3(T0,T1,T2         ) WRITE_OP_ARGS_2(T0,T1         ), WRITE_OP_ARG(T2,2)
#define WRITE_OP_ARGS_4(T0,T1,T2,T3      ) WRITE_OP_ARGS_3(T0,T1,T2      ), WRITE_OP_ARG(T3,3)
#define WRITE_OP_ARGS_5(T0,T1,T2,T3,T4   ) WRITE_OP_ARGS_4(T0,T1,T2,T3   ), WRITE_OP_ARG(T4,4)
#define WRITE_OP_ARGS_6(T0,T1,T2,T3,T4,T5) WRITE_OP_ARGS_5(T0,T1,T2,T3,T4), WRITE_OP_ARG(T5,5)

#define BIND_TO_WINDOWS_MANAGER_0(Ret, Func      )                             \
      WRITE_RET_T(Ret) GraphicalInterface::Func (                  )           \
      {                                                                        \
          try {                                                                \
            WRITE_RET_OP(Ret)( windowsManager_->Func (                ));      \
          } catch (const std::exception& exc) {                                \
              throw Error (exc.what ());                                       \
          }                                                                    \
      }

#define BIND_TO_WINDOWS_MANAGER_1(Ret, Func, T0)                               \
      WRITE_RET_T(Ret) GraphicalInterface::Func (WRITE_INPUT_ARGS_1(T0))       \
      {                                                                        \
          try {                                                                \
            WRITE_RET_OP(Ret)( windowsManager_->Func ( WRITE_OP_ARGS_1(T0) )); \
          } catch (const std::exception& exc) {                                \
              throw Error (exc.what ());                                       \
          }                                                                    \
      }

#define BIND_TO_WINDOWS_MANAGER_2(Ret, Func, T0, T1)     \
      WRITE_RET_T(Ret) GraphicalInterface::Func (WRITE_INPUT_ARGS_2(T0, T1))   \
      {                                                                        \
          try {                                                                \
            WRITE_RET_OP(Ret)( windowsManager_->Func ( WRITE_OP_ARGS_2(T0, T1) ));\
          } catch (const std::exception& exc) {                                \
              throw Error (exc.what ());                                       \
          }                                                                    \
      }

#define BIND_TO_WINDOWS_MANAGER_3(Ret, Func, T0, T1, T2)                       \
      WRITE_RET_T(Ret) GraphicalInterface::Func (WRITE_INPUT_ARGS_3(T0, T1, T2))          \
      {                                                                        \
          try {                                                                \
            WRITE_RET_OP(Ret)( windowsManager_->Func ( WRITE_OP_ARGS_3(T0, T1, T2) ));\
          } catch (const std::exception& exc) {                                \
              throw Error (exc.what ());                                       \
          }                                                                    \
      }

#define BIND_TO_WINDOWS_MANAGER_4(Ret, Func, T0, T1, T2, T3)                   \
      WRITE_RET_T(Ret) GraphicalInterface::Func (WRITE_INPUT_ARGS_4(T0, T1, T2, T3))          \
      {                                                                        \
          try {                                                                \
            WRITE_RET_OP(Ret)( windowsManager_->Func ( WRITE_OP_ARGS_4(T0, T1, T2, T3) ));\
          } catch (const std::exception& exc) {                                \
              throw Error (exc.what ());                                       \
          }                                                                    \
      }

#define BIND_TO_WINDOWS_MANAGER_5(Ret, Func, T0, T1, T2, T3, T4)               \
      WRITE_RET_T(Ret) GraphicalInterface::Func (WRITE_INPUT_ARGS_5(T0, T1, T2, T3, T4))          \
      {                                                                        \
          try {                                                                \
            WRITE_RET_OP(Ret)( windowsManager_->Func ( WRITE_OP_ARGS_5(T0, T1, T2, T3, T4) ));\
          } catch (const std::exception& exc) {                                \
              throw Error (exc.what ());                                       \
          }                                                                    \
      }

#define BIND_TO_WINDOWS_MANAGER_6(Ret, Func, T0,T1,T2,T3,T4,T5)                \
      WRITE_RET_T(Ret) GraphicalInterface::Func (WRITE_INPUT_ARGS_6(T0,T1,T2,T3,T4,T5))          \
      {                                                                        \
          try {                                                                \
            WRITE_RET_OP(Ret)( windowsManager_->Func ( WRITE_OP_ARGS_6(T0,T1,T2,T3,T4,T5) ));\
          } catch (const std::exception& exc) {                                \
              throw Error (exc.what ());                                       \
          }                                                                    \
      }

      GraphicalInterface::GraphicalInterface (corba::Server* server)
        : windowsManager_ (server->windowsManager ())
        , qparent_ (NULL)
      {
      }

      BIND_TO_WINDOWS_MANAGER_1(WINDOW_ID, createWindow, STRING)

      BIND_TO_WINDOWS_MANAGER_1(WINDOW_ID, getWindowID, STRING)

      BIND_TO_WINDOWS_MANAGER_0(VOID, refresh)

      BIND_TO_WINDOWS_MANAGER_1(VOID, setRefreshIsSynchronous, BOOL)

      BIND_TO_WINDOWS_MANAGER_1(VOID, createScene, STRING)

      BIND_TO_WINDOWS_MANAGER_1(VOID, createSceneWithFloor, STRING)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, addSceneToWindow, STRING, WINDOW_ID)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, attachCameraToNode, STRING, WINDOW_ID)

      BIND_TO_WINDOWS_MANAGER_1(BOOL, detachCamera, WINDOW_ID)

      BIND_TO_WINDOWS_MANAGER_1(BOOL, nodeExists, STRING)

      BIND_TO_WINDOWS_MANAGER_1(BOOL, addFloor, STRING)

      BIND_TO_WINDOWS_MANAGER_5(BOOL, addBox    , STRING, FLOAT, FLOAT, FLOAT, COLOR)

      BIND_TO_WINDOWS_MANAGER_4(BOOL, addCapsule, STRING, FLOAT, FLOAT, COLOR)

      BIND_TO_WINDOWS_MANAGER_4(BOOL, addArrow  , STRING, FLOAT, FLOAT, COLOR)

      BIND_TO_WINDOWS_MANAGER_5(BOOL, addRod    , STRING, COLOR, FLOAT, FLOAT, SHORT)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, resizeCapsule, STRING, FLOAT)

      BIND_TO_WINDOWS_MANAGER_3(BOOL, resizeArrow, STRING, FLOAT, FLOAT)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, addMesh, STRING, STRING)

      BIND_TO_WINDOWS_MANAGER_1(VOID, removeLightSources, STRING)

      BIND_TO_WINDOWS_MANAGER_4(BOOL, addCone, STRING, FLOAT, FLOAT, COLOR)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, setTexture, STRING, STRING)

      BIND_TO_WINDOWS_MANAGER_4(BOOL, addCylinder, STRING, FLOAT, FLOAT, COLOR)

      BIND_TO_WINDOWS_MANAGER_3(BOOL, addSphere, STRING, FLOAT, COLOR)

      BIND_TO_WINDOWS_MANAGER_4(BOOL, addLight, STRING, WINDOW_ID, FLOAT, COLOR)

      BIND_TO_WINDOWS_MANAGER_4(BOOL, addLine, STRING, POSITION, POSITION, COLOR)
      
      BIND_TO_WINDOWS_MANAGER_2(BOOL, setLineStartPoint, STRING, POSITION)
      BIND_TO_WINDOWS_MANAGER_2(BOOL, setLineEndPoint, STRING, POSITION)
      BIND_TO_WINDOWS_MANAGER_3(BOOL, setLineExtremalPoints, STRING, POSITION, POSITION)

      BIND_TO_WINDOWS_MANAGER_3(BOOL, addCurve, STRING, POSITION_SEQ, COLOR)
      BIND_TO_WINDOWS_MANAGER_2(BOOL, setCurvePoints, STRING, POSITION_SEQ)
      BIND_TO_WINDOWS_MANAGER_2(BOOL, setCurveColors, STRING, COLOR_SEQ)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, setCurveMode, STRING, GLMODE)

      BIND_TO_WINDOWS_MANAGER_3(BOOL, setCurvePointsSubset, STRING, LONG, LONG)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, setCurveLineWidth, STRING, FLOAT)

      BIND_TO_WINDOWS_MANAGER_5(BOOL, addTriangleFace, STRING, POSITION, POSITION, POSITION, COLOR)

      BIND_TO_WINDOWS_MANAGER_6(BOOL, addSquareFace, STRING, POSITION, POSITION, POSITION, POSITION, COLOR)

      BIND_TO_WINDOWS_MANAGER_4(BOOL, addXYZaxis, STRING, COLOR, FLOAT, FLOAT)

      /** initialise the roadmap (graphical roadmap)*/
      BIND_TO_WINDOWS_MANAGER_5(BOOL, createRoadmap, STRING, COLOR, FLOAT, FLOAT, COLOR)

      BIND_TO_WINDOWS_MANAGER_3(BOOL, addEdgeToRoadmap, STRING, POSITION, POSITION)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, addNodeToRoadmap, STRING, TRANSFORM)

      BIND_TO_WINDOWS_MANAGER_0(STRING_LIST, getNodeList)
      BIND_TO_WINDOWS_MANAGER_1(STRING_LIST, getGroupNodeList, STRING)
      BIND_TO_WINDOWS_MANAGER_0(STRING_LIST, getSceneList)
      BIND_TO_WINDOWS_MANAGER_0(STRING_LIST, getWindowList)

      BIND_TO_WINDOWS_MANAGER_1(BOOL, createGroup, STRING)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, addURDF, STRING, STRING)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, addUrdfCollision, STRING, STRING)

      BIND_TO_WINDOWS_MANAGER_3(VOID, addUrdfObjects, STRING, STRING, BOOL)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, addToGroup, STRING, STRING)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, removeFromGroup, STRING, STRING)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, applyConfiguration, STRING, TRANSFORM)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, applyConfigurations, STRING_LIST, TRANSFORM_SEQ)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, addLandmark, STRING, FLOAT)

      BIND_TO_WINDOWS_MANAGER_1(BOOL, deleteLandmark, STRING)

      BIND_TO_WINDOWS_MANAGER_1(TRANSFORM, getStaticTransform, STRING)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, setStaticTransform, STRING, TRANSFORM)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, setVisibility, STRING, STRING)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, setScale, STRING, POSITION)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, setColor, STRING, COLOR)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, setWireFrameMode, STRING, STRING)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, setLightingMode, STRING, STRING)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, setHighlight, STRING, LONG)

      BIND_TO_WINDOWS_MANAGER_2(VOID, captureFrame, WINDOW_ID, STRING)

      BIND_TO_WINDOWS_MANAGER_3(BOOL, startCapture, WINDOW_ID, STRING, STRING)

      BIND_TO_WINDOWS_MANAGER_1(BOOL, stopCapture, WINDOW_ID)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, setCaptureTransform, STRING, STRING_LIST)

      BIND_TO_WINDOWS_MANAGER_1(VOID, captureTransformOnRefresh, BOOL)

      BIND_TO_WINDOWS_MANAGER_0(VOID, captureTransform)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, writeBlenderScript, STRING, STRING_LIST)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, writeNodeFile, STRING, STRING)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, writeWindowFile, WINDOW_ID, STRING)
      
      BIND_TO_WINDOWS_MANAGER_1(TRANSFORM, getNodeGlobalTransform, STRING)

      BIND_TO_WINDOWS_MANAGER_2(VOID, deleteNode, STRING, BOOL)

      BIND_TO_WINDOWS_MANAGER_2(BOOL, setBackgroundColor1, WINDOW_ID, COLOR)
      
      BIND_TO_WINDOWS_MANAGER_2(BOOL, setBackgroundColor2, WINDOW_ID, COLOR)

      BIND_TO_WINDOWS_MANAGER_1(TRANSFORM,getCameraTransform,WINDOW_ID)

      BIND_TO_WINDOWS_MANAGER_2(BOOL,setCameraTransform,WINDOW_ID,TRANSFORM)

      // ------------- Properties -------------------- //

      BIND_TO_WINDOWS_MANAGER_1(STRING_LIST,getPropertyNames,STRING)
      BIND_TO_WINDOWS_MANAGER_1(STRING_LIST,getPropertyTypes,STRING)

      BIND_TO_WINDOWS_MANAGER_2(STRING,getStringProperty,STRING,STRING)

      BIND_TO_WINDOWS_MANAGER_3(VOID,setStringProperty,STRING,STRING,STRING)

      BIND_TO_WINDOWS_MANAGER_2(POSITION,getVector3Property,STRING,STRING)

      BIND_TO_WINDOWS_MANAGER_3(VOID,setVector3Property,STRING,STRING,POSITION)

      BIND_TO_WINDOWS_MANAGER_2(VECTOR2,getVector2Property,STRING,STRING)

      BIND_TO_WINDOWS_MANAGER_3(VOID,setVector2Property,STRING,STRING,VECTOR2)

      BIND_TO_WINDOWS_MANAGER_2(COLOR,getColorProperty,STRING,STRING)

      BIND_TO_WINDOWS_MANAGER_3(VOID,setColorProperty,STRING,STRING,COLOR)

      BIND_TO_WINDOWS_MANAGER_2(FLOAT,getFloatProperty,STRING,STRING)

      BIND_TO_WINDOWS_MANAGER_3(VOID,setFloatProperty,STRING,STRING,FLOAT)

      BIND_TO_WINDOWS_MANAGER_2(BOOL,getBoolProperty,STRING,STRING)

      BIND_TO_WINDOWS_MANAGER_3(VOID,setBoolProperty,STRING,STRING,BOOL)

      BIND_TO_WINDOWS_MANAGER_2(LONG,getIntProperty,STRING,STRING)

      BIND_TO_WINDOWS_MANAGER_3(VOID,setIntProperty,STRING,STRING,LONG)

      BIND_TO_WINDOWS_MANAGER_2(VOID,callVoidProperty,STRING,STRING)

      // ------------- Callbacks -------------------- //
      bool GraphicalInterface::registerNodeCallback (NodeCallback_ptr _cb)
      {
        corbaserver::NodeCallback_var cb(corbaserver::NodeCallback::_duplicate(_cb));
        new NodeCallback (qparent_, cb);
        return true;
      }
    } //end namespace impl
  } //end namespace corba
  } //end namespace viewer
} //end namespace graphics
