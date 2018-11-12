#ifndef CELLDIFF_H
#define CELLDIFF_H

#include <QVector>

struct cellDiff  // одно слагаемое из правой части дифф. уравнения из системы SYSALL
{
    double val;              // коэффициент-число
    QVector<int> indices;    // индексы всех theta, из произведения которых и состоит слагаемое

    bool hasL = false;

    // ОБРАТИТЬ ВНИМАНИЕ ПРИ ПРОЕКТИРОВАНИИ ЧТЕНИЯ ИЗ ФАЙЛА НА СЛУЧАЙ theta[i]^n (тэта в некоторой степени)!!!!
    // theta[i]^2 = theta[i] * theta[i]
};

#endif // CELLDIFF_H
