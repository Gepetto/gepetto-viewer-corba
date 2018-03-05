// Copyright (c) 2015-2018, LAAS-CNRS
// Authors: Joseph Mirabel (joseph.mirabel@laas.fr)
//
// This file is part of gepetto-viewer-corba.
// gepetto-viewer-corba is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
//
// gepetto-viewer-corba is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Lesser Public License for more details. You should have
// received a copy of the GNU Lesser General Public License along with
// gepetto-viewer-corba. If not, see <http://www.gnu.org/licenses/>.

#ifndef GEPETTO_GUI_PICK_HANDLER_HH
#define GEPETTO_GUI_PICK_HANDLER_HH

#include <osgGA/GUIEventHandler>
#include <gepetto/viewer/node.h>

#include <QObject>
#include <QModelIndex>

#include <gepetto/gui/fwd.hh>
#include <gepetto/gui/qt-osg-keyboard.hh>

namespace gepetto {
  namespace gui {
    class PickHandler : public QObject, public osgGA::GUIEventHandler
    {
      Q_OBJECT

    public:
      PickHandler (OSGWidget* parent, WindowsManagerPtr_t wsm);

      virtual ~PickHandler();

      virtual bool handle( const osgGA::GUIEventAdapter&  ea,
                                 osgGA::GUIActionAdapter& aa );

      void getUsage (osg::ApplicationUsage &usage) const;

    private:
      std::list <graphics::NodePtr_t> computeIntersection (osgGA::GUIActionAdapter& aa,
                                                           const float& x, const float& y,
							   int modMask);

      void setCameraToSelected (osgGA::GUIActionAdapter& aa, bool zoom);

      QtOsgKeyboard mapper_;
      WindowsManagerPtr_t wsm_;
      OSGWidget* parent_;
      bool pushed_;
      float lastX_, lastY_;
    };
  }
}

#endif // GEPETTO_GUI_PICK_HANDLER_HH
