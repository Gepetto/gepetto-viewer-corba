// Copyright (c) 2015, Joseph Mirabel
// Authors: Joseph Mirabel (joseph.mirabel@laas.fr)
//
// This file is part of hpp-gui.
// hpp-gui is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
//
// hpp-gui is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// hpp-gui. If not, see <http://www.gnu.org/licenses/>.

#include <gepetto/gui/settings.hh>

#include <boost/program_options.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <QSettings>
#include <QDir>
#include <QtDebug>

#include <gepetto/gui/dialog/dialogloadrobot.hh>
#include <gepetto/gui/dialog/dialogloadenvironment.hh>
#include <gepetto/gui/mainwindow.hh>

#if GEPETTO_GUI_HAS_PYTHONQT
# include <gepetto/gui/pythonwidget.hh>
#endif

namespace po = boost::program_options;

namespace gepetto {
  namespace gui {
    Settings::Settings (const char* installDir)
      : configurationFile ("settings")
      , predifinedRobotConf ("robots")
      , predifinedEnvConf ("environments")
      , verbose (false)
      , noPlugin (false)
      , startGepettoCorbaServer (true)
      , refreshRate (30)
      , captureDirectory ()
      , captureFilename ("screenshot")
      , captureExtension ("png")
      , installDirectory (installDir)

      , mw (0)
    {
      QDir user (QDir::home());
      const char path[] = "Pictures/hpp-gui";
      user.mkpath (path);
      user.cd (path);
      captureDirectory = user.absolutePath().toStdString();
    }

    void Settings::setupPaths () const
    {
      QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
      if (env.contains ("GEPETTO_GUI_SETTINGS_DIR")) {
        QSettings::setPath(QSettings::IniFormat,
            QSettings::SystemScope, env.value("GEPETTO_GUI_SETTINGS_DIR"));
        QSettings::setPath(QSettings::NativeFormat,
            QSettings::SystemScope, env.value("GEPETTO_GUI_SETTINGS_DIR"));
      } else {
        QSettings::setPath(QSettings::IniFormat,
            QSettings::SystemScope, installDirectory + "/etc");
        QSettings::setPath(QSettings::NativeFormat,
            QSettings::SystemScope, installDirectory + "/etc");
      }

      if (env.contains ("GEPETTO_GUI_PLUGIN_DIRS")) {
        foreach (QString p, env.value("GEPETTO_GUI_PLUGIN_DIRS").split(':')) {
          PluginManager::addPluginDir (p + "/gepetto-gui-plugins");
        }
      }
      foreach (QString p, env.value("LD_LIBRARY_PATH").split(':')) {
        PluginManager::addPluginDir (p + "/gepetto-gui-plugins");
      }
    }

    int Settings::fromArgv(const int argc, char * const argv[])
    {
      typedef std::vector <std::string> Strings_t;

      bool help = false;
      bool genAndQuit = false;
      int retVal = 0;

      // Declare the supported options.
      po::options_description desc("Options");
      desc.add_options()
          ("help,h", "produce help message")
          ("verbose,v", "activate verbose output")
          ("generate-config-files,g", "generate configuration files and quit")

          ("config-file,c",
           po::value<std::string>(&configurationFile),
           "set the configuration file (do not include .conf)")

          ("predefined-robots",
           po::value<std::string>(&predifinedRobotConf),
           "set the predefined robots configuration file (do not include .conf)")

          ("predefined-environments",
           po::value<std::string>(&predifinedEnvConf),
           "set the predefined environments configuration file (do not include .conf)")

          ("add-robot", po::value <Strings_t>(),
           "Add a robot (a list of comma sperated string)")

          ("add-env", po::value <Strings_t>(),
           "Add an environment (a list of comma sperated string)")

          ("load-plugin,p", po::value <Strings_t>(),
           "load the plugin")

          ("load-pyplugin,q", po::value <Strings_t>(),
           "load the PythonQt module as a plugin")

          ("no-plugin,P", "do not load any plugin")

          ("auto-write-settings,w", "write the settings in the configuration file")

          ("no-viewer-server", "do not start the Gepetto Viewer server")
          ;

      po::variables_map vm;
      po::parsed_options parsed = po::command_line_parser(argc, argv)
          .options(desc)
          .allow_unregistered()
          .run();
      po::store(parsed, vm);
      po::notify (vm);

      Strings_t unrecognized =
          po::collect_unrecognized (parsed.options, po::exclude_positional);

      help = (vm.count ("help") > 0);
      if (vm.count ("verbose") > 0) verbose = true;
      if (vm.count ("no-plugin") > 0) noPlugin = true;
      if (vm.count ("auto-write-settings") > 0) autoWriteSettings = true;
      if (vm.count ("generate-config-files") > 0) genAndQuit = true;
      if (vm.count ("no-viewer-server") > 0) startGepettoCorbaServer = false;
      if (vm.count ("add-robot") > 0) {
          const Strings_t& rbts = vm["add-robot"].as <Strings_t> ();
          for (Strings_t::const_iterator it = rbts.begin();
               it != rbts.end(); ++it)
            addRobotFromString (*it);
        }
      if (vm.count ("add-env") > 0) {
          const Strings_t& envs = vm["add-env"].as <Strings_t> ();
          for (Strings_t::const_iterator it = envs.begin();
               it != envs.end(); ++it)
            addEnvFromString (*it);
        }
      if (vm.count ("load-plugin") > 0) {
          const Strings_t& envs = vm["load-plugin"].as <Strings_t> ();
          for (Strings_t::const_iterator it = envs.begin();
               it != envs.end(); ++it)
            addPlugin (QString::fromStdString(*it), !noPlugin);
        }
      if (vm.count ("load-pyplugin") > 0) {
          const Strings_t& envs = vm["load-pyplugin"].as <Strings_t> ();
          for (Strings_t::const_iterator it = envs.begin();
               it != envs.end(); ++it)
            addPyPlugin (QString::fromStdString(*it), !noPlugin);
        }

      if (help || genAndQuit) retVal = 1;
      if (unrecognized.size () > 0) {
          std::cout << "Unrecognized options:\n";
          for (std::size_t i = 0; i < unrecognized.size (); ++i)
            std::cout << unrecognized[i] << "\n";
          std::cout << "\n";
          help = true;
          verbose = true;
          retVal = 2;
        }

      if (help) std::cout << desc << std::endl;
      if (verbose) print (std::cout) << std::endl;
      if (genAndQuit) writeSettings ();

      return retVal;
    }

