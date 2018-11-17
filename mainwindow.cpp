#include "mainwindow.h"

void MainWindow::createMenuBar(MainWindow* parent) // OK
{
  menuBar = new QMenuBar(parent);
  QMenu* menuFile = new QMenu("File", menuBar);

  menuBar->addAction(menuFile->menuAction());

  menuFile->addAction("Open");   // 0
  menuFile->addSeparator();      // 1
  menuFile->addAction("Exit");   // 2

  toolBar = new QToolBar(parent);
  comboBox = new QComboBox(toolBar);
  txtEdit = new QAction(toolBar);


  toolBar->setMovable(false);
  toolBar->addWidget(comboBox);
  toolBar->addAction(txtEdit);

  comboBox->addItem("MASTER");
  comboBox->setEditable(false);
  comboBox->setEnabled(false);

  txtEdit->setEnabled(false);

  setMenuBar(menuBar);
  addToolBar(Qt::TopToolBarArea, toolBar);
}

void MainWindow::createConnections()
{
  QAction* button = menuBar->actions().at(0)->menu()->actions().at(0);   // Open
  connect(button, SIGNAL(triggered(bool)), this, SLOT(slotOpen()));

  button = menuBar->actions().at(0)->menu()->actions().at(2);
  connect(button, SIGNAL(triggered(bool)), qApp, SLOT(quit()));

  connect(comboBox, SIGNAL(currentTextChanged(const QString&)), this->centralWidget(), SLOT(setCurrentImg(const QString&)));
}

void MainWindow::slotOpen()
{
  QString name = QFileDialog::getOpenFileName(this, "Explorer", "", "Input files(*.opn *.crte)");

  if (name != "")
  {
    thread = new QThread;              // инициализировали поток, который отвечает за реальную работу
    engnShell = new MainEngineShell;   // инициализировали начинку
    engnShell->moveToThread(thread);   // поместили начинку в рабочий поток

//    connect(thread, SIGNAL(started()), engnShell, SLOT(beginWork()));
    connect(this, SIGNAL(wantToOpenFile(const QString&)), engnShell, SLOT(openInitialFile(const QString&)));
    connect(engnShell, SIGNAL(toBorders(drawing_struct)), this->centralWidget(), SLOT(brdr(drawing_struct)));
    connect(engnShell, SIGNAL(toCurve(const QVector<QVector<particle>>&)), this->centralWidget(), SLOT(crv(const QVector<QVector<particle>>&)));
    connect(engnShell, SIGNAL(toDots(const QVector<particle>&)), this->centralWidget(), SLOT(prtcls(const QVector<particle>&)));

    connect(engnShell, SIGNAL(partAmount(int)), this, SLOT(fillComboBox(int)));
    connect(this, SIGNAL(partAmount(int)), this->centralWidget(), SLOT(setParticleAmont(int)));
    connect(this->centralWidget(), SIGNAL(readyForComputations()), this, SLOT(activateToolBox()));
    connect(this, SIGNAL(readyForComputations()), engnShell, SLOT(computeAll()));

    connect(engnShell, SIGNAL(currentStep(int)), this, SLOT(setTxtEdit(int)));

    thread->start();
    emit wantToOpenFile(name);
  }
}

void MainWindow::fillComboBox(int prtAm)
{
  for (int prt = 1; prt <= prtAm; prt++)
    comboBox->addItem(QString::number(prt));

  emit partAmount(prtAm);

}

void MainWindow::activateToolBox()
{
  comboBox->setEnabled(true);
  txtEdit->setText("computations initialized...");

  emit readyForComputations();
}

void MainWindow::setTxtEdit(int step)
{
  txtEdit->setText("step " + QString::number(step));
}

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent)
{
  qRegisterMetaType<drawing_struct>();
  qRegisterMetaType<particle>();
  qRegisterMetaType<QVector<QVector<particle>>>();
  qRegisterMetaType<QVector<particle>>();

  resize(1024, 768);

  createMenuBar(this);

  Widget* scene = new Widget(this);
  setCentralWidget(scene);

  createConnections();
}
