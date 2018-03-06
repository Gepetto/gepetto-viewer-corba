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

#ifndef GEPETTO_GUI_OSGWIDGET_HH
#define GEPETTO_GUI_OSGWIDGET_HH

#include <QString>
#include <QThread>
#include <QLabel>
#include <QTimer>

#include <osg/ref_ptr>

#include <osgQt/GraphicsWindowQt>
#include <gepetto/viewer/window-manager.h>

#include <gepetto/gui/fwd.hh>
#include <gepetto/gui/windows-manager.hh>

namespace gepetto {
  namespace gui {
    /// Widget that displays scenes.
    class OSGWidget : public QWidget
    {
      Q_OBJECT
      public:
        OSGWidget( WindowsManagerPtr_t wm,
                  const std::string & name,
                  MainWindow* parent,
                  Qt::WindowFlags f = 0,
                  osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::Viewer::ThreadPerContext );

        virtual ~OSGWidget();

        WindowsManager::WindowID windowID () const;

        graphics::WindowManagerPtr_t window () const;

        WindowsManagerPtr_t osg () const;

        public slots:
        /// Replace the camera at her home position.
        virtual void onHome();

        void addFloor();

      protected:
        virtual void paintEvent(QPaintEvent* event);

      private:
        osg::ref_ptr<osgQt::GraphicsWindowQt> graphicsWindow_;
        WindowsManagerPtr_t wsm_;
        osg::ref_ptr<PickHandler> pickHandler_;
        WindowsManager::WindowID wid_;
        graphics::WindowManagerPtr_t wm_;
        QTimer timer_;
        osgViewer::ViewerRefPtr viewer_;
        osg::ref_ptr <osgViewer::ScreenCaptureHandler> screenCapture_;

        friend class PickHandler;
    };
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_OSGWIDGET_HH
