#include "mainengine.h"

#include <QtWidgets>

void MainEngine::readConsts(QTextStream& stream)  // ++
{
  QString tmp = stream.readLine();
  QStringList tmpLst = tmp.split(" ");

  abnd = tmpLst[0].toDouble();
  bbnd = tmpLst[1].toDouble();

  tmp = stream.readLine();
  tmpLst = tmp.split(" ");

  tbegin = tmpLst[0].toDouble();
  tend = tmpLst[1].toDouble();

  equationAmount = stream.readLine().toInt();
  particleAmount = stream.readLine().toInt();
  stepAmount = stream.readLine().toInt();
  L = stream.readLine().toDouble();                // Число Рэлея
}

void MainEngine::resizeData(QTextStream& stream) // ++
{
    for (int i = 0 ; i < 4; i++)
      KThetasAtStep[i].resize(equationAmount);

    data.time.resize(1 + stepAmount);
    data.time[0] = tbegin;

    QString tmp = stream.readLine();
    QStringList tmpLst = tmp.split(" ");

    data.diffs.resize(1 + stepAmount);
    data.coords.resize(1 + stepAmount);

    for (int i = 0; i < 1 + stepAmount; i++)
    {
      data.diffs[i].resize(equationAmount);
      data.coords[i].resize(particleAmount);
    }

    for (int j = 0; j < equationAmount; j++)
        data.diffs[0][j] = tmpLst[j].toDouble();

    for (int j = 0; j < particleAmount; j++)
    {
      tmp = stream.readLine();
      tmpLst = tmp.split(" ");

      data.coords[0][j].x = tmpLst[0].toDouble();
      data.coords[0][j].y = tmpLst[1].toDouble();
    }
}

void MainEngine::readFromFile(const QString& fileName) // ++
{
  QFile file(fileName);

  if (file.open(QIODevice::ReadOnly))
  {
    QTextStream stream(&file);

    readConsts(stream);

    for (int i = 0; i <equationAmount; i++)
    {
      QVector<cellDiff> tmpVec;

      QString tmp = stream.readLine();
      tmpVec = proccessOneDiff(tmp);
      rhs.diffs.append(tmpVec);
    }

    QString tmp = stream.readLine();
    rhs.eqX = proccessOneCoord(tmp);

    tmp = stream.readLine();
    rhs.eqY = proccessOneCoord(tmp);

    resizeData(stream);

    file.close();
  }
}

void MainEngine::crtBas(QVector<basisCell>& basis) const    // ++
{
  int cntr = 0;

  for (int i = 1; i <= sqrt(equationAmount); i++)
    for (int j = 1; j <= sqrt(equationAmount); j++)
    {
      basis[cntr].i = i;
      basis[cntr].j = j;
      cntr++;
    }
}

