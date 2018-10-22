#include "mainengine.h"

void MainEngine::readFromFile(const QString& fileName)
{
  QFile file(fileName);

  if (file.open(QIODevice::ReadOnly))
  {
    QTextStream stream(&file);

    QString tmp = stream.readLine();
    QStringList tmpLst = tmp.split(" ");

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
