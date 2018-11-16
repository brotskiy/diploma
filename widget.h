#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPointF>
#include <QPen>
#include <QColor>

#include "drawing_struct.h"


#include <QtWidgets>


class Widget : public QWidget
{
    Q_OBJECT

  private:
    const drawing_struct* drw = nullptr;
    const drawing_struct* drw2 = nullptr;

    bool needTrajectory = false;
    bool needParticle = false;

    void drawTrajectory();

  public:
    explicit Widget(QWidget *parent = nullptr);

  protected:
    void paintEvent(QPaintEvent *event)
    {
      Q_UNUSED(event);

      if (needTrajectory)
        drawTrajectory();

    //  if (needParticle)
      //  drawParticle();
    }

  public slots:
    void to_repaint(const drawing_struct& a)
    {
      needTrajectory = true;
      drw = &a;

      update();
    }

};

#endif // WIDGET_H
