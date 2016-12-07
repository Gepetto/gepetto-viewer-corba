#ifndef GEPETTO_GUI_DIALOGLOADENVIRONMENT_HH
#define GEPETTO_GUI_DIALOGLOADENVIRONMENT_HH

#include <QDialog>
#include <QComboBox>

namespace Ui {
  class DialogLoadEnvironment;
}

namespace gepetto {
  namespace gui {
    class DialogLoadEnvironment : public QDialog
    {
      Q_OBJECT

      public:
        explicit DialogLoadEnvironment(QWidget *parent = 0);
        ~DialogLoadEnvironment();

        struct EnvironmentDefinition {
          QString name_, envName_, urdfFilename_, package_, urdfSuf_,
	    srdfSuf_;
          EnvironmentDefinition () {}
          EnvironmentDefinition (QString name, QString envName,
	    QString package,
	    QString urdfFilename, QString urdfSuffix, QString srdfSuffix) :
            name_(name), envName_ (envName), urdfFilename_(urdfFilename),
            package_ (package), urdfSuf_(urdfSuffix), srdfSuf_(srdfSuffix)
          {}
        };

        static void addEnvironmentDefinition (QString name,
            QString envName,
            QString package,
            QString urdfFilename,
	    QString urdfSuffix,
	    QString srdfSuffix);
        static QList <EnvironmentDefinition> getEnvironmentDefinitions ();

        EnvironmentDefinition getSelectedDescription () {
          return selected_;
        }

        private slots:
          void accept();
        void envSelect(int index);

      private:
        ::Ui::DialogLoadEnvironment *ui_;
        QComboBox* defs_;
        EnvironmentDefinition selected_;

        static QList <EnvironmentDefinition> definitions;
    };
  } // namespace gui
} // namespace gepetto

Q_DECLARE_METATYPE (gepetto::gui::DialogLoadEnvironment::EnvironmentDefinition)

#endif // GEPETTO_GUI_DIALOGLOADENVIRONMENT_HH
