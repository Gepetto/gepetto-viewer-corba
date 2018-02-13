// Copyright (c) 2015, Joseph Mirabel
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
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// gepetto-viewer-corba. If not, see <http://www.gnu.org/licenses/>.

#include <gepetto/gui/settings.hh>

#include <QSettings>
#include <QDir>
#include <QtDebug>

#include <osg/DisplaySettings>
#include <osg/ArgumentParser>

#include <gepetto/gui/dialog/dialogloadrobot.hh>
#include <gepetto/gui/dialog/dialogloadenvironment.hh>
#include <gepetto/gui/mainwindow.hh>

#if GEPETTO_GUI_HAS_PYTHONQT
# include <gepetto/gui/pythonwidget.hh>
#endif

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
      const char path[] = "Pictures/gepetto-gui";
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

    int Settings::fromArgv(int argc, char * argv[])
    {
      bool genAndQuit = false;
      int retVal = 0;

      // Declare the supported options.
      osg::ArgumentParser arguments(&argc, argv);
      osg::ApplicationUsage* au (arguments.getApplicationUsage());
      au->setApplicationName(arguments.getApplicationName());
      au->setCommandLineUsage(arguments.getApplicationName()+" [options]");

      osg::ApplicationUsage::Type help (arguments.readHelpType());
      au->addCommandLineOption("-v or --verbose",  "Activate verbose output");
      au->addCommandLineOption("-g or --generate-config-files", "generate configuration files and quit");
      au->addCommandLineOption("-c or --config-file", "set the configuration file (do not include .conf)", configurationFile);
      au->addCommandLineOption("--predefined-robots", "set the predefined robots configuration file (do not include .conf)", predifinedRobotConf);
      au->addCommandLineOption("--predefined-environments", "set the predefined environments configuration file (do not include .conf)", predifinedEnvConf);
      au->addCommandLineOption("--add-robot", "Add a robot (a list of comma sperated string)");
      au->addCommandLineOption("--add-env", "Add an environment (a list of comma sperated string)");
      au->addCommandLineOption("-p or --load-plugin", "load the plugin");
      au->addCommandLineOption("-q or --load-pyplugin", "load the PythonQt module as a plugin");
      au->addCommandLineOption("-P or --no-plugin", "do not load any plugin");
      au->addCommandLineOption("-w or --auto-write-settings", "write the settings in the configuration file");
      au->addCommandLineOption("--no-viewer-server", "do not start the Gepetto Viewer server");

      osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
      // This should not be done here. Two osg built-in way:
      // - Environment variable OSG_MULTI_SAMPLES  : a bit tedious
      // - Command line option --samples           : slight better with an alias
#ifndef DISABLE_SAMPLING
      ds->setNumMultiSamples(4);
#else
      ds->setNumMultiSamples(0);
#endif
      ds->readCommandLine (arguments); // populate the help message.

      verbose =                 (arguments.read("-v") || arguments.read("--verbose"));
      genAndQuit =              (arguments.read("-g") || arguments.read("--generate-config-files"));
      noPlugin =                (arguments.read("-P") || arguments.read("--no-plugin"));
      autoWriteSettings =       (arguments.read("-w") || arguments.read("--auto-write-settings"));
      startGepettoCorbaServer = (!arguments.read("--no-viewer-server"));

      std::string opt;
      while (arguments.read ("--add-robot", opt))
        addRobotFromString (opt);
      while (arguments.read ("--add-env", opt))
        addEnvFromString (opt);
      while (arguments.read ("--load-plugin", opt))
        addPlugin (QString::fromStdString(opt), !noPlugin);
      while (arguments.read ("--load-pyplugin", opt))
        addPyPlugin (QString::fromStdString(opt), !noPlugin);

      if (arguments.read("-c", configurationFile) || arguments.read("--config-file", configurationFile)) {}
      if (arguments.read("--predefined-robots",       predifinedRobotConf)) {}
      if (arguments.read("--predefined-environments", predifinedEnvConf)) {}

      arguments.reportRemainingOptionsAsUnrecognized(osg::ArgumentParser::BENIGN);
      if (arguments.errors(osg::ArgumentParser::CRITICAL)) {
        arguments.writeErrorMessages(std::cout);
        retVal = 2;
      } else if (arguments.errors(osg::ArgumentParser::BENIGN)) {
        arguments.writeErrorMessages(std::cout);
      }

      if (genAndQuit && retVal < 1) retVal = 1;

      if (help != osg::ApplicationUsage::NO_HELP) {
        au->write(std::cout, help, 80, true);
        if (retVal < 1) retVal = 1;
      }
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

    QString Settings::getQSettingsFileName (const std::string& settingsName) const
    {
      QString name (QString::fromStdString (settingsName));
      QString ext (".conf");
      // Remove extension
      if (name.endsWith (ext))
        name = name.left (name.size() - ext.size());
      return name;
    }

    void Settings::readRobotFile ()
    {
      QSettings robot (QSettings::SystemScope,
          QCoreApplication::organizationName (),
          getQSettingsFileName (predifinedRobotConf));
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
          QCoreApplication::organizationName (),
          getQSettingsFileName (predifinedEnvConf));
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
          QCoreApplication::organizationName (),
          getQSettingsFileName (configurationFile));
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
          QCoreApplication::organizationName (),
          getQSettingsFileName (predifinedRobotConf));
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
          QCoreApplication::organizationName (),
          getQSettingsFileName (predifinedEnvConf));
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
          QCoreApplication::organizationName (),
          getQSettingsFileName (configurationFile));
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
          QCoreApplication::organizationName (),
          getQSettingsFileName (configurationFile));
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
