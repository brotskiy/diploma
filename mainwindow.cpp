#include "mainwindow.h"

void MainWindow::createMenuBar(MainWindow* parent) // OK
{
  menuBar = new QMenuBar(parent);
  QMenu* menuFile = new QMenu("File", menuBar);

  menuBar->addAction(menuFile->menuAction());

  menuFile->addAction("Open");   // 0
  menuFile->addSeparator();      // 1
  menuFile->addAction("Exit");   // 2
}

void MainWindow::createConnections()
{
  QAction* button = menuBar->actions().at(0)->menu()->actions().at(0);   // Open
  connect(button, SIGNAL(triggered(bool)), this, SLOT(slotOpen()));

  button = menuBar->actions().at(0)->menu()->actions().at(2);
  connect(button, SIGNAL(triggered(bool)), qApp, SLOT(quit()));

  connect(this, SIGNAL(toCurve(const QVector<QVector<particle>>&)), this->centralWidget(), SLOT(crv(const QVector<QVector<particle>>&)));
  connect(this, SIGNAL(toBorders(drawing_struct)), this->centralWidget(), SLOT(brdr(drawing_struct)));
}

void MainWindow::slotOpen()
{
  QString name = QFileDialog::getOpenFileName(this, "Explorer", "", "Input files(*.opn *.crte)");

  if (name != "")
  {
    QFileInfo mfi(name);
    QString mfext = mfi.completeSuffix();

    if (mfext.contains("opn"))
      engn.readFromFile(name);

    if (mfext.contains("crte"))
      engn.createEqRhs(name);

    drawBorders();

    rk4(); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // --------------------------------------------------------
    QString nameu = "theta.txt";
    QFile outTheta(nameu);

    if (outTheta.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
      QTextStream stream(&outTheta);

      for (int i = 0; i < engn.getData().diffs.size(); i++)
      {
        for (int j = 0; j < engn.getData().diffs.at(i).size(); j++)
          stream << engn.getData().diffs.at(i).at(j) << " ";

        stream << endl;
      }

      outTheta.close();
    }
    // --------------------------------------------------------

    //---------------------------------------------------------
    for (int part = 0; part < engn.getPart(); part++)
    {
      QString name = "output" + QString::number(part) + ".txt";
      QFile output(name);

      if (output.open(QIODevice::WriteOnly | QIODevice::Truncate))
      {
        QTextStream stream(&output);

        for (int step = 0; step < engn.getData().coords.size(); step++)
        {
          double x = engn.getData().coords.at(step).at(part).x;
          double y = engn.getData().coords.at(step).at(part).y;

          stream <<"("<< x <<" "<< y <<")"<< endl;
        }
        stream << endl;
        output.close();
      }
    }
    //------------------------------------------------------------

    drawTrajectory();
  }
}

void MainWindow::drawBorders()
{
  drawing_struct drw;

  drw.abnd = engn.getA();
  drw.bbnd = engn.getB();

  if (drw.abnd > drw.bbnd)
    drw.scale = (width()-15) / drw.abnd;
  else
    drw.scale = (height()-40) / drw.bbnd;

  emit toBorders(drw);
}

void MainWindow::drawTrajectory()
{
  emit toCurve(engn.getData().coords);
}

void MainWindow::rk4()
{
  const double tbegin = engn.getTBegin();
  const double tend = engn.getTEnd();
  const int stepAmount = engn.getStep();

  const double h = (tend - tbegin)/stepAmount;    // приращение времени

  for (int step = 1; step <= stepAmount; step++)      // идем по шагам
  {
    engn.rk4_step(h, step);

    qApp->processEvents(); // надо сделать нормально с отдельным потоком !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    if (step == 2000)
    {
      // посылаю всякие другие сигналы

    }
  }
}

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent)
{
  resize(1024, 768);

  createMenuBar(this);
  setMenuBar(menuBar);

  Widget* scene = new Widget(this);
  scene->resize(800,600);
  setCentralWidget(scene);

  createConnections();
}
