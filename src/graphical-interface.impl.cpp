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
      namespace {
        typedef GraphicalInterface::WindowID WindowID;
        using gepetto::corbaserver::Names_t;
        using gepetto::corbaserver::Transform_slice;

        template <typename Input, typename Output>
          void to (const Input& in, Output& out) {
            for (CORBA::ULong i = 0; i < in.length(); ++i)
              out.push_back ((typename Output::value_type)in[i]);
          }

        template <typename Input, typename Output>
          void to (const Input& in, Output& out, const int size) {
            for (CORBA::ULong i = 0; i < (CORBA::ULong)size; ++i)
              out[i] = in[i];
          }

        template <typename Iterator>
        Names_t* toNames_t(Iterator begin, const Iterator& end)
        {
          ULong size = (ULong) std::distance (begin, end);
          char** nameList = Names_t::allocbuf(size);
          Names_t *names = new Names_t (size, size, nameList);
          for (ULong i = 0; i < size; ++i)
          {
            const std::string& name = *begin;
            nameList [i] = (char*) malloc (sizeof(char)*(name.length ()+1));
            strcpy (nameList [i], name.c_str ());
            ++begin;
          }
          return names;
        }

        enum ArgType { STRING, STRING_LIST, OUT_STRING_LIST, COLOR,
          TRANSFORM, TRANSFORM_SEQ, POSITION, POSITION_SEQ,
          FLOAT, SHORT, LONG, WINDOW_ID, BOOL, VOID,
          GLMODE
        };

        template <int what> struct traits {};
        template <> struct traits<COLOR> {
          typedef       WindowsManager::Color_t   Out_t;
          typedef const GraphicalInterface::Color In_t;
          typedef GraphicalInterface::Color_slice* Ret_t;
          static Out_t op (In_t color) { return Out_t (color[0], color[1], color[2], color[3]); }
          static Ret_t ret (Out_t in) { Ret_t r = gepetto::corbaserver::Color_alloc(); to(in, r, 4); return r; }
        };
        template <> struct traits<TRANSFORM> {
          typedef const GraphicalInterface::Transform  In_t;
          typedef       Configuration  Out_t;
          typedef       Transform_slice* Ret_t;
          static Out_t op  (In_t in) { return Out_t(in, true); /* true = (x,y,z,w) -> (x,y,z,w), false = (w,x,y,z) -> (x,y,z,w) */ }
          static Ret_t ret (const Out_t& in) {
            Ret_t ret = new GraphicalInterface::Transform();
            for(int i=0; i<3; i++) ret[(ULong)i]   = in.position[i];
            for(int i=0; i<4; i++) ret[(ULong)i+3] = (float)in.quat[i];
            return ret;
          }
        };
        template <> struct traits<TRANSFORM_SEQ> {
          typedef const GraphicalInterface::TransformSeq& In_t;
          typedef       std::vector<Configuration>        Out_t;
          static Out_t op  (In_t in) {
            Out_t out (in.length());
            for (CORBA::ULong i = 0; i < in.length (); ++i)
              out[i] = traits<TRANSFORM>::op (in[i]);
            return out;
          }
        };
        template <> struct traits<POSITION> {
          typedef       osgVector3    Out_t;
          typedef const GraphicalInterface::Position In_t;
          typedef GraphicalInterface::Color_slice* Ret_t;
          static Out_t op (In_t pos) { return Out_t (pos[0], pos[1], pos[2]); }
          static Ret_t ret (Out_t in) { Ret_t r = gepetto::corbaserver::Position_alloc(); to(in, r, 3); return r; }
        };
        template <> struct traits<POSITION_SEQ> {
          typedef const GraphicalInterface::PositionSeq& In_t;
          typedef       ::osg::Vec3ArrayRefPtr           Out_t;
          static inline Out_t op (In_t in) {
            Out_t out = new ::osg::Vec3Array;
            for (CORBA::ULong i = 0; i < in.length (); ++i)
              out->push_back (::osg::Vec3 (in[i][0],in[i][1],in[i][2]));
            return out;
          }
        };
        template <> struct traits<STRING> {
          typedef std::string Out_t;
          typedef const char* In_t;
          typedef char* Ret_t;
          static Out_t op (In_t in) { return Out_t (in); }
          static Ret_t ret (const Out_t& in) { char *cstr = new char[in.length() + 1]; strcpy(cstr, in.c_str()); return cstr; }
        };
        template <> struct traits<STRING_LIST> {
          typedef std::vector<std::string> Out_t;
          typedef const Names_t& In_t;
          typedef Names_t* Ret_t;
          static Out_t op (In_t in) { Out_t nodes; to (in, nodes); return nodes; }
          static Ret_t ret (const Out_t& in) { return toNames_t(in.begin(), in.end()); }
        };
        template <> struct traits<FLOAT> {
          typedef const float& Out_t;
          typedef const float  In_t;
          typedef float  Ret_t;
          typedef const float& OpIn_t;
          static const float& op (const float& in) { return in; }
          static       float& op (      float& in) { return in; }
          static Ret_t ret (Out_t  in) { return in; }
        };
        template <> struct traits<SHORT> {
          typedef const short& Out_t;
          typedef const short  In_t;
          static const short& op (const short& in) { return in; }
          static       short& op (      short& in) { return in; }
        };
        template <> struct traits<LONG> {
          typedef int Out_t;
          typedef CORBA::Long  In_t;
          typedef CORBA::Long  Ret_t;
          static int op (const In_t& in) { return in; }
          static Ret_t ret (Out_t  in) { return in; }
        };
        template <> struct traits<WINDOW_ID> {
          typedef       WindowsManager::WindowID  Out_t;
          typedef const WindowID  In_t;
          typedef       WindowID  Ret_t;
          static Out_t op  (In_t & in) { return in; }
          static Ret_t ret (Out_t  in) { return in; }
        };
        template <> struct traits<BOOL> {
          typedef bool In_t;
          typedef In_t Out_t;
          typedef In_t Ret_t;
          static Out_t op (In_t in) { return in; }
          static Ret_t ret (Out_t in) { return in; }
        };
        template <> struct traits<VOID> {
          typedef void Ret_t;
        };
        template <> struct traits<GLMODE> {
          typedef       GLenum Out_t;
          typedef const char*  In_t;
          static Out_t op (In_t modeName) {
              if      (strcasecmp (modeName, "lines")          == 0) return GL_LINES;
              else if (strcasecmp (modeName, "points")         == 0) return GL_POINTS;
              else if (strcasecmp (modeName, "line_strip")     == 0) return GL_LINE_STRIP;
              else if (strcasecmp (modeName, "line_loop")      == 0) return GL_LINE_LOOP;
              else if (strcasecmp (modeName, "polygon")        == 0) return GL_POLYGON;
              else if (strcasecmp (modeName, "quads")          == 0) return GL_QUADS;
              else if (strcasecmp (modeName, "quad_strip")     == 0) return GL_QUAD_STRIP;
              else if (strcasecmp (modeName, "triangle_strip") == 0) return GL_TRIANGLE_STRIP;
              else if (strcasecmp (modeName, "triangles")      == 0) return GL_TRIANGLES;
              else if (strcasecmp (modeName, "triangle_fan")   == 0) return GL_TRIANGLE_FAN;
              else throw gepetto::Error ("Unknown mode name");
        }
        };
      }

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
      WRITE_RET_T(Ret) GraphicalInterface::Func (                  ) throw (Error)          \
      {                                                                        \
          try {                                                                \
            WRITE_RET_OP(Ret)( windowsManager_->Func (                ));                  \
          } catch (const std::exception& exc) {                                \
              throw Error (exc.what ());                                       \
          }                                                                    \
      }

