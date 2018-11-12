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

  connect(this, SIGNAL(to_curve(const QVector<QVector<QPointF>>&)), this->centralWidget(), SLOT(to_repaint(const QVector<QVector<QPointF>>&)));
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


qDebug() << engn.getRhs().eqX.size(); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    engn.rk4();

    //---------------------------------------------------------------------
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
    //---------------------------------------------------------------------

    //------------------------------------------
    QVector<QVector<QPointF>> a;

    for (int part = 0; part < engn.getPart(); part++)
    {
      QVector<QPointF> tmp;

      for (int step = 0; step < engn.getData().coords.size(); step++)
      {
        qreal x = engn.getData().coords.at(step).at(part).x;
        x = x * 100 + 100;
        qreal y = engn.getData().coords.at(step).at(part).y;
        y = y * 100 + 100;
        tmp.append(QPointF(x, y));
      }
      a.append(tmp);
    }

    emit to_curve(a);
    //------------------------------------------
  }
}

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent) // OK
{
  resize(1024, 768);

  createMenuBar(this);
  setMenuBar(menuBar);

  //----------------------------------------------
  Widget* scene = new Widget(parent);
  scene->resize(800,600);
  setCentralWidget(scene);
  //----------------------------------------------

  createConnections();
}
