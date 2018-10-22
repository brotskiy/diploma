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

  connect(this, SIGNAL(to_curve(const QVector<QPointF>&)), this->centralWidget(), SLOT(to_repaint(const QVector<QPointF>&)));
}

void MainWindow::slotOpen()
{
  QString name = QFileDialog::getOpenFileName(this, "Explorer", "", "Text files(*.txt)");

  if (name != "")
  {
    engn.readFromFile(name);
    engn.rk4();
  }

  for (int i = 0; i <= engn.getStep(); i++)                // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   qDebug() << "(" <<engn.getData().coords.at(i).at(0).x << engn.getData().coords.at(i).at(0).y <<") | " << engn.getData().time.at(i);   // !!!!!!!!!!!!!!!!!!!!!!!!!


 //------------------------------------------
  if (name != "")
  {
    QVector<QPointF> a;

    for (int step = 0; step < engn.getData().coords.size(); step++)
    {
      qreal x = engn.getData().coords.at(step).at(0).x;
      x = x * 100 + 100;
      qreal y = engn.getData().coords.at(step).at(0).y;
      y = y * 100 + 100;
      a.append(QPointF(x, y));
    }
    emit to_curve(a);
  }
 //------------------------------------------


/*  //------------------------------------------   для Лоренца
   if (name != "")
   {
     QVector<QPointF> a;

     for (int step = 0; step < engn.getData().coords.size(); step++)
     {
       qreal x = engn.getData().diffs.at(step).at(0);             // x
       x = x * 10 + 400;
       qreal y = engn.getData().diffs.at(step).at(2);             // z
       y = y * 10 + 100;
       a.append(QPointF(x, y));
     }
     emit to_curve(a);
   }
  //------------------------------------------ */

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
