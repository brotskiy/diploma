#include "widget.h"


#include <QtWidgets>

Widget::Widget(QWidget* parent): QWidget(parent)
{
}

void Widget::drawTrajectory()
{
  QPainter painter(this);

  painter.setRenderHint(QPainter::Antialiasing);
  painter.translate(10, 10 + drw->bbnd*drw->scale);
  painter.rotate(180);
  painter.scale(-1,1);

  painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
  painter.drawRect(QRectF(0, 0, drw->abnd*drw->scale, drw->bbnd*drw->scale));

  for (int i = 0; i < drw->points.size(); i++)
  {
    QColor color(40 + rand()%180, 40 + rand()%180, 40 + rand()%180);
    QPen pen(QBrush(color), 5, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(pen);

    painter.drawPoint(drw->points.at(i).at(0));

    pen.setWidth(1);
    pen.setCapStyle(Qt::FlatCap);
    painter.setPen(pen);

    for (int j = 0; j < drw->points.at(i).size()-1; j++)
      painter.drawLine(drw->points.at(i).at(j), drw->points.at(i).at(j+1));
  }

  // needTrajectory = false;       // на всякий случай сбрасываем флаг, т.к. выполнили все необходимые действия.
}
