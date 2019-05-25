#include "mainengineshell.h"

#include <QtWidgets>
#include <QFile>

MainEngineShell::MainEngineShell(QObject* parent) : QObject(parent)
{
}

MainEngineShell::~MainEngineShell()
{
    if (engn != nullptr)  // зачем лишний раз удалять, если и так нету
      delete engn;
}

void MainEngineShell::openInitialFile(const QString& fileName)
{
  engn = new MainEngine(this); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  QFileInfo mfi(fileName);
  QString mfext = mfi.completeSuffix();

  if (mfext.contains("opn"))
    engn->readFromFile(fileName);

  if (mfext.contains("crte"))
    engn->createEqRhs(fileName);

  emit partAmount(engn->getPart());
}

void MainEngineShell::computeAll()
{
  drawBorders();
  emit toDots(engn->getParticlesAtStep(0));
  rk4();
  emit toCurve(engn->getParticles());
  writeDataToFiles();
}

void MainEngineShell::drawBorders()
{
  drawing_struct drw;

  drw.abnd = engn->getA();
  drw.bbnd = engn->getB();

  if (drw.abnd > drw.bbnd)
    drw.scale = (750.0 - 70) / drw.abnd;
  else
    drw.scale = (750.0 - 70) / drw.bbnd;

  emit toBorders(drw);
}

void MainEngineShell::rk4()
{
  const double tbegin = engn->getTBegin();
  const double tend = engn->getTEnd();
  const int stepAmount = engn->getStep();

  const double h = 1.0 * (tend - tbegin)/stepAmount;    // приращение времени !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  const int REPEAT_AMOUNT = 10; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  for (int repeat = 0; repeat < REPEAT_AMOUNT; repeat++)
  {
    for (int step = 1; step <= stepAmount; step++)      // идем по шагам
    {
      QTime t;
      t.start();

      engn->rk4_step(h, step);

      int ms = t.elapsed();

      if (step % 250 == 0)
      {
        emit toDots(engn->getParticlesAtStep(step));
        emit currentStep(Pair(step, ms));
      }
    }

   /* if (repeat == 0)
    {
      engn->writeThetasToFile(QIODevice::Truncate, 0);
      engn->writeCoordsToFile(QIODevice::Truncate, 0);
    }
    else
    {
      engn->writeThetasToFile(QIODevice::Append, 1);  // 1 чтобы строка не повторялась 2 раза подряд при записи в файл.
      engn->writeCoordsToFile(QIODevice::Append, 1);  // т.к. она уже записана последней на прошлом репите.
    } */

    if (repeat != REPEAT_AMOUNT - 1)
    {
      engn->cycleThetas();
      engn->cycleCoords();
    // надо еще сделать cycle time просто так !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
  }
}

void MainEngineShell::writeDataToFiles()
{
  engn->writeThetasToFile(QIODevice::Truncate, 0);
  engn->writeCoordsToFile(QIODevice::Truncate, 0);

  emit unblockOpen();
}

void MainEngineShell::calcNusselts(const QVector<QVector<double>>& thetas, const double a, const double b, const int thetaAmount,
                                   QVector<double>& nuv, QVector<double>& nuh)
{
  engn->calculateNusselts(thetas, a, b, thetaAmount, nuv, nuh);
}
