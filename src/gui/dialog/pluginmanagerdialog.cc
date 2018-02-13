#include "gepetto/gui/dialog/pluginmanagerdialog.hh"
#include "ui_pluginmanagerdialog.h"

#include <QDebug>
#include <QMenu>

#include "gepetto/gui/plugin-interface.hh"

#include <iostream>

namespace gepetto {
  namespace gui {
    QList <QDir> PluginManager::pluginDirs_;

    bool PluginManager::add(const QString &name, QWidget *parent, bool init)
    {
      if (!plugins_.contains(name)) {
          QString filename = name;
          if (!QDir::isAbsolutePath(name)) {
              foreach (QDir dir, pluginDirs_) {
                  if (dir.exists(name)) {
                      filename = dir.absoluteFilePath(name);
                      break;
                    }
                }
            }
          plugins_[name] = new QPluginLoader (filename, parent);
        }
      if (!plugins_[name]->load()) {
        qDebug() << name << ": " << plugins_[name]->errorString();
        return false;
      }
      if (init) return initPlugin(name);
      return false;
    }

    QIcon PluginManager::icon(const QPluginLoader *pl)
    {
      if (pl->isLoaded()) {
        const PluginInterface* pi = const_instance_cast <PluginInterface> (pl);
        if (pi && pi->isInit ()) {
          return QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation);
        }
      }
      return QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical);
    }

    QString PluginManager::status(const QPluginLoader *pl)
    {
      if (pl->isLoaded()) {
        const PluginInterface* pi = const_instance_cast <PluginInterface> (pl);
        if (pi) {
          if (pi->isInit ())
            return QString ("Plugin loaded correctly");
          else
            return pi->errorMsg ();
        } else
          return QString ("Wrong interface");
      } else
        return pl->errorString();
    }

    void PluginManager::addPluginDir(const QString &path)
    {
      QDir dir (QDir::cleanPath(path));
      QDir can (dir.canonicalPath());
      if (can.exists() && can.isReadable())
	{
	  pluginDirs_.append (can);
	}
    }

    bool PluginManager::initPlugin(const QString &name)
    {
      if (!plugins_[name]->isLoaded()) {
        qDebug() << name << ": " << plugins_[name]->errorString();
        return false;
      }
      PluginInterface* pi = qobject_cast <PluginInterface*> (plugins_[name]->instance());
      if (!pi) {
        qDebug() << name << ": Wrong interface.";
        return false;
      }
      pi->doInit();
      return pi->isInit ();
    }

    bool PluginManager::unloadPlugin(const QString &name)
    {
      return plugins_[name]->unload();
    }

    void PluginManager::clearPlugins()
    {
      foreach (QPluginLoader* p, plugins_) {
        p->unload();
      }
    }

    PluginManagerDialog::PluginManagerDialog(PluginManager *pm, QWidget *parent) :
      QDialog(parent),
      ui_(new ::Ui::PluginManagerDialog),
      pm_ (pm)
    {
      ui_->setupUi(this);

      updateList ();

      ui_->pluginList->setColumnHidden(P_FILE, true);

      connect(ui_->pluginList, SIGNAL (currentItemChanged (QTableWidgetItem*,QTableWidgetItem*)),
          this, SLOT (onItemChanged(QTableWidgetItem*,QTableWidgetItem*)));
      connect(ui_->pluginList, SIGNAL(customContextMenuRequested(QPoint)),
          this, SLOT(contextMenu(QPoint)));
    }

    PluginManagerDialog::~PluginManagerDialog()
    {
      delete ui_;
    }

    void PluginManagerDialog::onItemChanged(QTableWidgetItem *current,
        QTableWidgetItem* /*previous*/)
    {
      if (!current) return;
      QString key = ui_->pluginList->item(current->row(), P_FILE)->text();
      const QPluginLoader* pl = pm_->plugins()[key];
      ui_->pluginMessage->setText(pm_->status (pl));
    }

    void PluginManagerDialog::contextMenu(const QPoint &pos)
    {
      int row = ui_->pluginList->rowAt(pos.y());
      if (row == -1) return;
      QString key = ui_->pluginList->item(row, P_FILE)->text();
      QMenu contextMenu (tr("Plugin"), ui_->pluginList);
      if (pm_->plugins()[key]->isLoaded()) {
        QAction* unload = contextMenu.addAction("&Unload", &signalMapper_, SLOT(map()));
        signalMapper_.setMapping (unload, key);
        connect(&signalMapper_, SIGNAL (mapped(QString)), this, SLOT(unload(QString)));
        contextMenu.exec(ui_->pluginList->mapToGlobal(pos));
      } else {
        QAction* load = contextMenu.addAction("&Load", &signalMapper_, SLOT(map()));
        signalMapper_.setMapping (load, key);
        connect(&signalMapper_, SIGNAL (mapped(QString)), this, SLOT(load(QString)));
        contextMenu.exec(ui_->pluginList->mapToGlobal(pos));
      }
    }

    void PluginManagerDialog::load(const QString &name)
    {
      pm_->initPlugin(name);
      updateList ();
    }

    void PluginManagerDialog::unload(const QString &name)
    {
      pm_->unloadPlugin (name);
      updateList ();
    }

    const int PluginManagerDialog::P_NAME = 0;
    const int PluginManagerDialog::P_FILE = 1;
    const int PluginManagerDialog::P_VERSION = 2;
    const int PluginManagerDialog::P_FULLPATH = 3;

    void PluginManagerDialog::updateList()
    {
      while (ui_->pluginList->rowCount() > 0)
        ui_->pluginList->removeRow(0);
      for (PluginManager::Map::const_iterator p = pm_->plugins ().constBegin();
          p != pm_->plugins().constEnd(); p++) {
        QString name = p.value()->fileName(),
                filename = p.key(),
                fullpath = p.value()->fileName(),
                version = "";
        if (p.value ()->isLoaded ()) {
          PluginInterface* pi = qobject_cast <PluginInterface*> (p.value()->instance());
          name = pi->name();
          // version = pi->version();
        }
        QIcon icon = pm_->icon (p.value());

        ui_->pluginList->insertRow(ui_->pluginList->rowCount());
        ui_->pluginList->setItem(ui_->pluginList->rowCount() - 1, P_NAME, new QTableWidgetItem (icon, name));
        ui_->pluginList->setItem(ui_->pluginList->rowCount() - 1, P_FILE, new QTableWidgetItem (filename));
        ui_->pluginList->setItem(ui_->pluginList->rowCount() - 1, P_VERSION, new QTableWidgetItem (version));
        ui_->pluginList->setItem(ui_->pluginList->rowCount() - 1, P_FULLPATH, new QTableWidgetItem (fullpath));
      }
      ui_->pluginList->resizeColumnsToContents();
    }
  } // namespace gui
} // namespace gepetto
