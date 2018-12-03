#include "runge-kutta.h"

#include <QtWidgets>
#include <QFileDialog>

void evalDiffK(const typeRhs& rhs, typeData& data, const int equationAmount, const int step, const double L,
               const int KNum, const int addNum, const double coeffK, const double coeffKResult, QVector<QVector<double>>& KThetasAtStep)
{
  const QVector<double>& dataDiffAtPrevStep = data.diffs.at(step-1);     // значения всех theta с предыдущего шага

#pragma omp parallel for default(shared)
    for (int eqNum = 0; eqNum < equationAmount; eqNum++)       // перебираем все уравнения системы
    {
      const QVector<cellDiff>& rhsOneEq = rhs.diffs[eqNum];    // правая часть одного уравнения с соответствующим номером

      KThetasAtStep[KNum][eqNum] = 0;                          // очистили мусор для накопления суммы

      for (int i = 0; i < rhsOneEq.size(); i++)                // перебираем все слагаемые в правой части некого уравнения системы
      {
        double tmp = 1;
                                                             // произведение всех theta (+добавка), из которых состоит слагаемое
        for (int j = 0; j < rhsOneEq[i].indices.size(); j++)
          tmp *= (dataDiffAtPrevStep[rhsOneEq[i].indices[j]] + coeffK * KThetasAtStep[addNum][rhsOneEq[i].indices[j]]);

        if (rhsOneEq[i].hasL)                                  // если слагаемое имеет число Рэлея
          tmp *= L;                                            // то умножаем на него

        KThetasAtStep[KNum][eqNum] += tmp * rhsOneEq[i].val;   // не забываем умножить на число-коэффициент и накапливаем слагаемые дальше
      }

      if ( KNum == 1-1)      // Если считается коэффициент K1 (в С++ нумерация от 0)
        data.diffs[step][eqNum] = dataDiffAtPrevStep[eqNum] + coeffKResult * KThetasAtStep[KNum][eqNum];
      else
        data.diffs[step][eqNum] += coeffKResult * KThetasAtStep[KNum][eqNum];
    }
}

void evalDiffAtStep(const typeRhs& rhs, typeData& data, const int equationAmount, const int step,
                    const double h, const double L, QVector<QVector<double>>& KThetasAtStep)
{
  // для K1 нулевая добавка. в качестве вектора добавок можно ему передать его же
  evalDiffK(rhs, data, equationAmount, step, L, 1-1, 1-1, 0, h/6, KThetasAtStep);

  evalDiffK(rhs, data, equationAmount, step, L, 2-1, 1-1, h/2, h/6 * 2, KThetasAtStep);

  evalDiffK(rhs, data, equationAmount, step, L, 3-1, 2-1, h/2, h/6*2, KThetasAtStep);

  evalDiffK(rhs, data, equationAmount, step, L, 4-1, 3-1, h, h/6, KThetasAtStep);

//  double tmp = data.diffs.at(step-1).at(eqNum) + h/6*(K1.at(eqNum) + 2*K2.at(eqNum) + 2*K3.at(eqNum) + K4.at(eqNum)); !!!!!!!!!!!!!!!!!!!!!!!!
}

void evalCoordK(const typeRhs& rhs, const typeData& data, const int particleNumber, const int step,
                QPair<double, double>& KEq, const QVector<double>& KTheta)
{
  const QVector<cellCoord>& rhsEqX = rhs.eqX;                             // правая часть уравнения для нахождения координаты X
  const QVector<cellCoord>& rhsEqY = rhs.eqY;                             // правая часть уравнения для нахождения координаты Y

  const QVector<particle>& dataCoordAtStep = data.coords.at(step-1);      // значения всех координат с предыдущего шага
  const QVector<double>& dataDiffAtStep = data.diffs.at(step-1);          // значения всех theta с предыдущего шага

  double tmpX = 0;
  double tmpY = 0;

  #pragma omp parallel default(shared)
  {
    #pragma omp for reduction (+: tmpX)
    for (int i = 0; i < rhsEqX.size(); i++)   // идем по всем слагаемым уравнения для X
    {
      const double tmpsin = sin(rhsEqX[i].valSin * (dataCoordAtStep[particleNumber].x + KEq.first));            // выражение внутри синуса и косинуса
      const double tmpcos = cos(rhsEqX[i].valCos * (dataCoordAtStep[particleNumber].y + KEq.second));
      tmpX += rhsEqX[i].val * (dataDiffAtStep[rhsEqX[i].index] + KTheta[rhsEqX[i].index]) * tmpsin * tmpcos;    // накапливаем, theta + добавка
    }

    #pragma omp for reduction(+: tmpY)
    for (int i = 0; i < rhsEqY.size(); i++)   // идем по всем слагаемым уравнения для Y
    {
      const double tmpcos = cos(rhsEqY[i].valCos * (dataCoordAtStep[particleNumber].x + KEq.first));
      const double tmpsin = sin(rhsEqY[i].valSin * (dataCoordAtStep[particleNumber].y + KEq.second));
      tmpY += rhsEqY[i].val * (dataDiffAtStep[rhsEqY[i].index] + KTheta[rhsEqY[i].index]) * tmpcos * tmpsin;    // theta + добавка
    }
  }

  KEq.first = tmpX;
  KEq.second = tmpY;
}

void evalCoordAtStep(const typeRhs& rhs, typeData& data, const int particleAmount, const int step,
                     const double h, const QVector<QVector<double>>& KThetasAtStep)
{
  for (int partNum = 0; partNum < particleAmount; partNum++)   // по очереди высчитываем координаты всех точек
  {
    QPair<double, double> K1(0, 0);  // сначала в K хранится добавка, а потом само значение коэффициента
    evalCoordK(rhs, data, partNum, step, K1, KThetasAtStep[1-1]);

    QPair<double, double> K2(h/2 * K1.first, h/2 * K1.second);
    evalCoordK(rhs, data, partNum, step, K2, KThetasAtStep[2-1]);

    QPair<double, double> K3(h/2 * K2.first, h/2 * K2.second);
    evalCoordK(rhs, data, partNum, step, K3, KThetasAtStep[3-1]);

    QPair<double, double> K4(h * K3.first, h * K3.second);
    evalCoordK(rhs, data, partNum, step, K4, KThetasAtStep[4-1]);

    data.coords[step][partNum].x = data.coords[step-1][partNum].x + h/6*(K1.first + 2*K2.first + 2*K3.first + K4.first);       // оценка X по рунге-кутту
    data.coords[step][partNum].y = data.coords[step-1][partNum].y + h/6*(K1.second + 2*K2.second + 2*K3.second + K4.second);   // оценка Y по рунге-кутту
  }
}

void rk4step(const typeRhs& rhs, typeData& data, const double tbegin, const double h, const int step,
             const int equationAmount, const int particleAmount, const double L, QVector<QVector<double>>& KThetasAtStep)
{
  data.time[step] = tbegin + step*h;                                         // значение времени на текущем шаге

  evalDiffAtStep(rhs, data, equationAmount, step, h, L, KThetasAtStep);      // вычисляем все theta на текущем шаге
  evalCoordAtStep(rhs, data, particleAmount, step, h, KThetasAtStep);        // вычисляем координаты точек на текущем шаге
}
