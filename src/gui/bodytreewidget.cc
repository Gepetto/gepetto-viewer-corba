#include <gepetto/gui/bodytreewidget.hh>

#include <gepetto/gui/tree-item.hh>
#include <gepetto/gui/mainwindow.hh>
#include <gepetto/gui/windows-manager.hh>
#include <gepetto/gui/osgwidget.hh>
#include <gepetto/gui/meta.hh>

#include <QSignalMapper>
#include <QColorDialog>

static void addSelector (QToolBox* tb, QString title, QStringList display, QStringList command,
                         QObject* receiver, const char* slot) {
  QWidget* newW = new QWidget();
  newW->setObjectName(title);
  QSignalMapper* mapper = new QSignalMapper (tb);
  QHBoxLayout* layout = new QHBoxLayout(newW);
  layout->setSpacing(6);
  layout->setContentsMargins(11, 11, 11, 11);
  layout->setObjectName(title + "_layout");
  for (int i = 0; i < display.size(); ++i) {
      QPushButton* button = new QPushButton(display[i], newW);
      button->setObjectName(title + "_button_" + display[i]);
      layout->addWidget (button);
      mapper->setMapping(button, command[i]);
      QObject::connect (button, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    }
  receiver->connect (mapper, SIGNAL(mapped(QString)), slot);
  tb->addItem(newW, title);
}

static void addColorSelector (QToolBox* tb, QString title, QObject* receiver, const char* slot) {
  QWidget* newW = new QWidget();
  newW->setObjectName(title);
  QHBoxLayout* layout = new QHBoxLayout();
  newW->setLayout(layout);
  layout->setSpacing(6);
  layout->setContentsMargins(11, 11, 11, 11);
  layout->setObjectName(title + "_layout");
  QPushButton* button = new QPushButton("Select color", newW);
  button->setObjectName(title + "_buttonSelect");
  layout->addWidget (button);

  QColorDialog* colorDialog = new QColorDialog(newW);
  colorDialog->setObjectName(title + "_colorDialog");
  colorDialog->setOption(QColorDialog::ShowAlphaChannel, true);

  colorDialog->connect(button, SIGNAL(clicked()), SLOT(open()));
  receiver->connect (colorDialog, SIGNAL(colorSelected(QColor)), slot);
  tb->addItem(newW, title);
}

static void addSlider (QToolBox* tb, QString title, QObject* receiver, const char* slot) {
    QSlider* slider = new QSlider (Qt::Horizontal);
    slider->setMinimum(0);
    slider->setMaximum(100);
    slider->setObjectName(title);

  receiver->connect (slider, SIGNAL(valueChanged(int)), slot);
  tb->addItem(slider, title);
}

namespace gepetto {
  namespace gui {
    void BodyTreeWidget::init(QTreeView* view, QToolBox *toolBox)
    {
      MainWindow* main = MainWindow::instance();
      osg_ = main->osg();
      view_ = view;
      toolBox_ = toolBox;
      model_  = new QStandardItemModel (this);
      view_->setModel(model_);
      view_->setSelectionMode(QAbstractItemView::SingleSelection);

      connect (main, SIGNAL (refresh()), SLOT (reloadBodyTree()));
      connect (view_, SIGNAL (customContextMenuRequested(QPoint)), SLOT(customContextMenu(QPoint)));

      toolBox_->removeItem(0);
      addSlider(toolBox_, "Transparency", this, SLOT(setTransparency(int)));
      addSelector (toolBox_, "Visibility",
                   QStringList () << "On" << "Always on top" << "Off",
                   QStringList () << "ON" << "ALWAYS_ON_TOP" << "OFF",
                   this, SLOT(setVisibilityMode(QString)));
      addSelector (toolBox_, "Wireframe mode",
                   QStringList () << "Fill" << "Both" << "Wireframe",
                   QStringList () << "FILL" << "FILL_AND_WIREFRAME" << "WIREFRAME",
                   this, SLOT(setWireFrameMode(QString)));
      addColorSelector(toolBox_, "Color", this, SLOT(setColor(QColor)));
      addSlider(toolBox_, "Scale", this, SLOT(setScale(int)));
    }

    QTreeView* BodyTreeWidget::view ()
    {
      return view_;
    }

    void BodyTreeWidget::selectBodyByName(const QString bodyName)
    {
      QList<QStandardItem*> matches;
      if (!bodyName.isEmpty() && !bodyName.isNull()) {
        matches = model_->findItems(bodyName, Qt::MatchFixedString
            | Qt::MatchCaseSensitive
            | Qt::MatchRecursive);
      }
      if (matches.empty()) {
        qDebug () << "Body" << bodyName << "not found.";
        view_->clearSelection();
      } else {
        view_->setCurrentIndex(matches.first()->index());
      }
    }

    void BodyTreeWidget::selectBodyByName (const std::string& bodyName)
    {
      qDebug () << "Use QString instead of std::string";
      return selectBodyByName (QString::fromStdString (bodyName));
    }

    void BodyTreeWidget::reloadBodyTree()
    {
      model_->clear();
      std::vector <std::string> sceneNames = osg_->getSceneList ();
      for (unsigned int i = 0; i < sceneNames.size(); ++i) {
        graphics::GroupNodePtr_t group = osg_->getScene(sceneNames[i]);
        if (!group) continue;
        addBodyToTree(group);
      }
    }

    void BodyTreeWidget::addBodyToTree(graphics::GroupNodePtr_t group)
    {
      model_->appendRow(new BodyTreeItem (this, group));
    }

    void BodyTreeWidget::customContextMenu(const QPoint &pos)
    {
      QModelIndex index = view_->indexAt(pos);
      if(index.isValid()) {
          BodyTreeItem *item = dynamic_cast <BodyTreeItem*>
              (model_->itemFromIndex(index));
          if (!item) return;
          MainWindow* main = MainWindow::instance ();
          QMenu contextMenu (tr("Node"));
          item->populateContextMenu (&contextMenu);
          QMenu* windows = contextMenu.addMenu(tr("Attach to window"));
          foreach (OSGWidget* w, main->osgWindows ()) {
              JointAction* ja = new JointAction (w->objectName(), item->node()->getID(), windows);
              windows->addAction (ja);
              w->connect(ja, SIGNAL(triggered(std::string)), SLOT(attachToWindow(std::string)));
            }
          contextMenu.exec(view_->mapToGlobal(pos));
        }
    }

    void BodyTreeWidget::changeAlphaValue(const float& alpha)
    {
        QSlider *tr = qobject_cast<QSlider *>(toolBox_->widget(0));

        tr->setValue((int)alpha * 100);
    }

    GEPETTO_GUI_BODYTREE_IMPL_FEATURE (setTransparency, int, int, setAlpha)
    GEPETTO_GUI_BODYTREE_IMPL_FEATURE (setVisibilityMode, QString, CORBA::String_var, setVisibility)
    GEPETTO_GUI_BODYTREE_IMPL_FEATURE (setWireFrameMode, QString, CORBA::String_var, setWireFrameMode)
    GEPETTO_GUI_BODYTREE_IMPL_FEATURE (setColor, QColor, gepetto::corbaserver::Color_var, setColor)
    GEPETTO_GUI_BODYTREE_IMPL_FEATURE (setScale, int, gepetto::corbaserver::Position_var, setScale)
  }
}
