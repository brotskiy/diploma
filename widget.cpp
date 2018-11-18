#include "widget.h"


#include <QtWidgets>

Widget::Widget(QWidget* parent): QWidget(parent)
{  
}

Widget::~Widget()
{
  if (img != nullptr)  // зачем лишний раз удалять, если и так нету
    delete img;
}

void Widget::setParticleAmont(int prtAm)
{
  particleAmount = prtAm;

  img = new QVector<QImage>(1 + particleAmount, QImage(1024,768,QImage::Format_ARGB32));

  emit readyForComputations();
}

void Widget::drawBorders()
{
  for (int part = 0; part < 1 + particleAmount; part++)
  {
    QPainter painter(&((*img)[part]));

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(10, 10 + drw.bbnd*drw.scale);
    painter.rotate(180);
    painter.scale(-1,1);

    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(QRectF(0, 0, drw.abnd*drw.scale, drw.bbnd*drw.scale));
  }
}

void Widget::drawParticles(const QVector<particle>& a)
{
  QPainter painterM(&((*img)[0]));

  painterM.setRenderHint(QPainter::Antialiasing);
  painterM.translate(10, 10 + drw.bbnd*drw.scale);
  painterM.rotate(180);
  painterM.scale(-1,1);

  qsrand(1337);

  for (int part = 0; part < particleAmount; part++)   // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  {
    QPainter painterP(&((*img)[1 + part]));

    painterP.setRenderHint(QPainter::Antialiasing);
    painterP.translate(10, 10 + drw.bbnd*drw.scale);
    painterP.rotate(180);
    painterP.scale(-1,1);

    QColor color(40 + qrand()%180, 40 + qrand()%180, 40 + qrand()%180);
    QPen pen(QBrush(color), 3, Qt::SolidLine, Qt::RoundCap);

    painterM.setPen(pen);
    painterP.setPen(pen);

    qreal x0 = drw.scale * a.at(part).x;
    qreal y0 = drw.scale * a.at(part).y;

    painterM.drawPoint(QPointF(x0,y0));
    painterP.drawPoint(QPointF(x0,y0));
  }
}

void Widget::drawTrajectory(const QVector<QVector<particle> >& a)
{
  for (int part = 0; part < 1 + particleAmount; part++)
  {
    (*img)[part] = QImage(1024,768,QImage::Format_ARGB32);
  }

  drawBorders();

  QPainter painterM(&((*img)[0]));

  painterM.setRenderHint(QPainter::Antialiasing);
  painterM.translate(10, 10 + drw.bbnd*drw.scale);
  painterM.rotate(180);
  painterM.scale(-1,1);

  qsrand(1337);

  for (int part = 0; part < particleAmount; part++)         // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  {
    QPainter painterP(&((*img)[1 + part]));

    painterP.setRenderHint(QPainter::Antialiasing);
    painterP.translate(10, 10 + drw.bbnd*drw.scale);
    painterP.rotate(180);
    painterP.scale(-1,1);

    QColor color(40 + qrand()%180, 40 + qrand()%180, 40 + qrand()%180);
    QPen pen(QBrush(color), 5, Qt::SolidLine, Qt::RoundCap);

    painterM.setPen(pen);
    painterP.setPen(pen);

    qreal x0 = drw.scale * a.at(0).at(part).x;
    qreal y0 = drw.scale * a.at(0).at(part).y;

    painterM.drawPoint(QPointF(x0,y0));
    painterP.drawPoint(QPointF(x0,y0));

    pen.setWidth(1);
    pen.setCapStyle(Qt::FlatCap);

    painterM.setPen(pen);
    painterP.setPen(pen);

    for (int step = 0; step < a.size() - 1; step++)
    {
      qreal xi = drw.scale * a.at(step).at(part).x;
      qreal yi = drw.scale * a.at(step).at(part).y;

      qreal xii = drw.scale * a.at(step+1).at(part).x;
      qreal yii = drw.scale * a.at(step+1).at(part).y;

      painterM.drawLine(QPointF(xi,yi), QPointF(xii,yii));
      painterP.drawLine(QPointF(xi,yi), QPointF(xii,yii));
    }
  }
}
