#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPointF>

#include <QVector>

#include "drawing_struct.h"

#include <QtWidgets>

class Widget : public QWidget
{
    Q_OBJECT


  private:
   //QVector<QVector<QPointF>> points;
    drawing_struct drw;

  public:
   explicit Widget(QWidget *parent = nullptr);

  protected:

    void paintEvent(QPaintEvent *event)
    {
      Q_UNUSED(event);

      QPainter painter(this);

      painter.setRenderHint(QPainter::Antialiasing);
      painter.translate(10, 10 + drw.bbnd*drw.scale);
      painter.rotate(180);
      painter.scale(-1,1);

      painter.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap));
      painter.drawRect(QRectF(0, 0, drw.abnd*drw.scale, drw.bbnd*drw.scale)); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

      for (int i = 0; i < drw.points.size(); i++)
        for (int j = 0; j < drw.points.at(i).size()-1; j++)
          painter.drawLine(drw.points.at(i).at(j), drw.points.at(i).at(j+1));
    }

  public slots:
    void to_repaint(const drawing_struct& a)
    {
      drw = a;

      repaint();
    }

};

#endif // WIDGET_H