    void Settings::fromFiles ()
    {
      readRobotFile();
      readEnvFile();
      readSettingFile();
    }

    void Settings::writeSettings()
    {
      writeRobotFile ();
      writeEnvFile ();
      writeSettingFile ();
    }

    void Settings::initPlugins()
    {
      foreach (QString name, pluginsToInit_)
        pluginManager_.initPlugin (name);
#if GEPETTO_GUI_HAS_PYTHONQT
      PythonWidget* pw = mw->pythonWidget();
      foreach (QString name, pyplugins_)
        pw->loadModulePlugin (name);
#endif
    }

    void Settings::setMainWindow(gepetto::gui::MainWindow *main)
    {
      mw = main;
    }

    std::ostream& Settings::print (std::ostream& os) {
      const char tab = '\t';
      const char nl = '\n';
      return os
        << nl <<     "Configuration:"
        << nl << tab << "Configuration file:     " << tab << configurationFile
        << nl << tab << "Predefined robots:      " << tab << predifinedRobotConf
        << nl << tab << "Predefined environments:" << tab << predifinedEnvConf

        << nl << nl << "Options:"
        << nl << tab << "Verbose:                " << tab << verbose
        << nl << tab << "No plugin:              " << tab << noPlugin
        << nl << tab << "Start Viewer server:    " << tab << startGepettoCorbaServer
        << nl << tab << "Refresh rate:           " << tab << refreshRate

        << nl << nl << "Screen capture options:"
        << nl << tab << "Directory:              " << tab << captureDirectory
        << nl << tab << "Filename:               " << tab << captureFilename
        << nl << tab << "Extension:              " << tab << captureExtension
           ;
    }

    void Settings::readRobotFile ()
    {
      QSettings robot (QSettings::SystemScope,
          QCoreApplication::organizationName(),
          QString::fromStdString(predifinedRobotConf));
      if (robot.status() != QSettings::NoError) {
        logError(QString ("Enable to open configuration file ")
                 + robot.fileName());
      } else {
        foreach (QString name, robot.childGroups()) {
          robot.beginGroup(name);
          QString robotName = robot.value("RobotName", name).toString();
          DialogLoadRobot::addRobotDefinition(
              name,
              robotName,
              robot.value("RootJointType", "freeflyer").toString(),
              robot.value("ModelName", robotName).toString(),
              robot.value("Package", "").toString(),
              robot.value("URDFSuffix", "").toString(),
              robot.value("SRDFSuffix", "").toString()
              );
          robot.endGroup();
        }
        log(QString ("Read configuration file ")
            + robot.fileName());
      }
    }

    void Settings::readEnvFile ()
    {
      QSettings env (QSettings::SystemScope,
          QCoreApplication::organizationName(),
          QString::fromStdString(predifinedEnvConf));
      if (env.status() != QSettings::NoError) {
        logError(QString ("Enable to open configuration file ") + env.fileName());
      } else {
        foreach (QString name, env.childGroups()) {
          env.beginGroup(name);
          QString envName = env.value("EnvironmentName", name).toString();
          DialogLoadEnvironment::addEnvironmentDefinition(
              name,
              envName,
              env.value("Package", "").toString(),
              env.value("URDFFilename").toString(),
	      env.value("URDFSuffix").toString(),
	      env.value("SRDFSuffix").toString()
              );
          env.endGroup();
        }
        log (QString ("Read configuration file ") + env.fileName());
      }
    }