#define BIND_TO_WINDOWS_MANAGER_1(Ret, Func, T0)                             \
      WRITE_RET_T(Ret) GraphicalInterface::Func (WRITE_INPUT_ARGS_1(T0)) throw (Error)          \
      {                                                                        \
          try {                                                                \
            WRITE_RET_OP(Ret)( windowsManager_->Func ( WRITE_OP_ARGS_1(T0) ));                  \
          } catch (const std::exception& exc) {                                \
              throw Error (exc.what ());                                       \
          }                                                                    \
      }

#define BIND_TO_WINDOWS_MANAGER_2(Ret, Func, T0, T1)     \
      WRITE_RET_T(Ret) GraphicalInterface::Func (WRITE_INPUT_ARGS_2(T0, T1)) throw (Error)          \
      {                                                                        \
          try {                                                                \
            WRITE_RET_OP(Ret)( windowsManager_->Func ( WRITE_OP_ARGS_2(T0, T1) ));\
          } catch (const std::exception& exc) {                                \
              throw Error (exc.what ());                                       \
          }                                                                    \
      }

#define BIND_TO_WINDOWS_MANAGER_3(Ret, Func, T0, T1, T2)     \
      WRITE_RET_T(Ret) GraphicalInterface::Func (WRITE_INPUT_ARGS_3(T0, T1, T2)) throw (Error)          \
      {                                                                        \
          try {                                                                \
            WRITE_RET_OP(Ret)( windowsManager_->Func ( WRITE_OP_ARGS_3(T0, T1, T2) ));\
          } catch (const std::exception& exc) {                                \
              throw Error (exc.what ());                                       \
          }                                                                    \
      }

