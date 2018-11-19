#include "widget.h"


#include <QtWidgets>

Widget::Widget(QWidget* parent): QWidget(parent)
{  
  connect(this, &Widget::svImgs, &Widget::saveImages);
}

Widget::~Widget()
{
  if (img != nullptr)  // зачем лишний раз удалять, если и так нету
    delete img;
}

void Widget::setParticleAmont(int prtAm)
{
  particleAmount = prtAm;

  if (img != nullptr) // очищаем холсты, если на них уже есть изображения
    delete img;

  img = new QVector<QImage>(1 + particleAmount, QImage(750,750,QImage::Format_ARGB32));

  emit readyForComputations();
}

void Widget::drawBorders()
{
  for (int part = 0; part < 1 + particleAmount; part++)
  {
    (*img)[part].fill(Qt::white);

    QPainter painter(&((*img)[part]));

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(750/2-static_cast<int>(drw.abnd*drw.scale/2),353+static_cast<int>(drw.bbnd*drw.scale/2));
    painter.scale(1,-1);

    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(QRectF(0, 0, drw.abnd*drw.scale, drw.bbnd*drw.scale));
  }
}

void Widget::drawParticles(const QVector<particle>& a)
{
  QPainter painterM(&((*img)[0]));

  painterM.setRenderHint(QPainter::Antialiasing);
  painterM.translate(750/2-static_cast<int>(drw.abnd*drw.scale/2),353+static_cast<int>(drw.bbnd*drw.scale/2));
  painterM.scale(1,-1);

  qsrand(1337);

  for (int part = 0; part < particleAmount; part++)
  {
    QPainter painterP(&((*img)[1 + part]));

    painterP.setRenderHint(QPainter::Antialiasing);
    painterP.translate(750/2-static_cast<int>(drw.abnd*drw.scale/2),353+static_cast<int>(drw.bbnd*drw.scale/2));
    painterP.scale(1,-1);

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
  //for (int part = 0; part < 1 + particleAmount; part++)
  //{
    //(*img)[part] = QImage(750,750,QImage::Format_ARGB32);
  //}

  drawBorders();

  QPainter painterM(&((*img)[0]));

  painterM.setRenderHint(QPainter::Antialiasing);
  painterM.translate(750/2-static_cast<int>(drw.abnd*drw.scale/2),353+static_cast<int>(drw.bbnd*drw.scale/2));
  painterM.scale(1,-1);

  qsrand(1337);

  for (int part = 0; part < particleAmount; part++)
  {
    QPainter painterP(&((*img)[1 + part]));

    painterP.setRenderHint(QPainter::Antialiasing);
    painterP.translate(750/2-static_cast<int>(drw.abnd*drw.scale/2),353+static_cast<int>(drw.bbnd*drw.scale/2));
    painterP.scale(1,-1);

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

  emit svImgs();
}

void Widget::saveImages() const
{
  QDir dir;
  dir.mkdir("output/trajectory");

  for (int part = 0; part < 1 + particleAmount; part++)
  {
   // (*img)[part] = (*img)[part].convertToFormat(QImage::Format_ARGB32_Premultiplied);

    if (part == 0)
      (*img)[part].save("output/trajectory/master.png", "PNG");
    else
      (*img)[part].save("output/trajectory/prt" + QString::number(part) + ".png", "PNG");
  }
}
