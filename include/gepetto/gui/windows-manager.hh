#ifndef GEPETTO_GUI_WINDOWSMANAGER_HH
#define GEPETTO_GUI_WINDOWSMANAGER_HH

#include <gepetto/gui/fwd.hh>
#include <gepetto/viewer/corba/windows-manager.hh>

#include <gepetto/gui/meta.hh>
#include <QColor>
#include <QVector3D>

namespace gepetto {
  namespace gui {
    template <> struct convertTo<graphics::WindowsManager::Color_t> {
      typedef graphics::WindowsManager::Color_t T;
      typedef typename T::value_type v_t;
      static inline T from (const QColor& in) {
        T v((v_t)in.redF(), (v_t)in.greenF(), (v_t)in.blueF(), (v_t)in.alphaF());
        // in.getRgbF(&v[0],&v[1],&v[2],&v[3]);
        return v;
      }
    };

    class WindowsManager : public QObject, public graphics::WindowsManager
    {
      Q_OBJECT

      public:
        typedef graphics::WindowsManager Parent_t;
        typedef graphics::NodePtr_t      NodePtr_t;
        typedef graphics::GroupNodePtr_t GroupNodePtr_t;

        static WindowsManagerPtr_t create (BodyTreeWidget* bodyTree);

        WindowID createWindow(const std::string& windowName);
        WindowID createWindow(const std::string& windowName,
                              osgViewer::Viewer* viewer,
                              osg::GraphicsContext *gc);

        bool addToGroup(const std::string& nodeName, const std::string& groupName);
        bool removeFromGroup (const std::string& nodeName, const std::string& groupName);
        bool deleteNode (const std::string& nodeName, bool all);

      protected:
        WindowsManager (BodyTreeWidget* bodyTree);

        virtual void addNode (const std::string& nodeName, NodePtr_t node, GroupNodePtr_t parent);
        virtual void addGroup(const std::string& groupName, GroupNodePtr_t group, GroupNodePtr_t parent);

      private:
        typedef std::vector<BodyTreeItem*> BodyTreeItems_t;
        typedef std::pair<BodyTreeItems_t, bool> BodyTreeItemsAndGroup_t;
        typedef std::map<std::string, BodyTreeItemsAndGroup_t> BodyTreeItemMap_t;
        BodyTreeWidget* bodyTree_;
        BodyTreeItemMap_t nodeItemMap_;

        bool initParent (NodePtr_t node, GroupNodePtr_t parent, bool isGroup);
        void addToGroup (const std::string& nodeName, const std::string& groupName,
                         const NodePtr_t&   node,     const BodyTreeItems_t& groups,
                         bool isGroup);
        void deleteBodyItem(const std::string& nodeName);
    };
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_WINDOWSMANAGER_HH
