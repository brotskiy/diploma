#include "mainengineshell.h"

#include <QtWidgets>

MainEngineShell::MainEngineShell(QObject* parent) : QObject(parent)
{
}

MainEngineShell::~MainEngineShell()
{
    if (engn != nullptr)  // зачем лишний раз удалять, если и так нету
      delete engn;
}

//void MainEngineShell::beginWork()
//{
//  engn = new MainEngine(this);
//}

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
 // computeAll();
}

void MainEngineShell::computeAll()
{
  drawBorders();
  rk4();
  writeDataToFiles();
  drawTrajectory();
}

void MainEngineShell::drawBorders()
{
  drawing_struct drw;

  drw.abnd = engn->getA();
  drw.bbnd = engn->getB();

  if (drw.abnd > drw.bbnd)
    drw.scale = (1024 - 15) / drw.abnd;
  else
    drw.scale = (768 - 70) / drw.bbnd;

  emit toBorders(drw);
}

void MainEngineShell::rk4()
{
  const double tbegin = engn->getTBegin();
  const double tend = engn->getTEnd();
  const int stepAmount = engn->getStep();

  const double h = (tend - tbegin)/stepAmount;    // приращение времени

  for (int step = 1; step <= stepAmount; step++)      // идем по шагам
  {
    engn->rk4_step(h, step);

    if (step % 100 == 0)
    {
      emit toDots(engn->getParticlesAtStep(step));
    //  qDebug() << step;  // посылаю всякие другие сигналы
      emit currentStep(step);
      // --------------------------------------------------------
    }
  }
}

void MainEngineShell::writeDataToFiles() const
{
  engn->writeThetasToFile();
  engn->writeCoordsToFile();
}

void MainEngineShell::drawTrajectory()
{
  emit toCurve(engn->getParticles());
}