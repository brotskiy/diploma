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

  connect(this, SIGNAL(to_curve(const drawing_struct&)), this->centralWidget(), SLOT(to_repaint(const drawing_struct&)));
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


//qDebug() << engn.getRhs().eqX.size(); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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
    //QVector<QVector<QPointF>> a;
    drawing_struct drw;

    for (int part = 0; part < engn.getPart(); part++)
    {
      drw.abnd = engn.getA();
      drw.bbnd = engn.getB();

      if (drw.abnd > drw.bbnd)
        drw.scale = (width()-10) / drw.abnd;
      else
        drw.scale = (height()-10) / drw.bbnd;

      QVector<QPointF> tmp;

      for (int step = 0; step < engn.getData().coords.size(); step++)
      {
        qreal x = engn.getData().coords.at(step).at(part).x;
        x = x * drw.scale;
        qreal y = engn.getData().coords.at(step).at(part).y;
        y = y * drw.scale;
        tmp.append(QPointF(x, y));
      }
      drw.points.append(tmp);
    }

    emit to_curve(drw);
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
