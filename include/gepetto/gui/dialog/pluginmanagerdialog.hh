#ifndef GEPETTO_GUI_PLUGINMANAGERDIALOG_HH
#define GEPETTO_GUI_PLUGINMANAGERDIALOG_HH

#include <QDialog>
#include <QMap>
#include <QSignalMapper>
#include <QDir>
#include <QPluginLoader>
#include <QTableWidgetItem>

#include <gepetto/gui/plugin-interface.hh>

namespace Ui {
  class PluginManagerDialog;
}

namespace gepetto {
  namespace gui {
    class PluginManager {
      public:
        typedef QPair <QString, QPluginLoader*> Pair;
        typedef QMap <QString, QPluginLoader*> Map;

        ~PluginManager () {
          qDeleteAll (plugins_);
        }

        const QMap <QString, QPluginLoader*>& plugins () const {
          return plugins_;
        }

        bool add (const QString& name, QWidget* parent = NULL, bool load = false);

        template <typename Interface> Interface* getFirstOf ();

        template <typename Interface> QList <Interface*> get ();

        static QIcon icon (const QPluginLoader* pl);

        static QString status (const QPluginLoader* pl);

        static void addPluginDir (const QString& path);

        bool initPlugin (const QString& name);

        bool unloadPlugin (const QString& name);

        void clearPlugins ();

      private:
        template <typename Interface>
          static const Interface* const_instance_cast (const QPluginLoader* pl);

        QMap <QString, QPluginLoader*> plugins_;
        static QList <QDir> pluginDirs_;
    };

    class PluginManagerDialog : public QDialog
    {
      Q_OBJECT

      public:
        explicit PluginManagerDialog(PluginManager* pm, QWidget *parent = 0);
        ~PluginManagerDialog();

        public slots:
          void onItemChanged (QTableWidgetItem* current, QTableWidgetItem* previous);
        void contextMenu(const QPoint& pos);

        void load (const QString& name);
        void unload (const QString& name);

      private:
        static const int P_NAME;
        static const int P_FILE;
        static const int P_FULLPATH;
        static const int P_VERSION;

        void updateList ();

        ::Ui::PluginManagerDialog *ui_;

        PluginManager* pm_;
        QSignalMapper signalMapper_;
    };

    template <typename Interface>
      Interface* PluginManager::getFirstOf ()
      {
        foreach (QPluginLoader* p, plugins_) {
          if (p->isLoaded()) {
            PluginInterface* check = qobject_cast <PluginInterface*> (p->instance());
            if (check && check->isInit ()) {
              Interface* pi = qobject_cast <Interface*> (p->instance());
              if (pi) return pi;
            }
          }
        }
        return NULL;
      }

    template <typename Interface>
      QList <Interface*> PluginManager::get ()
      {
        QList <Interface*> list;
        foreach (QPluginLoader* p, plugins_) {
          if (p->isLoaded()) {
            PluginInterface* check = qobject_cast <PluginInterface*> (p->instance());
            if (check && check->isInit ()) {
              Interface* pi = qobject_cast <Interface*> (p->instance());
              if (pi)
                list.append(pi);
            }
          }
        }
        return list;
      }

    template <typename Interface>
      const Interface* PluginManager::const_instance_cast (const QPluginLoader* pl)
      {
        return (const Interface*) qobject_cast <Interface*> (const_cast <QPluginLoader*>(pl)->instance());
      }
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_PLUGINMANAGERDIALOG_HH
