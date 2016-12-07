// Copyright (c) 2014, LAAS-CNRS
// Authors: Joseph Mirabel (joseph.mirabel@laas.fr)
//
// This file is part of gepetto-viewer.
// gepetto-viewer is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
//
// gepetto-viewer is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// gepetto-viewer. If not, see <http://www.gnu.org/licenses/>.

#ifndef SCENEVIEWER_WINDOWMANAGERS_HH
#define SCENEVIEWER_WINDOWMANAGERS_HH

#include <gepetto/viewer/window-manager.h>
#include <gepetto/viewer/roadmap-viewer.h>
#include <gepetto/viewer/transform-writer.h>
#include "gepetto/viewer/corba/graphical-interface.hh"
#include <boost/thread/mutex.hpp>

namespace graphics {

    struct Configuration {
        osgVector3 position;
        osgQuat quat;
        Configuration() {}
        /// \param XYZW when false, the 4 last parameters are a quaternion (w,x,y,z)
        ///             otherwise, a quaternion (x,y,z,w)
        explicit Configuration(const float* a, bool XYZW)
          : position(a[0],a[1],a[2])
          , quat(a[(XYZW ? 3 : 4)],
                 a[(XYZW ? 4 : 5)],
                 a[(XYZW ? 5 : 6)],
                 a[(XYZW ? 6 : 3)]) {}
        Configuration(const osgVector3& p, const osgQuat& q) : position(p), quat(q) {}
    };
    struct NodeConfiguration : Configuration {
        NodePtr_t node;
    };

    struct BlenderFrameCapture {
      typedef std::vector<NodePtr_t> Nodes_t;
      osg::ref_ptr < TransformWriterVisitor > writer_visitor_;
      Nodes_t nodes_;
      BlenderFrameCapture ()
        : writer_visitor_ (new TransformWriterVisitor (
              new YamlTransformWriter ("gepetto_viewer.yaml")))
        , nodes_ ()
      {}
      void captureFrame () {
        using std::invalid_argument;
        if (!writer_visitor_)
          throw invalid_argument ("Capture writer not defined");
        if (nodes_.empty()) throw invalid_argument ("No node to capture");
        writer_visitor_->captureFrame (nodes_.begin(), nodes_.end());
      }
    };

    struct UrdfFile {
      std::string filename;
      time_t modTime;
      UrdfFile (const std::string& f);
      UrdfFile () {}
    };

    DEF_CLASS_SMART_PTR(WindowsManager)

    class WindowsManager
    {
        public:
            // Typedef for position and color values.
            typedef osg::Vec3f::value_type value_type;
            typedef osgVector4 Color_t;
            typedef ::osg::Vec3ArrayRefPtr Vec3ArrayPtr_t;
            typedef unsigned int WindowID;

            typedef std::map <std::string, WindowID> WindowIDMap_t;
            WindowIDMap_t windowIDmap_;

            typedef std::map <std::string, UrdfFile> UrdfFileMap_t;
            UrdfFileMap_t urdfFileMap_;

        private:
            typedef std::vector <WindowManagerPtr_t> WindowManagerVector_t;
            typedef std::vector<NodeConfiguration> NodeConfigurations_t;
            WindowManagerVector_t windowManagers_;
            std::map<std::string, NodePtr_t> nodes_;
            std::map<std::string, GroupNodePtr_t> groupNodes_;
            std::map<std::string, RoadmapViewerPtr_t> roadmapNodes_;
            boost::mutex mtx_;
            int rate_;
            NodeConfigurations_t newNodeConfigurations_;
            BlenderFrameCapture blenderCapture_;
            bool autoCaptureTransform_;

            static osgVector4 getColor(const std::string& colorName);
            static std::string parentName(const std::string& name);
            static VisibilityMode getVisibility(const std::string& visibilityName);
            static WireFrameMode getWire(const std::string& wireName);
            static LightingMode getLight(const std::string& lightName);
            NodePtr_t find (const std::string name, GroupNodePtr_t group = GroupNodePtr_t());
            void initParent(NodePtr_t node, GroupNodePtr_t parent);
            void threadRefreshing(WindowManagerPtr_t window);
            bool urdfUpToDate (const std::string nodeName,
                const std::string filename);
            void registerUrdfNode (const std::string nodeName,
                const std::string filename);
            bool urdfNodeMustBeAdded (const std::string& nodeName,
                const std::string& filename);

