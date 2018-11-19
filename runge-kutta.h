#ifndef RUNGEKUTTA_H
#define RUNGEKUTTA_H

#include <QVector>
#include <QPair>

#include "typerhs.h"
#include "typedata.h"

void evalDiffK(const typeRhs& rhs, typeData& data, const int equationAmount, const int step, const double L,
               const int KNum, const int addNum, const double coeffK, const double coeffKResult, QVector<QVector<double> >& KThetasAtStep);

void evalDiffAtStep(const typeRhs& rhs, typeData& data, const int equationAmount, const int step,
                    const double h, const double L, QVector<QVector<double>>& KThetasAtStep);


void evalCoordK(const typeRhs& rhs, const typeData& data, const int particleNumber, const int step,
                QPair<double, double>& KEq, const QVector<double>& KTheta);

void evalCoordAtStep(const typeRhs& rhs, typeData& data, const int particleAmount, const int step,
                     const double h, const QVector<QVector<double>>& KThetasAtStep);


void rk4step(const typeRhs& rhs, typeData& data, const double tbegin, const double h, const int step,
             const int equationAmount, const int particleAmount, const double L, QVector<QVector<double>>& KThetasAtStep);

#endif // RUNGEKUTTA_H
