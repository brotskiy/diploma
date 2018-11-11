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
   QVector<QVector<QPointF>> points;
  public:
   explicit Widget(QWidget *parent = nullptr);

  protected:

    void paintEvent(QPaintEvent *event)
    {
      Q_UNUSED(event);

      QPainter painter(this);

      painter.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap));
      painter.drawRect(100, 100, 2*100+100, 5*100+100); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

      for (int i = 0; i < points.size(); i++)
        for (int j = 0; j < points.at(i).size()-1; j++)
         painter.drawLine(points.at(i).at(j), points.at(i).at(j+1));
    }

  public slots:
    void to_repaint(const QVector<QVector<QPointF>>& a)
    {
      points = a;

      repaint();
    }

};

#endif // WIDGET_H