        protected:
            /**
              \brief Default constructor
              */
            WindowsManager ();
            WindowID addWindow (std::string winName, WindowManagerPtr_t newWindow);

            template <typename NodeContainer_t>
              std::size_t getNodes
              (const gepetto::corbaserver::Names_t& name, NodeContainer_t& nodes);
            template <typename Iterator, typename NodeContainer_t> 
              std::size_t getNodes
              (const Iterator& begin, const Iterator& end, NodeContainer_t& nodes);

            /// Warning, the mutex should be locked before and unlocked after this opertations.
            void addNode (const std::string& nodeName, NodePtr_t node, bool guessParent = false);
            virtual void addNode (const std::string& nodeName, NodePtr_t node, GroupNodePtr_t parent);
            void addGroup(const std::string& groupName, GroupNodePtr_t group, bool guessParent = false);
            virtual void addGroup(const std::string& groupName, GroupNodePtr_t group, GroupNodePtr_t parent);

        public:
            static WindowsManagerPtr_t create ();

            virtual ~WindowsManager () {};

            virtual std::vector<std::string> getNodeList();
            virtual std::vector<std::string> getGroupNodeList(const std::string& group);
            virtual std::vector<std::string> getSceneList();
            virtual std::vector<std::string> getWindowList();

            /// Return the mutex to be locked before refreshing
            boost::mutex& lock () {
              return mtx_;
            }

            virtual bool setRate(const int& rate);
            virtual void refresh();

            virtual WindowID createWindow(const std::string& windowName);
            virtual WindowID getWindowID (const std::string& windowName);

            virtual void createScene(const std::string& sceneName);
            virtual void createSceneWithFloor(const std::string& sceneName);
            virtual bool addSceneToWindow(const std::string& sceneName, const WindowID windowId);

            virtual bool attachCameraToNode(const std::string& nodeName, const WindowID windowId);
            virtual bool detachCamera(const WindowID windowId);

            virtual bool nodeExists (const std::string& name);

            virtual bool addFloor(const std::string& floorName);

            virtual bool addBox(const std::string& boxName, const float& boxSize1, const float& boxSize2, const float& boxSize3, const Color_t& color);

            virtual bool addCapsule(const std::string& capsuleName, float radius, float height, const Color_t& color);

            virtual  bool addArrow (const std::string& arrowName,const float radius, const float length,  const Color_t& color);

            virtual bool addRod (const std::string& rodName, const Color_t& color,const float radius,const float length, short maxCapsule);

            virtual bool resizeCapsule(const std::string& capsuleName, float newHeight) throw (std::exception);
            virtual bool resizeArrow(const std::string& arrowName ,float newRadius, float newLength) throw(std::exception);

            virtual bool addMesh(const std::string& meshName, const std::string& meshPath);

            virtual bool addCone(const std::string& coneName, float radius, float height, const Color_t& color);

            virtual bool addCylinder(const std::string& cylinderName, float radius, float height, const Color_t& color);

            virtual bool addSphere(const std::string& sphereName, float radius, const Color_t& color);

            virtual bool addLight(const std::string& lightName, const WindowID wid, float radius, const Color_t& color);

            virtual bool addLine(const std::string& lineName, const osgVector3& pos1, const osgVector3& pos2, const Color_t& color);

            virtual bool addCurve(const std::string& curveName, const Vec3ArrayPtr_t& pos, const Color_t& color);

            /// See http://svn.openscenegraph.org/osg/OpenSceneGraph-Data/trunk/Images/primitives.gif for
            /// possible values
            virtual bool setCurveMode (const std::string& curveName, const GLenum mode);

            virtual bool addSquareFace(const std::string& faceName, const osgVector3& pos1, const osgVector3& pos2, const osgVector3& pos3, const osgVector3& pos4, const Color_t& color);
            virtual bool setTexture (const std::string& nodeName, const std::string& filename);
            virtual bool addTriangleFace(const std::string& faceName, const osgVector3& pos1, const osgVector3& pos2, const osgVector3& pos3, const Color_t& color);
            virtual bool addXYZaxis (const std::string& nodeName, const Color_t& color, float radius, float sizeAxis);

