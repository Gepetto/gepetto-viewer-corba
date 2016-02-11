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

    struct NodeConfiguration {
        NodePtr_t node;
        osgVector3 position;
        osgQuat quat;
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
            typedef std::vector<float> configuration_t;
            typedef unsigned int WindowID;

            typedef std::map <std::string, WindowID> WindowIDMap_t;
            WindowIDMap_t windowIDmap_;

            typedef std::map <std::string, UrdfFile> UrdfFileMap_t;
            UrdfFileMap_t urdfFileMap_;

            typedef gepetto::corbaserver::PositionSeq PositionSeq;

        private:
            typedef std::vector <WindowManagerPtr_t> WindowManagerVector_t;
            WindowManagerVector_t windowManagers_;
            std::map<std::string, NodePtr_t> nodes_;
            std::map<std::string, GroupNodePtr_t> groupNodes_;
            std::map<std::string, RoadmapViewerPtr_t> roadmapNodes_;
            boost::mutex mtx_;
            int rate_;
            std::list<NodeConfiguration> newNodeConfigurations_;
            BlenderFrameCapture blenderCapture_;
            bool autoCaptureTransform_;

            static osgVector4 getColor(const std::string& colorName);
            static osgVector4 getColor(const float* color);
            static std::string parentName(const std::string& name);
            static VisibilityMode getVisibility(const std::string& visibilityName);
            static WireFrameMode getWire(const std::string& wireName);
            static LightingMode getLight(const std::string& lightName);
            NodePtr_t find (const std::string name, GroupNodePtr_t group = GroupNodePtr_t());
            void initParent(const std::string& nodeName, NodePtr_t node);
            void addNode(const std::string& nodeName, NodePtr_t node);
            void addGroup(const std::string& groupName, GroupNodePtr_t group);
            void threadRefreshing(WindowManagerPtr_t window);
            static osgQuat corbaConfToOsgQuat(const value_type* configurationCorba);
            static osgVector3 corbaConfToOsgVec3(const value_type* configurationCorba);
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

            bool nodeExists (const std::string& name);
            template <typename NodeContainer_t>
              std::size_t getNodes
              (const gepetto::corbaserver::Names_t& name, NodeContainer_t& nodes);
            template <typename Iterator, typename NodeContainer_t> 
              std::size_t getNodes
              (const Iterator& begin, const Iterator& end, NodeContainer_t& nodes);

        public:
            static WindowsManagerPtr_t create ();

            virtual std::vector<std::string> getNodeList();
            virtual std::vector<std::string> getGroupNodeList(const char* group);
            virtual std::vector<std::string> getSceneList();
            virtual std::vector<std::string> getWindowList();

            /// Return the mutex to be locked before refreshing
            boost::mutex& lock () {
              return mtx_;
            }

            virtual bool setRate(const int& rate);
            virtual void refresh();

            virtual WindowID createWindow(const char* windowNameCorba);
            virtual WindowID getWindowID (const char* windowNameCorba);

            virtual void createScene(const char* sceneNameCorba);
            virtual void createSceneWithFloor(const char* sceneNameCorba);
            virtual bool addSceneToWindow(const char* sceneNameCorba, const WindowID windowId);


            virtual bool addFloor(const char* floorNameCorba);

            virtual bool addBox(const char* boxName, float boxSize1, float boxSize2, float boxSize3, const value_type* color);

            virtual bool addCapsule(const char* capsuleName, float radius, float height, const value_type* color);

            virtual  bool addArrow (const char* arrowNameCorba,const float radius, const float length,  const value_type* colorCorba);

            virtual bool addRod (const char* rodNameCorba, const value_type* colorCorba,const float radius,const float length, short maxCapsule);

            virtual bool resizeCapsule(const char* capsuleNameCorba, float newHeight) throw (std::exception);
            virtual bool resizeArrow(const char* arrowNameCorba ,float newRadius, float newLength) throw(std::exception);

            virtual bool addMesh(const char* meshNameCorba, const char* meshPathCorba);

            virtual bool addCone(const char* coneName, float radius, float height, const value_type* color);

            virtual bool addCylinder(const char* cylinderName, float radius, float height, const value_type* color);

            virtual bool addSphere(const char* sphereName, float radius, const value_type* color);

            virtual bool addLight(const char* lightName, const WindowID wid, float radius, const value_type* color);

            virtual bool addLine(const char* lineName, const value_type* pos1, const value_type* pos2, const value_type* color);

            virtual bool addCurve(const char* curveName, const PositionSeq& pos, const value_type* color);

            /// See http://svn.openscenegraph.org/osg/OpenSceneGraph-Data/trunk/Images/primitives.gif for
            /// possible values
            virtual bool setCurveMode (const char* curveName, const GLenum mode);

            virtual bool addSquareFace(const char* faceName, const value_type* pos1, const value_type* pos2, const value_type* pos3, const value_type* pos4, const value_type* color);
            virtual bool addTriangleFace(const char* faceName, const value_type* pos1, const value_type* pos2, const value_type* pos3, const value_type* color);
            virtual bool addXYZaxis (const char* nodeNameCorba, const value_type* colorCorba, float radius, float sizeAxis);

            virtual bool createRoadmap(const char* nameCorba,const value_type* colorNodeCorba, float radius, float sizeAxis, const value_type* colorEdgeCorba);

            virtual bool addEdgeToRoadmap(const char* nameRoadmapCorba, const value_type* posFromCorba, const value_type* posToCorba);

            virtual bool addNodeToRoadmap(const char* nameRoadmapCorba, const value_type* configuration);

            virtual bool addURDF(const char* urdfNameCorba, const char* urdfPathCorba, const char* urdfPackagePathCorba);

            virtual bool addUrdfCollision (const char* urdfNameCorba,
                    const char* urdfPathCorba,
                    const char* urdfPackagePathCorba) ;

            virtual void addUrdfObjects (const char* urdfNameCorba,
                    const char* urdfPathCorba,
                    const char* urdfPackagePathCorba,
                    bool visual) ;

            virtual bool createGroup(const char* groupNameCorba);
            virtual bool addToGroup(const char* nodeNameCorba, const char* groupNameCorba);
            virtual bool removeFromGroup (const char* nodeNameCorba, const char* groupNameCorba);
            virtual bool deleteNode (const char* nodeNameCorba);

            virtual bool applyConfiguration(const char* nodeNameCorba, const value_type* configuration);

            virtual bool addLandmark(const char* nodeNameCorba, float size);
            virtual bool deleteLandmark(const char* nodeNameCorba);

            virtual bool getStaticTransform (const char* nodeName, ::gepetto::corbaserver::Transform transform) const;

            virtual bool setVisibility(const char* nodeNameCorba, const char* visibilityModeCorba);
            virtual bool setScale(const char* nodeNameCorba, const value_type* scale);
            virtual bool setColor(const char* nodeNameCorba, const value_type* color);
            virtual bool setWireFrameMode(const char* nodeNameCorba, const char* wireFrameModeCorba);
            virtual bool setLightingMode(const char* nodeNameCorba, const char* lightingModeCorba);
            virtual bool setHighlight(const char* nodeNameCorba, int state);

            virtual bool startCapture (const WindowID windowId, const char* filename,
                    const char* extension);
            virtual bool stopCapture (const WindowID windowId);
            virtual bool setCaptureTransform (const char* filename, const std::list<std::string>& nodename);
            virtual void captureTransformOnRefresh (bool autoCapture);
            virtual void captureTransform ();
            virtual bool writeNodeFile (const char* nodename, const char* filename);
            virtual bool writeWindowFile (const WindowID windowId, const char* filename);

            WindowManagerPtr_t getWindowManager (const WindowID wid);
            GroupNodePtr_t getScene (const std::string sceneName);
            NodePtr_t getNode (const std::string& nodeName) const;
            configuration_t getNodeGlobalTransform(const std::string nodeName);
    };
} /* namespace graphics */

#endif /* SCENEVIEWER_WINDOWMANAGERS_HH */
