#include "mainengine.h"

#include <QtWidgets> // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void MainEngine::readFromFile(const QString& fileName)
{
  QFile file(fileName);

  if (file.open(QIODevice::ReadOnly))
  {
    QTextStream stream(&file);

    QString tmp = stream.readLine();
    QStringList tmpLst = tmp.split(" ");

    abnd = tmpLst.at(0).toDouble();
    bbnd = tmpLst.at(1).toDouble();

    tmp = stream.readLine();
    tmpLst = tmp.split(" ");

    tbegin = tmpLst.at(0).toDouble();
    tend = tmpLst.at(1).toDouble();

    equationAmount = stream.readLine().toInt();
    particleAmount = stream.readLine().toInt();
    stepAmount = stream.readLine().toInt();
    L = stream.readLine().toDouble();                // Число Рэлея !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    for (int i = 0; i <equationAmount; i++)
    {
      QVector<cellDiff> tmpVec;

      tmp = stream.readLine();
      tmpVec = proccessOneDiff(tmp);
      rhs.diffs.append(tmpVec);
    }

    QVector<cellCoord> tmpVec;

    tmp = stream.readLine();
    tmpVec = proccessOneCoord(tmp);
    rhs.eqX = tmpVec;

    tmp = stream.readLine();
    tmpVec = proccessOneCoord(tmp);
    rhs.eqY = tmpVec;

    // --------- теперь читаются начальные данные ------------------

    tmp = stream.readLine();
    tmpLst = tmp.split(" ");

    QVector<double> tmpVec2;
    for (int i = 0; i < equationAmount; i++)
      tmpVec2.append(tmpLst[i].toDouble());

    data.diffs.append(tmpVec2);

    QVector<particle> tmpVec3;
    for (int i = 0; i < particleAmount; i++)
    {
      particle tmpPrt;

      tmp = stream.readLine();
      tmpLst = tmp.split(" ");
      tmpPrt.x = tmpLst.at(0).toDouble();
      tmpPrt.y = tmpLst.at(1).toDouble();

      tmpVec3.append(tmpPrt);
    }

    data.coords.append(tmpVec3);
    data.time.append(tbegin);

    file.close();
  }
}

