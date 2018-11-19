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

#include <QVector>

class Widget : public QWidget
{
    Q_OBJECT

  private:
    int particleAmount;
    int current = 0;

    drawing_struct drw;
    QVector<QImage>* img = nullptr;

    void drawTrajectory(const QVector<QVector<particle>>& a);
    void drawBorders();
    void drawParticles(const QVector<particle>& a);

  public:
    explicit Widget(QWidget* parent = nullptr);

    ~Widget();

  protected:
    void paintEvent(QPaintEvent* event)
    {
      Q_UNUSED(event)
      QPainter painter(this);

      if (img != nullptr)
        painter.drawImage(0, 0, img->at(current));
    }

  public slots:
    void setParticleAmont(int prtAm);
    void saveImages() const;

    void setCurrentImg(const QString& cur)
    {
      if (cur.contains("MASTER"))
        current = 0;
      else
        current = cur.toInt();

      update();
    }

    void brdr(const drawing_struct& a)
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

    void prtcls(const QVector<particle>& a)
    {
      drawParticles(a);
      update();
    }

  signals:
    void svImgs();
    void readyForComputations();   // Если очень много точек, то массив может не успеть создасться до того,
                                   // как начнут приходить первые запросы на отрисовку. Поэтому этот сигнал
                                   // скажет движку начинать только после того, как "холсты" будут готовы.
};

#endif // WIDGET_H
