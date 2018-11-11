#ifndef EQ_RHS_CREATION_H
#define EQ_RHS_CREATION_H

#define MY_PI 3.1415926535

#include <QVector>                 // для sin и cos

double intSin2D(const int ik, const double bnd);

double intCosSin(const int i, const int ik, const double bnd);

double intSinSin(const int i, const int ik, const double bnd);

double intSub(const int i, const int ii, const double a);
double intSinCosSin(const int i, const int ii, const int ik, const double a);

#endif // EQ_RHS_CREATION_H