void MainEngine::rk4()
{
  rk4full(rhs,data,tbegin,tend,stepAmount,equationAmount,particleAmount, L);   // с Числом Рэлея !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

void MainEngine::crtBas(QVector<basisCell>& basis)
{
  int cntr = 0;   // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  for (int i = 1; i <= sqrt(equationAmount); i++)
    for (int j = 1; j <= sqrt(equationAmount); j++)
    {
      basis[cntr].i = i;
      basis[cntr].j = j;
      cntr++;
    }
}

void MainEngine::crtEq1(const QVector<basisCell>& basis, QVector<QVector<psiCell>>& psi)
{
  for (int k = 0; k < equationAmount; k++)
  {
    const int ik = basis[k].i;
    const int jk = basis[k].j;

    const double xpr1 = (-1*ik*ik/(abnd*abnd) - jk*jk/(bbnd*bbnd)) * MY_PI*MY_PI * 4/(abnd*bbnd) * intSin2D(ik, abnd) * intSin2D(jk, bbnd);

    for (int n = 0; n < equationAmount; n++)
    {
      const int i = basis[n].i;
      const int j = basis[n].j;

      const double xpr2 = -1*i/abnd * MY_PI * 4/(abnd*bbnd) * intCosSin(i, ik, abnd) * intSinSin(j, jk, bbnd);

      if (fabs(xpr2/xpr1) > MY_ZERO)
        psi[k].append(psiCell(-1 * xpr2/xpr1, n));
    }
  }
}

void MainEngine::crtEq2Sub32(const QVector<QVector<psiCell>>& psi, const double xpr1, const int ik, const int jk,
                            const int i, const int j, const int ii, const int jj, const int n, const int k, const int count)
{
  const double xpr2 = 2/(sqrt(abnd*bbnd)) * MY_PI*MY_PI * j/bbnd * ii/abnd * intSinCosSin(i,ii,ik,abnd) * intSinCosSin(jj,j,jk,bbnd);
  const double xpr3 = 2/(sqrt(abnd*bbnd)) * MY_PI*MY_PI * i/abnd * jj/bbnd * intSinCosSin(ii,i,ik,abnd) * intSinCosSin(j,jj,jk,bbnd);

  if (fabs(xpr3-xpr2) > MY_ZERO)                        // не 0 ли числитель?
  {
    const double diff = xpr3 - xpr2;

    if (fabs(diff/xpr1) > MY_ZERO)                      // стоит ли добавлять это слагаемое (не 0 ли оно)?
    {
      const double coeff = diff / xpr1;                 // считаем коэффициент, стоящий перед интегралами при взятии проекции

      for (int t = 0; t < psi.at(k).size(); t++)        // смотрим eq1: перебираем все theta, из суммы которых состоит текущее psi_k
      {
        cellDiff tmp;                                   // создаем член результирующей системы: d(theta_1)/dt = A*theta_1*theta_2 + B*theta_3 + ...

        tmp.val = coeff * psi.at(k).at(t).val;          // его коэфф. - это коэфф. проекции умножить на соответствующий коефф. при theta в eq1
        tmp.indices.append(n);                          // добавляем в член номер текущего theta_n из формулы проекции
        tmp.indices.append(psi.at(k).at(t).index);      // добавляем номер theta из уравнения eq1, соответствующего текущему psi_k

        rhs.diffs[count].append(tmp);                   // добавляем полученный член в модель к соответствующему уравнению du/dt = ...
      }
    }
  }
}

void MainEngine::crtEq2Sub5(const QVector<QVector<psiCell>>& psi, const double xpr1, const int ik, const int jk,
                            const int i, const int j, const int k, const int count)
{
  const double xpr5 = i * MY_PI/abnd * intCosSin(i,ik,abnd) * intSinSin(j,jk,bbnd);

  if (fabs(xpr5/xpr1) > MY_ZERO)
  {
    const double coeff = xpr5 / xpr1;

    for (int t = 0; t < psi.at(k).size(); t++)
    {
      cellDiff tmp;

      tmp.hasL = true;
      tmp.val = coeff * psi.at(k).at(t).val;
      tmp.indices.append(psi.at(k).at(t).index);

      rhs.diffs[count].append(tmp);
    }
  }
}

void MainEngine::crtEq2Sub4(const double xpr1, const int ik, const int jk, const int count)
{
  const double xpr4 = MY_PI*MY_PI * (ik*ik/(abnd*abnd) + jk*jk/(bbnd*bbnd)) * intSin2D(ik,abnd) * intSin2D(jk,bbnd);

  if (fabs(xpr4/xpr1) > MY_ZERO)       // по идее тут должны сокращаться интегралы
  {
    const double coeff = -1 * xpr4 / xpr1;

    cellDiff tmp;

    tmp.val = coeff;
    tmp.indices.append(count);

    rhs.diffs[count].append(tmp);
  }
}

void MainEngine::crtEq2(const QVector<basisCell>& basis, const QVector<QVector<psiCell>>& psi)
{
  for (int count = 0; count < equationAmount; count++)  // идем по производным
  {
    const int ik = basis[count].i;
    const int jk = basis[count].j;

    const double xpr1 = intSin2D(ik, abnd) * intSin2D(jk, bbnd);

    crtEq2Sub4(xpr1, ik, jk, count);

    for (int k = 0; k < equationAmount; k++)     // psi
    {
      const int i = basis[k].i;
      const int j = basis[k].j;

      crtEq2Sub5(psi, xpr1, ik, jk, i, j, k, count);

      for (int n = 0; n < equationAmount; n++)   // theta
      {
        const int ii = basis[n].i;
        const int jj = basis[n].j;

        crtEq2Sub32(psi, xpr1, ik, jk, i, j, ii, jj, n, k, count);
      }
    }
  }
}

void MainEngine::crtCoords(const QVector<basisCell>& basis, const QVector<QVector<psiCell>>& psi)
{
  for (int k = 0; k < equationAmount; k++)
  {
    const int i = basis[k].i;
    const int j = basis[k].j;

    for (int t = 0; t < psi.at(k).size(); t++)
    {
      cellCoord tmpx, tmpy;

      tmpx.val = -1 * psi.at(k).at(t).val * 2/(sqrt(abnd*bbnd)) * j*MY_PI/bbnd;
      tmpx.valSin = i * MY_PI / abnd;
      tmpx.valCos = j * MY_PI / bbnd;
      tmpx.index = psi.at(k).at(t).index;

      tmpy.val = psi.at(k).at(t).val * 2/(sqrt(abnd*bbnd)) * i*MY_PI/abnd;
      tmpy.valCos = i * MY_PI / abnd;
      tmpy.valSin = j * MY_PI / bbnd;
      tmpy.index = psi.at(k).at(t).index;

      rhs.eqX.append(tmpx);
      rhs.eqY.append(tmpy);
    }
  }
}

void MainEngine::createEqRhs(const QString& fileName)
{
  QFile file(fileName);

  if (file.open(QIODevice::ReadOnly))
  {
    QTextStream stream(&file);

    QString tmp = stream.readLine();
    QStringList tmpLst = tmp.split(" ");

    abnd = tmpLst.at(0).toDouble();
    bbnd = tmpLst.at(1).toDouble();

    tmp = stream.readLine();
    tmpLst = tmp.split(" ");

    tbegin = tmpLst.at(0).toDouble();
    tend = tmpLst.at(1).toDouble();

    equationAmount = stream.readLine().toInt();
    particleAmount = stream.readLine().toInt();
    stepAmount = stream.readLine().toInt();
    L = stream.readLine().toDouble();                // Число Рэлея !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    tmp = stream.readLine();
    tmpLst = tmp.split(" ");

    QVector<double> tmpVec2;
    for (int i = 0; i < equationAmount; i++)
      tmpVec2.append(tmpLst[i].toDouble());

    data.diffs.append(tmpVec2);

    QVector<particle> tmpVec3;
    for (int i = 0; i < particleAmount; i++)
    {
      particle tmpPrt;

      tmp = stream.readLine();
      tmpLst = tmp.split(" ");
      tmpPrt.x = tmpLst.at(0).toDouble();
      tmpPrt.y = tmpLst.at(1).toDouble();

      tmpVec3.append(tmpPrt);
    }

    data.coords.append(tmpVec3);
    data.time.append(tbegin);

    // ---------- создание системы ---------------
    QVector<basisCell> basis(equationAmount);
    crtBas(basis);

    QVector<QVector<psiCell>> psi(equationAmount);
    crtEq1(basis, psi);

    rhs.diffs.resize(equationAmount);
    crtEq2(basis, psi);

// -----------------------------------------------------------------------------------------------------------------------------------
//    for (int i = 0; i < equationAmount; i++)
//    {
//      for (int j = 0; j < psi.at(i).size();j++)
//        qDebug() << psi.at(i).at(j).val <<"| u" << psi.at(i).at(j).index+1;
//      qDebug() << "---------------------------------";
//    }

//  for (int i = 0; i < rhs.diffs.size(); i++)
//  {
//    qDebug() << "   EQUATION" << i;
//    for (int j = 0; j < rhs.diffs.at(i).size(); j++)
//    {
//      qDebug() << j <<"| "<< rhs.diffs.at(i).at(j).val << (rhs.diffs.at(i).at(j).hasL ? "  L  " : "") << rhs.diffs.at(i).at(j).indices;
//    }
//     qDebug() << "===================================";
//  }

//    for (int ik = 1; ik <= sqrt(equationAmount); ik++)
//      for (int i = 1; i <= sqrt(equationAmount); i++)
//        for (int ii = 1; ii <= sqrt(equationAmount); ii++)
//        {
//          qDebug() << i << ii << ik<< "| "<< intSinCosSin(i,ii,ik,abnd) << " " << intSinCosSin(ii,i,ik,abnd);
//        }

//  int xpr1 = 45;
//  int xpr2 = 76;
//  qDebug() << sqrt(xpr1*xpr2);
// -----------------------------------------------------------------------------------------------------------------------------------

    crtCoords(basis, psi);

    file.close();
  }
}

