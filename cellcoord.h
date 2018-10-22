#ifndef CELLCOORD_H
#define CELLCOORD_H

#include <QVector>

struct cellCoord    // одно слагаемое из правой части дифф. уравнения для нахождения координаты частицы
{
    double val;                // коэффициент-число
    int index;                 // индексы одной theta из этого слагаемого. Если будет несколько, то надо будет поменять на массив.
    double valSin, valCos;     // коэффициенты в двух множителях: синусе и косинусе
};

#endif // CELLCOORD_H