    void Settings::readSettingFile ()
    {
      QSettings env (QSettings::SystemScope,
          QCoreApplication::organizationName(),
          QString::fromStdString(configurationFile));
      if (env.status() != QSettings::NoError) {
        logError(QString ("Enable to open configuration file ") + env.fileName());
      } else {
        env.beginGroup("plugins");
        foreach (QString name, env.childKeys()) {
            addPlugin (name, (noPlugin)?false:env.value(name, true).toBool());
        }
        env.endGroup ();
        env.beginGroup("pyplugins");
        foreach (QString name, env.childKeys()) {
            addPyPlugin (name, (noPlugin)?false:env.value(name, true).toBool());
        }
        env.endGroup ();
        log (QString ("Read configuration file ") + env.fileName());
      }
    }

    void Settings::writeRobotFile ()
    {
      QSettings robot (QSettings::SystemScope,
                       QCoreApplication::organizationName(),
                       QString::fromStdString(predifinedRobotConf));
      if (!robot.isWritable()) {
          log (QString("Configuration file ") + robot.fileName() + QString(" is not writable."));
          return;
        }
      foreach (DialogLoadRobot::RobotDefinition rd, DialogLoadRobot::getRobotDefinitions()) {
          if (rd.name_.isEmpty()) continue;
          robot.beginGroup(rd.name_);
          robot.setValue("RobotName", rd.robotName_);
          robot.setValue("ModelName", rd.modelName_);
          robot.setValue("RootJointType", rd.rootJointType_);
          robot.setValue("Package", rd.package_);
          robot.setValue("URDFSuffix", rd.urdfSuf_);
          robot.setValue("SRDFSuffix", rd.srdfSuf_);
          robot.endGroup();
        }
      log (QString("Wrote configuration file ") + robot.fileName());
    }

    void Settings::writeEnvFile ()
    {
      QSettings env (QSettings::SystemScope,
                     QCoreApplication::organizationName(),
                     QString::fromStdString(predifinedEnvConf));
      if (!env.isWritable()) {
          logError (QString ("Configuration file") + env.fileName() + QString("is not writable."));
          return;
        }
      foreach (DialogLoadEnvironment::EnvironmentDefinition ed, DialogLoadEnvironment::getEnvironmentDefinitions()) {
          if (ed.name_.isEmpty()) continue;
          env.beginGroup(ed.name_);
          env.setValue("RobotName", ed.envName_);
          env.setValue("Package", ed.package_);
          env.setValue("URDFFilename", ed.urdfFilename_);
          env.endGroup();
        }
      log (QString ("Wrote configuration file ") + env.fileName());
    }

    void Settings::writeSettingFile ()
    {
      QSettings env (QSettings::SystemScope,
                     QCoreApplication::organizationName(),
                     QString::fromStdString(configurationFile));
      if (!env.isWritable()) {
          logError (QString ("Configuration file") + env.fileName() + QString("is not writable."));
          return;
        }
      env.beginGroup("plugins");
      for (PluginManager::Map::const_iterator p = pluginManager_.plugins ().constBegin();
           p != pluginManager_.plugins().constEnd(); p++) {
          env.setValue(p.key(), (noPlugin)?false:p.value()->isLoaded());
        }
      env.endGroup ();
      env.beginGroup("pyplugins");
      foreach (QString name, pyplugins_)
        env.setValue(name, !noPlugin);
      env.endGroup ();
      log (QString ("Read configuration file ") + env.fileName());
    }

    QVariant Settings::getSetting (const QString & key,
        const QVariant & defaultValue)
    {
      QSettings env (QSettings::SystemScope,
          QCoreApplication::organizationName(),
          QString::fromStdString(configurationFile));
      if (env.status() == QSettings::NoError) {
        return env.value (key, defaultValue);
      }
      return defaultValue;
    }

    void Settings::addRobotFromString (const std::string& rbtStr)
    {
      QString rbt = QString::fromStdString(rbtStr);
      QStringList split = rbt.split(",", QString::KeepEmptyParts);
      if (split.size() != 7) {
          logError ("Robot string is not of length 7");
          logError (rbt);
          return;
        }
      DialogLoadRobot::addRobotDefinition(split[0], split[1],
          split[2].toLower(), split[3], split[4],
          split[5], split[6]);
    }

    void Settings::addEnvFromString (const std::string& envStr)
    {
      QString env = QString::fromStdString(envStr);
      QStringList split = env.split(",", QString::KeepEmptyParts);
      if (split.size() != 6) {
          logError ("Environment string is not of length 6");
          logError (env);
          return;
        }
      DialogLoadEnvironment::addEnvironmentDefinition(split[0],
	  split[1], split[2], split[3], split[4], split[5]);
    }

    void Settings::addPlugin (const QString& plg, bool init)
    {
      if (init) pluginsToInit_.append (plg);
      pluginManager_.add(plg, 0, false);
    }

    void Settings::addPyPlugin (const QString& plg, bool init)
    {
      if (init) pyplugins_.append (plg);
    }

    void Settings::log(const QString &t)
    {
      if (!verbose) return;
      if (mw) mw->log(t);
      else    std::cout << t.toLocal8Bit().constData() << std::endl;
    }

    void Settings::logError(const QString &t)
    {
      if (mw) mw->logError(t);
      else    qWarning() << t;
    }
  } // namespace gui
} // namespace gepetto
