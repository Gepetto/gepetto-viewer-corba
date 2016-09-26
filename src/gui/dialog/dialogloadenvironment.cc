#include "gepetto/gui/dialog/dialogloadenvironment.hh"
#include "ui_dialogloadenvironment.h"

#include <QDir>
#include <QMessageBox>
#include <QFileDialog>

namespace gepetto {
  namespace gui {
    QList <DialogLoadEnvironment::EnvironmentDefinition> DialogLoadEnvironment::definitions =
      QList <DialogLoadEnvironment::EnvironmentDefinition>()
      << DialogLoadEnvironment::EnvironmentDefinition ();

    DialogLoadEnvironment::DialogLoadEnvironment(QWidget *parent) :
      QDialog(parent),
      ui_(new ::Ui::DialogLoadEnvironment)
    {
      ui_->setupUi(this);
      ui_->groupBox_details->setVisible (false);
      defs_  = ui_->comboBox_defs;
      defs_->setFocus();

      foreach (EnvironmentDefinition r, definitions) {
        defs_->addItem(r.name_, QVariant::fromValue(r));
      }
    }

    DialogLoadEnvironment::~DialogLoadEnvironment()
    {
      delete ui_;
    }

    void DialogLoadEnvironment::addEnvironmentDefinition(QString name, QString envName, QString package, QString urdfFilename, QString urdfSuffix, QString srdfSuffix)
    {
      definitions.append(EnvironmentDefinition (name, envName, package, urdfFilename, urdfSuffix, srdfSuffix));
    }

    QList<DialogLoadEnvironment::EnvironmentDefinition> DialogLoadEnvironment::getEnvironmentDefinitions()
    {
      return definitions;
    }

    void DialogLoadEnvironment::accept()
    {
      /* TODO use the urdfParser::getFilename to check if the package exists
      QDir d (ui_->packagePath->text ());
      if (!d.cd("urdf")) {
        QMessageBox (QMessageBox::Warning, "Directory not found", d.absolutePath(), QMessageBox::Ok, this).exec();
        return;
      }
      if (!d.exists(ui_->urdfFilename->text () + ".urdf")) {
        QMessageBox (QMessageBox::Warning, "File not found", d.absoluteFilePath(ui_->urdfFilename->text() + ".urdf"), QMessageBox::Ok, this).exec();
        return;
      }
      if (!QDir (ui_->meshDirectory->text ()).exists()) {
        QMessageBox (QMessageBox::Warning, "File not found", ui_->meshDirectory->text (), QMessageBox::Ok, this).exec();
        return;
      }*/
      selected_ = EnvironmentDefinition (defs_->currentText() ,
          ui_->envName->text(),
          ui_->packageName->text(),
          ui_->urdfFilename->text(),
	  ui_->urdfSuffix->text(),
	  ui_->srdfSuffix->text());
      done(QDialog::Accepted);
    }

    void DialogLoadEnvironment::envSelect(int index)
    {
      QVariant v = defs_->itemData(index);
      if (v.canConvert <EnvironmentDefinition>()) {
        EnvironmentDefinition rd = v.value <EnvironmentDefinition> ();
        ui_->envName->setText(rd.envName_);
        ui_->packageName->setText(rd.package_);
        ui_->urdfFilename->setText(rd.urdfFilename_);
	ui_->urdfSuffix->setText(rd.urdfSuf_);
	ui_->srdfSuffix->setText(rd.srdfSuf_);
      }
    }
  } // namespace gui
} // namespace gepetto
