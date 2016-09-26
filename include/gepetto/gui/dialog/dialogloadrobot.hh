#ifndef GEPETTO_GUI_DIALOGLOADROBOT_HH
#define GEPETTO_GUI_DIALOGLOADROBOT_HH

#include <QDialog>
#include <QComboBox>

namespace Ui {
  class DialogLoadRobot;
}

namespace gepetto {
  namespace gui {
    class DialogLoadRobot : public QDialog
    {
      Q_OBJECT

      public:
        struct RobotDefinition {
          QString name_, robotName_, urdfSuf_, srdfSuf_, package_, modelName_, rootJointType_;
          RobotDefinition () : rootJointType_ ("Freeflyer") {}
          RobotDefinition (QString name,
              QString robotName,
              QString rootJointType,
              QString modelName,
              QString package,
              QString urdfSuffix,
              QString srdfSuffix) :
            name_(name), robotName_ (robotName), urdfSuf_(urdfSuffix), srdfSuf_(srdfSuffix),
            package_ (package), modelName_ (modelName),
            rootJointType_ (rootJointType){}
        };

        explicit DialogLoadRobot(QWidget *parent = 0);
        ~DialogLoadRobot();

        static void addRobotDefinition (QString name,
            QString robotName,
            QString rootJointType,
            QString modelName,
            QString package,
            QString urdfSuffix,
            QString srdfSuffix);
        static QList <RobotDefinition> getRobotDefinitions ();

        RobotDefinition getSelectedRobotDescription () {
          return selected_;
        }

        private slots:
          void accept();
        void robotSelect(int index);

      private:
        ::Ui::DialogLoadRobot *ui_;
        QComboBox* defs_;
        RobotDefinition selected_;

        static QList <RobotDefinition> definitions;
        static QStringList rootJointTypes;
    };
  } // namespace gui
} // namespace gepetto

Q_DECLARE_METATYPE (gepetto::gui::DialogLoadRobot::RobotDefinition)

#endif // GEPETTO_GUI_DIALOGLOADROBOT_HH