            virtual bool createRoadmap(const std::string& name,const Color_t& colorNode, float radius, float sizeAxis, const Color_t& colorEdge);

            virtual bool addEdgeToRoadmap(const std::string& nameRoadmap, const osgVector3& posFrom, const osgVector3& posTo);

            virtual bool addNodeToRoadmap(const std::string& nameRoadmap, const Configuration& configuration);

            virtual bool addURDF(const std::string& urdfName, const std::string& urdfPath);
            /// \deprecated Argument urdfPackagePathCorba is ignored.
            virtual bool addURDF(const std::string& urdfName, const std::string& urdfPath, const std::string& urdfPackagePath);

            virtual bool addUrdfCollision (const std::string& urdfName,
                    const std::string& urdfPath) ;
            /// \deprecated Argument urdfPackagePathCorba is ignored.
            virtual bool addUrdfCollision (const std::string& urdfName,
                    const std::string& urdfPath,
                    const std::string& urdfPackagePath) ;

            virtual void addUrdfObjects (const std::string& urdfName,
                    const std::string& urdfPath,
                    bool visual) ;
            /// \deprecated Argument urdfPackagePathCorba is ignored.
            virtual void addUrdfObjects (const std::string& urdfName,
                    const std::string& urdfPath,
                    const std::string& urdfPackagePath,
                    bool visual) ;

            virtual bool createGroup(const std::string& groupName);
            virtual bool addToGroup(const std::string& nodeName, const std::string& groupName);
            virtual bool removeFromGroup (const std::string& nodeName, const std::string& groupName);
            virtual bool deleteNode (const std::string& nodeName, bool all);

            virtual bool applyConfiguration(const std::string& nodeName, const Configuration& configuration);

            virtual bool addLandmark(const std::string& nodeName, float size);
            virtual bool deleteLandmark(const std::string& nodeName);

            virtual Configuration getStaticTransform (const std::string& nodeName) const;
            virtual bool setStaticTransform (const std::string& nodeName, const Configuration& transform);

            virtual bool setVisibility(const std::string& nodeName, const std::string& visibilityMode);
            virtual bool setScale(const std::string& nodeName, const osgVector3& scale);
            virtual bool setScale(const std::string& nodeName, const float& scale);
            virtual bool setScale(const std::string& nodeName, const int& scalePercentage);
            virtual bool setColor(const std::string& nodeName, const Color_t& color);
            virtual bool setWireFrameMode(const std::string& nodeName, const std::string& wireFrameMode);
            virtual bool setLightingMode(const std::string& nodeName, const std::string& lightingMode);
            virtual bool setHighlight(const std::string& nodeName, int state);
            virtual bool setAlpha(const std::string& nodeName, const float& alpha);
            virtual bool setAlpha(const std::string& nodeName, const int& alphaPercentage);

            virtual bool startCapture (const WindowID windowId, const std::string& filename,
                    const std::string& extension);
            virtual bool stopCapture (const WindowID windowId);
            virtual bool setCaptureTransform (const std::string& filename, const std::vector<std::string>& nodename);
            virtual void captureTransformOnRefresh (bool autoCapture);
            virtual void captureTransform ();
            virtual bool writeBlenderScript (const std::string& filename, const std::vector<std::string>& nodename);
            virtual bool writeNodeFile (const std::string& nodename, const std::string& filename);
            virtual bool writeWindowFile (const WindowID windowId, const std::string& filename);
            virtual bool setBackgroundColor1(const WindowID windowId,const Color_t& color);
            virtual bool setBackgroundColor2(const WindowID windowId,const Color_t& color);
            

            WindowManagerPtr_t getWindowManager (const WindowID wid, bool throwIfDoesntExist = false) const;
            GroupNodePtr_t getGroup (const std::string groupName, bool throwIfDoesntExist = false) const;
            NodePtr_t getNode (const std::string& nodeName, bool throwIfDoesntExist = false) const;
            Configuration getNodeGlobalTransform(const std::string nodeName) const;
    };
} /* namespace graphics */

#endif /* SCENEVIEWER_WINDOWMANAGERS_HH */
