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
	index_(-1),
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
	if (index_ != -1) {
	  modes_[index_]->reset();
	  disconnect(osg_, SIGNAL(clicked(QString, QVector3D, QKeyEvent*)),
		     modes_[index_], SLOT(onSelect(QString, QVector3D, QKeyEvent*)));
	  disconnect(modes_[index_], SIGNAL(selectedBodies(QStringList)),
		     this, SLOT(getBodies(QStringList)));
	}
	index_ = index;
	connect(osg_, SIGNAL(clicked(QString, QVector3D, QKeyEvent*)),
		modes_[index], SLOT(onSelect(QString, QVector3D, QKeyEvent*)));
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

    void UniqueSelection::onSelect(QString name, QVector3D /*position*/,
				   QKeyEvent* /*event*/)
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

    void MultiSelection::onSelect(QString name, QVector3D /*position*/,
				  QKeyEvent* event)
    {
      if (currentSelected_ == name) return;
      if (!event || event->key() != Qt::Key_Control) {
	foreach (QString n, selectedBodies_) {
	  wsm_->setHighlight(n.toStdString().c_str(), 0);
	}
	selectedBodies_.clear();
      }
      else if (currentSelected_ != "") {
	wsm_->setHighlight(currentSelected_.toStdString().c_str(), 7);
      }
      currentSelected_ = name;
      wsm_->setHighlight(name.toStdString().c_str(), 8);
      selectedBodies_ << name;
      emit selectedBodies(selectedBodies_);
    }
  }
}
