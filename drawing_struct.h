#ifndef DRAWING_STRUCT_H
#define DRAWING_STRUCT_H

#include <QVector>
#include <QPointF>

struct drawing_struct
{
  double abnd = 0, bbnd = 0;
  double scale = 1;

  QVector<QVector<QPointF>> points;
};

#endif // DRAWING_STRUCT_H
