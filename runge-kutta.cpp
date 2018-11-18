#include "runge-kutta.h"

#include <QtWidgets>
#include <QFileDialog>


void evalDiffK(const typeRhs& rhs, typeData& data, const int equationAmount, const int step,
               const double L, QVector<double>& K, const QVector<double>& add, const double coeffK, const int KNumber, const double coeffKResult)
{
  const QVector<double>& dataDiffAtPrevStep = data.diffs.at(step-1);             // значения всех theta с предыдущего шага

  for (int eqNum = 0; eqNum < equationAmount; eqNum++)         // перебираем все уравнения системы
  {  
    const QVector<cellDiff>& rhsOneEq = rhs.diffs.at(eqNum);          // правая часть одного уравнения с соответствующим номером

    for (int i = 0; i < rhsOneEq.size(); i++)                  // перебираем все слагаемые в правой части некого уравнения системы
    {
      double tmp = 1;

      for (int j = 0; j < rhsOneEq[i].indices.size(); j++)
        tmp *= (dataDiffAtPrevStep[rhsOneEq[i].indices[j]] + coeffK * add.at(rhsOneEq[i].indices[j]));   // произведение всех theta (+добавка), из которых состоит слагаемое

      if (rhsOneEq.at(i).hasL)                    // если слагаемое имеет число Рэлея
        tmp *= L;                                 // то умножаем на него

      K[eqNum] += tmp * rhsOneEq[i].val;                         // не забываем умножить на число-коэффициент и накапливаем слагаемые дальше
    }

    if ( KNumber == 1)
      data.diffs[step][eqNum] += dataDiffAtPrevStep.at(eqNum) + coeffKResult * K.at(eqNum);
    else
      data.diffs[step][eqNum] += coeffKResult * K.at(eqNum);
  }

  // конец функции ------------------------------------------------
}

void evalDiffAtStep(const typeRhs& rhs, typeData& data, const int equationAmount, const int step,
                               const double h, const double L, QVector<QVector<double>>& tmpAddDiff)
{
  QVector<double> K1(equationAmount, 0);
  QVector<double> K2(equationAmount, 0);
  evalDiffK(rhs, data, equationAmount, step, L, K1, K2, 0, 1, h/6);

  evalDiffK(rhs, data, equationAmount, step, L, K2, K1, h/2, 2, h/6 * 2);

  QVector<double> K3(equationAmount, 0);
  evalDiffK(rhs, data, equationAmount, step, L, K3, K2, h/2, 3, h/6 * 2);

  QVector<double> K4(equationAmount, 0);
  evalDiffK(rhs, data, equationAmount, step, L, K4, K3, h, 4, h/6);


 // for (int eqNum = 0; eqNum < equationAmount; eqNum++)    // вычисление оценки всех theta на текущем шаге
  //{
       //  double tmp = data.diffs.at(step-1).at(eqNum) + h/6*(K1.at(eqNum) + 2*K2.at(eqNum) + 2*K3.at(eqNum) + K4.at(eqNum)); !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //currentData.push_back(tmp);
  //}

  tmpAddDiff.append(K1);
  tmpAddDiff.append(K2);
  tmpAddDiff.append(K3);
  tmpAddDiff.append(K4);
}

QPair<double, double> evalCoordK(const typeRhs& rhs, const typeData& data, const int particleNumber, const int step,
                                 const double addX, const double addY, const QVector<double>& addDiffForK)
{
  const QVector<cellCoord>& rhsEqX = rhs.eqX;                             // правая часть уравнения для нахождения координаты X
  const QVector<cellCoord>& rhsEqY = rhs.eqY;                             // правая часть уравнения для нахождения координаты Y

  const QVector<particle>& dataCoordAtStep = data.coords.at(step-1);      // значения всех координат с предыдущего шага
  const QVector<double>& dataDiffAtStep = data.diffs.at(step-1);          // значения всех theta с предыдущего шага

  QPair<double, double> result(0,0);

  for (int i = 0; i < rhsEqX.size(); i++)   // идем по всем слагаемым уравнения для X
  {
    double tmpsin = sin(rhsEqX[i].valSin * (dataCoordAtStep[particleNumber].x + addX));         // выражение внутри синуса и косинуса
    double tmpcos = cos(rhsEqX[i].valCos * (dataCoordAtStep[particleNumber].y + addY));
    double tmp = rhsEqX[i].val * (dataDiffAtStep[rhsEqX[i].index] + addDiffForK.at(rhsEqX[i].index)) * tmpsin * tmpcos;      // theta + добавка !!!!!!!!!!!!!!!!!!!!!!!!!!

    result.first += tmp;        // накапливаем
  }

  for (int i = 0; i < rhsEqY.size(); i++)   // идем по всем слагаемым уравнения для Y
  {
    double tmpcos = cos(rhsEqY[i].valCos * (dataCoordAtStep[particleNumber].x + addX));
    double tmpsin = sin(rhsEqY[i].valSin * (dataCoordAtStep[particleNumber].y + addY));
    double tmp = rhsEqY[i].val * (dataDiffAtStep[rhsEqY[i].index] + addDiffForK.at(rhsEqY[i].index)) * tmpcos * tmpsin;      // theta + добавка !!!!!!!!!!!!!!!!!!!!!!!!!!

    result.second += tmp;
  }

  return result;
}

QVector<particle> evalCoordAtStep(const typeRhs& rhs, const typeData& data, const int particleAmount, const int step,
                                  const double h, const QVector<QVector<double>>& addDiff)
{
  QVector<particle> currentData;  // искомые значения всех координат на данном шаге

  for (int partNum = 0; partNum < particleAmount; partNum++)   // по очереди высчитываем координаты всех точек
  {
    double addX = 0;
    double addY = 0;
    QPair<double, double> K1 = evalCoordK(rhs, data, partNum, step, addX, addY, addDiff.at(1-1));

    addX = h/2 * K1.first;
    addY = h/2 * K1.second;
    QPair<double, double> K2 = evalCoordK(rhs, data, partNum, step, addX, addY, addDiff.at(2-1));

    addX = h/2 * K2.first;
    addY = h/2 * K2.second;
    QPair<double, double> K3 = evalCoordK(rhs, data, partNum, step, addX, addY, addDiff.at(3-1));

    addX = h * K3.first;
    addY = h * K3.second;
    QPair<double, double> K4 = evalCoordK(rhs, data, partNum, step, addX, addY, addDiff.at(4-1));

    particle tmp;
    tmp.x = data.coords.at(step-1).at(partNum).x + h/6*(K1.first + 2*K2.first + 2*K3.first + K4.first);       // оценка X по рунге-кутту
    tmp.y = data.coords.at(step-1).at(partNum).y + h/6*(K1.second + 2*K2.second + 2*K3.second + K4.second);   // оценка Y по рунге-кутту

    currentData.push_back(tmp);
  }

  return currentData;
}

void rk4step(const typeRhs& rhs, typeData& data, const double tbegin, const double h,
             const int step, const int equationAmount, const int particleAmount, const double L)
{
  data.time[step] = tbegin + step*h;       // значение времени на текущем шаге

  QVector<QVector<double>> tmpAddDiff; // прибавка к аргументу theta_i для каждого из коэффициентов K

  evalDiffAtStep(rhs, data, equationAmount, step, h, L,tmpAddDiff);      // вычисляем все theta на текущем шаге
  QVector<particle> tmpCoord = evalCoordAtStep(rhs, data, particleAmount, step, h, tmpAddDiff);    // вычисляем координаты точек на текущем шаге

  data.coords.push_back(tmpCoord);
}
