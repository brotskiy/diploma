#ifndef DRAWING_STRUCT_H
#define DRAWING_STRUCT_H

#include <QVector>
#include <QPointF>

struct drawing_struct
{
  double abnd = 0;
  double bbnd = 0;
  double scale = 1;

  drawing_struct& operator=(const drawing_struct& a)
  {
    if (this != &a)
    {
      abnd = a.abnd;
      bbnd = a.bbnd;
      scale = a.scale;
    }

    return *this;
  }

};

#endif // DRAWING_STRUCT_H
