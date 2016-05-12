#include "gepetto/gui/ledindicator.hh"

#include <QPainter>
#include <QPalette>
#include <QColor>
#include <QMouseEvent>

namespace gepetto {
  namespace gui {
    LedIndicator::LedIndicator(QWidget *parent):
      QWidget(parent),
      width(28), height(12) {
        setFixedSize(width, height);
        lit = false;
      }

    void LedIndicator::paintEvent(QPaintEvent *) {
      QPainter p(this);
      p.fillRect(0,0,width,height,lit?Qt::green:Qt::red);
    }

    void LedIndicator::mouseReleaseEvent(QMouseEvent *event)
    {
      if (event->button() == Qt::LeftButton) {
        emit mouseClickEvent ();
      }
    }

    void LedIndicator::switchLed() {
      lit = ! lit;
      repaint();
      emit switched(lit);
    }

    void LedIndicator::switchLed(bool on) {
      if (lit == on) return;
      switchLed();
    }
  } // namespace gui
} // namespace gepetto