#define BIND_TO_WINDOWS_MANAGER_4(Ret, Func, T0, T1, T2, T3)     \
      WRITE_RET_T(Ret) GraphicalInterface::Func (WRITE_INPUT_ARGS_4(T0, T1, T2, T3)) throw (Error)          \
      {                                                                        \
          try {                                                                \
            WRITE_RET_OP(Ret)( windowsManager_->Func ( WRITE_OP_ARGS_4(T0, T1, T2, T3) ));\
          } catch (const std::exception& exc) {                                \
              throw Error (exc.what ());                                       \
          }                                                                    \
      }

#define BIND_TO_WINDOWS_MANAGER_5(Ret, Func, T0, T1, T2, T3, T4)     \
      WRITE_RET_T(Ret) GraphicalInterface::Func (WRITE_INPUT_ARGS_5(T0, T1, T2, T3, T4)) throw (Error)          \
      {                                                                        \
          try {                                                                \
            WRITE_RET_OP(Ret)( windowsManager_->Func ( WRITE_OP_ARGS_5(T0, T1, T2, T3, T4) ));\
          } catch (const std::exception& exc) {                                \
              throw Error (exc.what ());                                       \
          }                                                                    \
      }

#define BIND_TO_WINDOWS_MANAGER_6(Ret, Func, T0,T1,T2,T3,T4,T5)     \
      WRITE_RET_T(Ret) GraphicalInterface::Func (WRITE_INPUT_ARGS_6(T0,T1,T2,T3,T4,T5)) throw (Error)          \
      {                                                                        \
          try {                                                                \
            WRITE_RET_OP(Ret)( windowsManager_->Func ( WRITE_OP_ARGS_6(T0,T1,T2,T3,T4,T5) ));\
          } catch (const std::exception& exc) {                                \
              throw Error (exc.what ());                                       \
          }                                                                    \
      }

      GraphicalInterface::GraphicalInterface (corbaServer::Server* server) :
	windowsManager_ (server->windowsManager ())
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

      BIND_TO_WINDOWS_MANAGER_3(BOOL, addURDF, STRING, STRING, STRING)

      BIND_TO_WINDOWS_MANAGER_3(BOOL, addUrdfCollision, STRING, STRING, STRING)

      BIND_TO_WINDOWS_MANAGER_4(VOID, addUrdfObjects, STRING, STRING, STRING, BOOL)

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

      BIND_TO_WINDOWS_MANAGER_2(COLOR,getColorProperty,STRING,STRING)

      BIND_TO_WINDOWS_MANAGER_3(VOID,setColorProperty,STRING,STRING,COLOR)

      BIND_TO_WINDOWS_MANAGER_2(FLOAT,getFloatProperty,STRING,STRING)

      BIND_TO_WINDOWS_MANAGER_3(VOID,setFloatProperty,STRING,STRING,FLOAT)

      BIND_TO_WINDOWS_MANAGER_2(BOOL,getBoolProperty,STRING,STRING)

      BIND_TO_WINDOWS_MANAGER_3(VOID,setBoolProperty,STRING,STRING,BOOL)

      BIND_TO_WINDOWS_MANAGER_2(LONG,getIntProperty,STRING,STRING)

      BIND_TO_WINDOWS_MANAGER_3(VOID,setIntProperty,STRING,STRING,LONG)
    } //end namespace impl
  } //end namespace corbaServer
} //end namespace graphics
