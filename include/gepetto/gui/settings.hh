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

    struct Settings {
      std::string configurationFile;
      std::string predifinedRobotConf;
      std::string predifinedEnvConf;

      bool verbose;
      bool noPlugin;
      bool autoWriteSettings;
      bool startGepettoCorbaServer;

      int refreshRate;

      std::string captureDirectory, captureFilename, captureExtension;

      /// Set up default values
      Settings ();

      /// Update values accordingly with command arguments
      int fromArgv (const int argc, char* const argv[]);

      /// Update settings from setting files
      void fromFiles ();

      /// Write the settings to configuration files
      void writeSettings ();

      /// Get a setting
      QVariant getSetting (const QString & key,
          const QVariant & defaultValue = QVariant());

      PluginManager pluginManager_;
      QStringList pluginsToInit_;

      void setMainWindow (MainWindow* main);

      void initPlugins ();

      std::ostream& print (std::ostream& os);

    private:
      void readRobotFile ();
      void readEnvFile ();
      void readSettingFile ();

      void writeRobotFile ();
      void writeEnvFile ();
      void writeSettingFile ();

      void addRobotFromString (const std::string& rbtStr);
      void addEnvFromString (const std::string& envStr);
      void addPlugin (const QString& plg, bool init);

      inline void log (const QString& t);
      inline void logError (const QString& t);

      MainWindow* mw;
    };
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_SETTINGS_HH
