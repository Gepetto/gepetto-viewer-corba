//
//  GraphicalInterface.impl.cc
//  Corbaserver for gepetto-viewer
//
//  Author: Mathieu Geisert, Florent Lamiraux
//  Copyright (c) 2014 LAAS-CNRS. All rights reserved.
//

#include "graphical-interface.impl.hh"

namespace gepetto {
  namespace viewer {
  namespace corba {
    namespace impl {
        typedef GraphicalInterface::WindowID WindowID;

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
          typedef const Color In_t;
          typedef Color_slice* Ret_t;
          static Out_t op (In_t color) { return Out_t (color[0], color[1], color[2], color[3]); }
          static Ret_t ret (Out_t in) { Ret_t r = gepetto::corbaserver::Color_alloc(); to(in, r, 4); return r; }
        };
        template <> struct traits<TRANSFORM> {
          typedef const Transform  In_t;
          typedef       Configuration  Out_t;
          typedef       Transform_slice* Ret_t;
          static Out_t op  (In_t in) { return Out_t(in, true); /* true = (x,y,z,w) -> (x,y,z,w), false = (w,x,y,z) -> (x,y,z,w) */ }
          static Ret_t ret (const Out_t& in) {
            Ret_t ret = new Transform();
            for(int i=0; i<3; i++) ret[(ULong)i]   = in.position[i];
            for(int i=0; i<4; i++) ret[(ULong)i+3] = (float)in.quat[i];
            return ret;
          }
        };
        template <> struct traits<TRANSFORM_SEQ> {
          typedef const TransformSeq& In_t;
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
          typedef const Position In_t;
          typedef Color_slice* Ret_t;
          static Out_t op (In_t pos) { return Out_t (pos[0], pos[1], pos[2]); }
          static Ret_t ret (Out_t in) { Ret_t r = gepetto::corbaserver::Position_alloc(); to(in, r, 3); return r; }
        };
        template <> struct traits<POSITION_SEQ> {
          typedef const PositionSeq& In_t;
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
    } //end namespace impl
  } //end namespace corba
  } //end namespace viewer
} //end namespace graphics
