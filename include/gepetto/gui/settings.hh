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

#ifndef GEPETTO_GUI_SETTINGS_HH
#define GEPETTO_GUI_SETTINGS_HH

#include <ostream>
#include <string>
#include <QString>
#include <QStringList>

#include <gepetto/gui/dialog/pluginmanagerdialog.hh>

namespace gepetto {
  namespace gui {
    class MainWindow;

    /// Settings manager for the interface.
    ///
    /// This struct is responsible for parsing configuration files as follow:
    /// - Robots file: Settings::readRobotFile()
    /// - Environments file: Settings::readEnvFile()
    /// - Configuration file: Settings::readSettingFile()
    struct Settings {
      std::string configurationFile;
      std::string predifinedRobotConf;
      std::string predifinedEnvConf;
      std::string stateConf;

      bool verbose;
      bool noPlugin;
      bool autoWriteSettings;
      bool startGepettoCorbaServer;

      int refreshRate;

      std::string captureDirectory, captureFilename, captureExtension;

      QString installDirectory;

      /// Set up default values
      Settings (const char* installDirectory);

      /// Setup paths to find setting files and plugins.
      /// \note The environment variable
      /// GEPETTO_GUI_PLUGIN_DIRS, LD_LIBRARY_PATH
      /// and GEPETTO_GUI_SETTINGS_DIR are read.
      void setupPaths () const;

      /// Get the filename of a configuration file.
      QString getQSettingsFileName (const std::string& settingsName) const;

      /// Update values accordingly with command arguments
      int fromArgv (int argc, char* argv[]);

      /// Update settings from setting files
      void fromFiles ();

      /// Write the settings to configuration files
      void writeSettings ();

      /// Get a setting
      QVariant getSetting (const QString & key,
          const QVariant & defaultValue = QVariant());

      PluginManager pluginManager_;
      QStringList pluginsToInit_;
      QStringList pyplugins_;

      void setMainWindow (MainWindow* main);

      void initPlugins ();

      std::ostream& print (std::ostream& os);

      /// \note Prefer using Settings::fromFiles()
      void readRobotFile ();
      /// \note Prefer using Settings::fromFiles()
      void readEnvFile ();
      /// Read the settings file.
      ///
      /// Here is the syntax:
      /// \code
      /// ; Comments starts with a ; You may uncomment to see the effect.
      ///
      /// [plugins]
      /// ; Put a list of C++ plugins followed by '=true'. For instance, HPP users may have
      /// ; hppwidgetsplugin.so=true
      /// ; hppcorbaserverplugin.so=true
      ///
      /// [pyplugins]
      /// ; Put a list of Python plugins followed by '=true'. For instance, the example plugin can be loaded with
      /// ; gepetto.plugin=true
      ///
      /// ; WARNING: Any comment in this file may be removed by the GUI if you regenerate a configuration file.
      /// \endcode
      /// \note Details on plugin interface can be found in PluginInterface, resp. PythonWidget, class
      /// for C++, resp. Python, plugins.
      /// \note Prefer using Settings::fromFiles()
      void readSettingFile ();

      void saveState () const;
      void restoreState () const;
      void restoreDockWidgetsState () const;

      void writeRobotFile ();
      void writeEnvFile ();
      void writeSettingFile ();

      const char** makeOmniORBargs (int &argc);

    private:
      void addRobotFromString (const std::string& rbtStr);
      void addEnvFromString (const std::string& envStr);
      void addPlugin (const QString& plg, bool init);
      void addPyPlugin (const QString& plg, bool init);
      void addOmniORB (const QString& arg, const QString& value);

      inline void log (const QString& t);
      inline void logError (const QString& t);

      MainWindow* mw;
      QStringList omniORBargv_;
    };
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_SETTINGS_HH
