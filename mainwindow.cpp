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
}

void MainWindow::slotOpen()
{
  QString name = QFileDialog::getOpenFileName(this, "Explorer", "", "Input files(*.opn *.crte)");

  if (name != "")
  {
    thread = new QThread;              // инициализировали поток, который отвечает за реальную работу
    engnShell = new MainEngineShell;   // инициализировали начинку
    engnShell->moveToThread(thread);   // поместили начинку в рабочий поток

    // ТУТ ИДУТ КОННЕКТЫ ДЛЯ ПОТОКА И НАЧИНКИ
    connect(thread, SIGNAL(started()), engnShell, SLOT(beginWork()));
    connect(this, SIGNAL(wantToOpenFile(const QString&)), engnShell, SLOT(openInitialFile(const QString&)));
    connect(engnShell, SIGNAL(toBorders(drawing_struct)), this->centralWidget(), SLOT(brdr(drawing_struct)));
    connect(engnShell, SIGNAL(toCurve(const QVector<QVector<particle>>&)), this->centralWidget(), SLOT(crv(const QVector<QVector<particle>>&)));
    connect(engnShell, SIGNAL(toDots(const QVector<particle>&)), this->centralWidget(), SLOT(prtcls(const QVector<particle>&)));

    thread->start();
    emit wantToOpenFile(name);
  }
}


MainWindow::MainWindow(QWidget* parent): QMainWindow(parent)
{
  qRegisterMetaType<drawing_struct>();
  qRegisterMetaType<particle>();
  qRegisterMetaType<QVector<QVector<particle>>>();
  qRegisterMetaType<QVector<particle>>();

  resize(1024, 768);

  createMenuBar(this);
  setMenuBar(menuBar);

  Widget* scene = new Widget(this);
 // scene->resize(800,600);
  setCentralWidget(scene);

  createConnections();
}
