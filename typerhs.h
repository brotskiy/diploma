#ifndef TYPERHS_H
#define TYPERHS_H

#include <QVector>
#include "celldiff.h"
#include "cellcoord.h"

struct typeRhs    // правые части всех уравнений системы
{
    QVector<QVector<cellDiff>> diffs;          // правые части для всех theta
    QVector<cellCoord> eqX;              // выражение для нахождения координаты X частицы
    QVector<cellCoord> eqY;              // выражение для нахождения координаты Y частицы
};

#endif // TYPERHS_H
