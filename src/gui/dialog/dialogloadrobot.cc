#include "gepetto/gui/dialog/dialogloadrobot.hh"
#include "ui_dialogloadrobot.h"

#include <QDir>
#include <QMessageBox>
#include <QFileDialog>

namespace gepetto {
  namespace gui {
    QStringList DialogLoadRobot::rootJointTypes = QStringList () << "freeflyer" << "planar" << "anchor";

    QList <DialogLoadRobot::RobotDefinition> DialogLoadRobot::definitions =
      QList <DialogLoadRobot::RobotDefinition>()
      << DialogLoadRobot::RobotDefinition ();

    DialogLoadRobot::DialogLoadRobot(QWidget *parent) :
      QDialog(parent),
      ui_(new ::Ui::DialogLoadRobot)
    {
      ui_->setupUi(this);
      ui_->groupBox_details->setVisible (false);

      ui_->rootJointType->addItems (rootJointTypes);
      defs_  = ui_->comboBox_defs;

      foreach (RobotDefinition r, definitions) {
        defs_->addItem(r.name_, QVariant::fromValue(r));
      }
    }

    DialogLoadRobot::~DialogLoadRobot()
    {
      delete ui_;
    }

    void DialogLoadRobot::addRobotDefinition(QString name, QString robotName, QString rootJointType, QString modelName, QString package, QString packagePath, QString urdfSuffix, QString srdfSuffix, QString meshDirectory)
    {
      definitions.append(RobotDefinition (name, robotName, rootJointType, modelName, package, packagePath, urdfSuffix, srdfSuffix, meshDirectory));
    }

    QList<DialogLoadRobot::RobotDefinition> DialogLoadRobot::getRobotDefinitions()
    {
      return definitions;
    }

    void DialogLoadRobot::accept()
    {
      QDir d (ui_->packagePath->text ());
      if (!d.cd("urdf")) {
        QMessageBox (QMessageBox::Warning, "Directory not found", d.absolutePath(), QMessageBox::Ok, this).exec();
        return;
      }
      if (!d.exists(ui_->modelName->text () + ui_->urdfSuffix->text () + ".urdf")) {
        QMessageBox (QMessageBox::Warning, "File not found", d.absoluteFilePath(ui_->modelName->text () + ui_->urdfSuffix->text () + ".urdf"), QMessageBox::Ok, this).exec();
        return;
      }
      if (!QDir (ui_->meshDirectory->text ()).exists()) {
        QMessageBox (QMessageBox::Warning, "File not found", ui_->meshDirectory->text (), QMessageBox::Ok, this).exec();
        return;
      }
      selected_ = RobotDefinition (defs_->currentText() ,
          ui_->robotName->text(),
          ui_->rootJointType->currentText(),
          ui_->modelName->text(),
          ui_->packageName->text(),
          ui_->packagePath->text(),
          ui_->urdfSuffix->text(),
          ui_->srdfSuffix->text(),
          ui_->meshDirectory->text());
      done(QDialog::Accepted);
    }

    void DialogLoadRobot::packagePathSelect ()
    {
      QDir path (QFileDialog::getExistingDirectory(this, "Package", ui_->packagePath->text ()));
      ui_->packagePath->setText(path.absolutePath());
      ui_->packageName->setText(path.dirName());
      if (ui_->meshDirectory->text().isEmpty())
        ui_->meshDirectory->setText(path.absolutePath());
    }

    void DialogLoadRobot::robotSelect(int index)
    {
      QVariant v = defs_->itemData(index);
      if (v.canConvert <RobotDefinition>()) {
        RobotDefinition rd = v.value <RobotDefinition> ();
        ui_->robotName->setText(rd.robotName_);
        ui_->modelName->setText(rd.modelName_);
        if (rootJointTypes.contains(rd.rootJointType_))
          ui_->rootJointType->setCurrentIndex (rootJointTypes.indexOf(rd.rootJointType_));
        ui_->packageName->setText(rd.package_);
        ui_->packagePath->setText(rd.packagePath_);
        ui_->urdfSuffix->setText(rd.urdfSuf_);
        ui_->srdfSuffix->setText(rd.srdfSuf_);
        ui_->meshDirectory->setText(rd.mesh_);
      }
    }

    void DialogLoadRobot::meshSelect ()
    {
      ui_->meshDirectory->setText(QFileDialog::getExistingDirectory(this, "Mesh data directory", ui_->meshDirectory->text()));
    }
  } // namespace gui
} // namespace gepetto
