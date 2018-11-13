#ifndef EQ_RHS_CREATION_H
#define EQ_RHS_CREATION_H

// #define MY_PI 3.1415926535
#define MY_PI M_PI
#define MY_ZERO 1e-9

#include <QVector>   // для sin, cos и fabs

struct basisCell    // характеризует одну базисную функцию C * sin(...) * sin(...)
{
  int i, j;          // коэффициенты i и j, на которых строится базисная функция
};

struct psiCell      // характеризует одно слагаемое в уравнении, определяющим зависимость psi_k от различных theta_n
{
  double val;
  int index;

  psiCell() {}
  explicit psiCell(double vl, int indx) : val(vl), index(indx) {}
};

double intSin2D(const int ik, const double bnd);

double intCosSin(const int i, const int ik, const double bnd);

double intSinSin(const int i, const int ik, const double bnd);

double intSub(const int i, const int ii, const double a);
double intSinCosSin(const int i, const int ii, const int ik, const double a);

#endif // EQ_RHS_CREATION_H
