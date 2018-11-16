#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPointF>
#include <QPen>
#include <QColor>

#include "drawing_struct.h"
#include "particle.h"

#include <QtWidgets>

class Widget : public QWidget
{
    Q_OBJECT

  private:
    drawing_struct drw;

    QImage img;

    void drawTrajectory(const QVector<QVector<particle>>& a);
    void drawBorders();

  public:
    explicit Widget(QWidget* parent = nullptr);

  protected:
    void paintEvent(QPaintEvent* event)
    {
      Q_UNUSED(event)
      QPainter painter(this);
      painter.drawImage(0, 0, img);
    }

  public slots:
    void brdr(drawing_struct a)
    {
      drw = a;
      drawBorders();
      update();
    }

    void crv(const QVector<QVector<particle>>& a)
    {
      drawTrajectory(a);
      update();
    }

};

#endif // WIDGET_H