void MainEngine::crtEq1(const QVector<basisCell>& basis, QVector<QVector<psiCell>>& psi)    // ++
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

      for (int t = 0; t < psi[k].size(); t++)           // смотрим eq1: перебираем все theta, из суммы которых состоит текущее psi_k
      {
        cellDiff tmp;                                   // создаем член результирующей системы: d(theta_1)/dt = A*theta_1*theta_2 + B*theta_3 + ...

        tmp.val = coeff * psi[k][t].val;                // его коэфф. - это коэфф. проекции умножить на соответствующий коефф. при theta в eq1
        tmp.indices.append(n);                          // добавляем в член номер текущего theta_n из формулы проекции
        tmp.indices.append(psi[k][t].index);            // добавляем номер theta из уравнения eq1, соответствующего текущему psi_k

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

    for (int t = 0; t < psi[k].size(); t++)
    {
      cellDiff tmp;

      tmp.hasL = true;
      tmp.val = coeff * psi[k][t].val;
      tmp.indices.append(psi[k][t].index);

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

void MainEngine::crtCoords(const QVector<basisCell>& basis, const QVector<QVector<psiCell>>& psi)  // ++
{
  rhs.eqX.clear();       // очистка, если в них хранится мусор с прошлого раза!!!!!!
  rhs.eqY.clear();

  for (int k = 0; k < equationAmount; k++)
  {
    const int i = basis[k].i;
    const int j = basis[k].j;

    for (int t = 0; t < psi[k].size(); t++)
    {
      cellCoord tmp;

      double val = -1 * psi[k][t].val * 2/(sqrt(abnd*bbnd)) * j*MY_PI/bbnd;

      if (fabs(val) > MY_ZERO)
      {
        tmp.val = val;
        tmp.valSin = i * MY_PI / abnd;
        tmp.valCos = j * MY_PI / bbnd;
        tmp.index = psi[k][t].index;

        rhs.eqX.append(tmp);
      }

      val = psi[k][t].val * 2/(sqrt(abnd*bbnd)) * i*MY_PI/abnd;

      if (fabs(val) > MY_ZERO)
      {
        tmp.val = val;
        tmp.valCos = i * MY_PI / abnd;
        tmp.valSin = j * MY_PI / bbnd;
        tmp.index = psi[k][t].index;

        rhs.eqY.append(tmp);
      }
    }
  }
}

void MainEngine::calculateNusselts(const QVector<QVector<double>>& thetas, const double a, const double b, const int thetaAmount,
                                            QVector<double>& nuv, QVector<double>& nuh)    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
  equationAmount = thetaAmount;

  QVector<basisCell> basis(thetaAmount);
  crtBas(basis);

  const int stepAmount = thetas.size();

  for (int step = 0; step < stepAmount; step++)
  {
    nuv[step] = 0;
    nuh[step] = 0;

    for (int k = 0; k < thetaAmount; k++)
    {
      const int i = basis[k].i;
      const int j = basis[k].j;

    //  qDebug() << i << " " << j;

      nuv[step] += thetas[step][k] * 2*j*a / (sqrt(a*b)*b*i) * (1 - cos(MY_PI * i)); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      nuh[step] += thetas[step][k] * 2*i*b / (sqrt(a*b)*a*j) * (1 - cos(MY_PI * j)) * cos(0.5 * MY_PI * i);  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      if ((k % 50 == 0) && (step % 200 == 0))
        qDebug() << "nuv = "<< nuv[step] << "  nuh = " << nuh[step];
    }
  }
}

void MainEngine::createEqRhs(const QString& fileName)   // ++
{
  QFile file(fileName);

  if (file.open(QIODevice::ReadOnly))
  {
    QTextStream stream(&file);

    readConsts(stream);
    resizeData(stream);

    // ---------- создание системы ---------------
    QVector<basisCell> basis(equationAmount);
    crtBas(basis);

    QVector<QVector<psiCell>> psi(equationAmount);
    crtEq1(basis, psi);

    rhs.diffs.resize(equationAmount);      // ресайз аннулирует то, что могло было быть записано в нем заранее
    crtEq2(basis, psi);

    crtCoords(basis, psi);

    file.close();
  }
}

void MainEngine::writeThetasToFile(QIODevice::OpenMode mode, const int strtNum) const
{
  QDir dir;
  dir.mkdir("output");

  QString name = "output/theta.txt";
  QFile outTheta(name);

  if (outTheta.open(QIODevice::WriteOnly | mode)) // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  {
    QTextStream stream(&outTheta);

    for (int i = strtNum; i < data.diffs.size(); i++) // strtNum вместо 0 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    {
      for (int j = 0; j < data.diffs[i].size(); j++)
        stream << data.diffs[i][j] << " ";

      stream << endl;
    }

    outTheta.close();
  }
}

void MainEngine::writeCoordsToFile(QIODevice::OpenMode mode, const int strtNum) const
{
  QDir dir;
  dir.mkdir("output/particles");

  for (int part = 0; part < particleAmount; part++)
  {
    QString name = "output/particles/prt" + QString::number(part) + ".txt";
    QFile output(name);

    if (output.open(QIODevice::WriteOnly | mode)) // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    {
      QTextStream stream(&output);

      for (int step = strtNum; step < data.coords.size(); step++)  // strtNum вместо 0 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      {
        double x = data.coords[step][part].x;
        double y = data.coords[step][part].y;

        stream <<"("<< x <<" "<< y <<")"<< endl;
      }

      output.close();
    }
  }
}

void MainEngine::cycleThetas() // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
  for (int j = 0; j < data.diffs[0].size(); j++)
    data.diffs[0][j] = data.diffs[data.diffs.size()-1][j];
}

void MainEngine::cycleCoords()  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
  for (int part = 0; part < particleAmount; part++)
  {
    data.coords[0][part].x = data.coords[data.coords.size()-1][part].x;
    data.coords[0][part].y = data.coords[data.coords.size()-1][part].y;
  }
}

void MainEngine::rk4_step(const double h, const int step)
{
  rk4step(rhs, data, tbegin, h, step, equationAmount, particleAmount, L, KThetasAtStep);
}

