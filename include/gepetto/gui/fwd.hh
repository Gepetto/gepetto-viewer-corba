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

#ifndef GEPETTO_GUI_FWD_HH
#define GEPETTO_GUI_FWD_HH

#include <vector>

#include <QtGlobal>
#if QT_VERSION >= 0x050000
    #include <QtGui/qopengl.h>
#endif
#include <QtGui>


#include <gepetto/viewer/macros.h>
#include <gepetto/gui/config-dep.hh>

namespace gepetto {
  namespace gui {
    class MainWindow;
    class OSGWidget;
    class PickHandler;
    class BodyTreeWidget;
    class BodyTreeItem;
    typedef std::vector<BodyTreeItem*> BodyTreeItems_t;
    class ShortcutFactory;
    class SelectionHandler;
    class SelectionEvent;
    class ActionSearchBar;

    class ViewerCorbaServer;

    class WindowsManager;
    typedef graphics::shared_ptr <WindowsManager> WindowsManagerPtr_t;

#if GEPETTO_GUI_HAS_PYTHONQT
    class PythonWidget;
#endif
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_FWD_HH
