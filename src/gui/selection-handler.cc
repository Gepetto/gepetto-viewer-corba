#include <QHBoxLayout>
#include <QLabel>
#include <QVector3D>
#include <QAction>

#include <gepetto/gui/windows-manager.hh>
#include "gepetto/gui/osgwidget.hh"
#include "gepetto/gui/selection-handler.hh"

namespace gepetto {
  namespace gui {
    SelectionHandler::SelectionHandler(WindowsManagerPtr_t wsm, QWidget *parent)
      : QComboBox(parent),
	osg_(NULL),
	wsm_(wsm)
    {
      connect(this, SIGNAL(currentIndexChanged(int)), SLOT(changeMode(int)));
    }

    SelectionHandler::~SelectionHandler()
    {
    }

    void SelectionHandler::setParentOSG(OSGWidget* parent)
    {
      osg_ = parent;
      changeMode(currentIndex());
    }

    void SelectionHandler::changeMode(int index)
    {
      if (osg_ != NULL) {
	foreach(QString name, selected_) {
	  wsm_->setHighlight(name.toStdString().c_str(), 0);	
	}
	modes_[index]->reset();
	disconnect(osg_, SIGNAL(clicked(QString, QVector3D)),
		   modes_[index], SLOT(onSelect(QString, QVector3D)));
	disconnect(modes_[index], SIGNAL(selectedBodies(QStringList)),
		   this, SLOT(getBodies(QStringList)));
	connect(osg_, SIGNAL(clicked(QString, QVector3D)),
		modes_[index], SLOT(onSelect(QString, QVector3D)));
	connect(modes_[index], SIGNAL(selectedBodies(QStringList)),
		SLOT(getBodies(QStringList)));
      }
    }

    void SelectionHandler::addMode(SelectionMode* mode)
    {
      modes_.push_back(mode);
      addItem(mode->getName());
    }

    void SelectionHandler::getBodies(QStringList selectedBodies)
    {
      selected_ = selectedBodies;
    }

    UniqueSelection::UniqueSelection(WindowsManagerPtr_t wsm)
      : SelectionMode(wsm)
    {
    }

    UniqueSelection::~UniqueSelection()
    {
    }

    void UniqueSelection::onSelect(QString name, QVector3D position)
    {
      if (currentSelected_ == name) return;
      if (currentSelected_ != "") wsm_->setHighlight(currentSelected_.toStdString().c_str(), 0);
      currentSelected_ = name;
      wsm_->setHighlight(name.toStdString().c_str(), 8);
      emit selectedBodies(QStringList() << name);
    }

    MultiSelection::MultiSelection(WindowsManagerPtr_t wsm)
      : SelectionMode(wsm)
    {
    }

    MultiSelection::~MultiSelection()
    {
    }

    void MultiSelection::reset()
    {
      SelectionMode::reset();
      selectedBodies_ = QStringList();
    }

    void MultiSelection::onSelect(QString name, QVector3D position)
    {
      if (currentSelected_ == name) return;
      if (currentSelected_ != "") wsm_->setHighlight(currentSelected_.toStdString().c_str(), 7);
      currentSelected_ = name;
      wsm_->setHighlight(name.toStdString().c_str(), 8);
      selectedBodies_ << name;
      emit selectedBodies(selectedBodies_);
    }
  }
}
