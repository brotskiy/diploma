#include "widget.h"


#include <QtWidgets>

Widget::Widget(QWidget* parent): QWidget(parent)
{
  img = QImage(parent->width(),parent->height(),QImage::Format_ARGB32);
}

void Widget::drawBorders()
{
  QPainter painter(&img);

  painter.setRenderHint(QPainter::Antialiasing);
  painter.translate(10, 10 + drw.bbnd*drw.scale);
  painter.rotate(180);
  painter.scale(-1,1);

  painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
  painter.drawRect(QRectF(0, 0, drw.abnd*drw.scale, drw.bbnd*drw.scale));
}

void Widget::drawParticles(const QVector<particle>& a)
{
  QPainter painter(&img);

  painter.setRenderHint(QPainter::Antialiasing);
  painter.translate(10, 10 + drw.bbnd*drw.scale);
  painter.rotate(180);
  painter.scale(-1,1);

  qsrand(1337);

  for (int part = 0; part < a.size(); part++)
  {
    QColor color(40 + qrand()%180, 40 + qrand()%180, 40 + qrand()%180);
    QPen pen(QBrush(color), 5, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(pen);

    qreal x0 = drw.scale * a.at(part).x;
    qreal y0 = drw.scale * a.at(part).y;
    painter.drawPoint(QPointF(x0,y0));
  }
}

void Widget::drawTrajectory(const QVector<QVector<particle> >& a)
{
  QPainter painter(&img);

  painter.setRenderHint(QPainter::Antialiasing);
  painter.translate(10, 10 + drw.bbnd*drw.scale);
  painter.rotate(180);
  painter.scale(-1,1);

  qsrand(1337);

  for (int part = 0; part < a.at(0).size(); part++)         // в начальном условии есть хотя бы 1 частица
  {
    QColor color(40 + qrand()%180, 40 + qrand()%180, 40 + qrand()%180);
    QPen pen(QBrush(color), 5, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(pen);

    qreal x0 = drw.scale * a.at(0).at(part).x;
    qreal y0 = drw.scale * a.at(0).at(part).y;
    painter.drawPoint(QPointF(x0,y0));

    pen.setWidth(1);
    pen.setCapStyle(Qt::FlatCap);
    painter.setPen(pen);

    for (int step = 0; step < a.size() - 1; step++)
    {
      qreal xi = drw.scale * a.at(step).at(part).x;
      qreal yi = drw.scale * a.at(step).at(part).y;

      qreal xii = drw.scale * a.at(step+1).at(part).x;
      qreal yii = drw.scale * a.at(step+1).at(part).y;

      painter.drawLine(QPointF(xi,yi), QPointF(xii,yii));
    }
  }
}
