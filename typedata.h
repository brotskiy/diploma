#ifndef TYPEDATA_H
#define TYPEDATA_H

#include <QVector>
#include "particle.h"

struct typeData
{
    QVector<QVector<double>> diffs;       // значения всех theta(i) на каждом шаге (в моменты времени)
    QVector<QVector<particle>> coords;     // координаты частиц на каждом шаге. Кол-во частиц любое, но кол-во шагов - как в матрице выше
    QVector<double> time;                  // значение времени на каждом шаге

    // в thetas.at(0), coords.at(0) должны быть записаны векторы начальных значений.
    // в time.at(0) должно быть записано tbegin
};

#endif // TYPEDATA_H
