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

#ifndef GEPETTO_GUI_PLUGININTERFACE_HH
#define GEPETTO_GUI_PLUGININTERFACE_HH

#include <QtGui>
#include <QWidget>
#include <iostream>

#include <gepetto/gui/dialog/dialogloadrobot.hh>
#include <gepetto/gui/dialog/dialogloadenvironment.hh>

#include <omniORB4/CORBA.h>

namespace gepetto {
  namespace gui {
    const int DockKeyShortcutBase = Qt::CTRL + Qt::ALT;

    /// \ingroup plugin_cpp
    /// Base interface for C++ plugins.
    class PluginInterface {
      public:
        PluginInterface ()
          : errorMsg_ ("Not initalized")
          , isInit_ (false)
        {}

        virtual ~PluginInterface () {}

        virtual QString name () const = 0;

        void doInit ()
        {
          try {
            init ();
            isInit_ = true;
          } catch (const std::exception& e) {
            errorMsg_ = QString (e.what ());
          }
        }

        bool isInit () const
        {
          return isInit_;
        }

        const QString& errorMsg () const
        {
          return errorMsg_;
        }

      protected:
        virtual void init () = 0;

      private:
        QString errorMsg_;
        bool isInit_;
    };

    /// \ingroup plugin_cpp
    /// Interface to add actions to the joint tree.
    class JointModifierInterface {
      public:
        virtual ~JointModifierInterface () {}

        virtual QAction* action (const std::string& jointName) const = 0;
    };

    /// \ingroup plugin_cpp
    /// Interface to load robot models.
      class ModelInterface {
        public:
          virtual ~ModelInterface () {}

          virtual void loadRobotModel (DialogLoadRobot::RobotDefinition rd) = 0;

          virtual void loadEnvironmentModel (DialogLoadEnvironment::EnvironmentDefinition ed) = 0;

          virtual std::string getBodyFromJoint (const std::string& jointName) const = 0;
      };

    /// \ingroup plugin_cpp
    /// You must implement this interface is you have corba clients.
      class CorbaInterface {
        public:
          virtual ~CorbaInterface () {}

          virtual void openConnection () = 0;

          virtual void closeConnection () = 0;

          /// return true if error was handled.
          virtual bool corbaException (int jobId, const CORBA::Exception& excep) const = 0;
      };
  } // namespace gui
} // namespace gepetto

Q_DECLARE_INTERFACE (gepetto::gui::PluginInterface, "gepetto-gui.plugins/0.0")
Q_DECLARE_INTERFACE (gepetto::gui::JointModifierInterface, "gepetto-gui.plugin.joint-modifier/0.0")
Q_DECLARE_INTERFACE (gepetto::gui::ModelInterface, "gepetto-gui.plugin.model/0.0")
Q_DECLARE_INTERFACE (gepetto::gui::CorbaInterface, "gepetto-gui.plugin.corba/0.0")



#endif // GEPETTO_GUI_PLUGININTERFACE_HH
