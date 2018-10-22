#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPointF>

#include <QVector>

class Widget : public QWidget
{
    Q_OBJECT


  private:
   QVector<QPointF> points;
  public:
   explicit Widget(QWidget *parent = nullptr);

  protected:

    void paintEvent(QPaintEvent *event)
    {
      Q_UNUSED(event);

      QPainter painter(this);

      painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

     // painter.drawRect(300,300, 600, 600); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      for (int i = 0; i < points.size()-1; i++)
        painter.drawLine(points.at(i), points.at(i+1));
    }

  public slots:
    void to_repaint(const QVector<QPointF>& a)
    {
      points = a;

      repaint();
    }

};

#endif // WIDGET_H
