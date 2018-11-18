#ifndef RUNGEKUTTA_H
#define RUNGEKUTTA_H

#include <QVector>
#include <QPair>

#include "typerhs.h"
#include "typedata.h"

void evalDiffK(const typeRhs& rhs, const typeData& data, const int equationAmount, const int step,
               const double L, QVector<double>& K, const QVector<double>& add, const double coeffK);

QVector<double> evalDiffAtStep(const typeRhs& rhs, const typeData& data, const int equationAmount, const int step,
                               const double h, const double L, QVector<QVector<double>>& tmpAddDiff);


QPair<double, double> evalCoordK(const typeRhs& rhs, const typeData& data, const int particleNumber, const int step,
                                 const double addX, const double addY, const QVector<double>& addDiffForK);

QVector<particle> evalCoordAtStep(const typeRhs& rhs, const typeData& data, const int particleAmount, const int step,
                                  const double h, const QVector<QVector<double>>& addDiff);


void rk4step(const typeRhs& rhs, typeData& data, const double tbegin, const double h,
             const int step, const int equationAmount, const int particleAmount, const double L);

#endif // RUNGEKUTTA_H
