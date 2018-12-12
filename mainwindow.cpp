#include "mainwindow.h"

void MainWindow::createMenuBar(MainWindow* parent) // OK
{
  menuBar = new QMenuBar(parent);
  QMenu* menuFile = new QMenu("File", menuBar);
  QMenu* menuTheta = new QMenu("Theta", menuBar);

  menuBar->addAction(menuFile->menuAction());    // 0
  menuBar->addAction(menuTheta->menuAction());   // 1

  menuFile->addAction("Open");   // 0
  menuFile->addSeparator();      // 1
  menuFile->addAction("Exit");   // 2

  menuTheta->addAction("Draw trajectory projection");   // 0
  menuTheta->addAction("Create periodic table");        // 1

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

  button = menuBar->actions().at(0)->menu()->actions().at(2);            // Exit
  connect(button, SIGNAL(triggered(bool)), qApp, SLOT(quit()));

  button = menuBar->actions().at(1)->menu()->actions().at(0);           // Draw trajectory projection
  connect(button, &QAction::triggered, this, &MainWindow::dialogReadThetasFromFile);

  button = menuBar->actions().at(1)->menu()->actions().at(1);           // Create periodic table
  connect(button, &QAction::triggered, this, &MainWindow::processPeriodicThetas);

  connect(comboBox, SIGNAL(currentTextChanged(const QString&)), this->centralWidget(), SLOT(setCurrentImg(const QString&)));

}

void MainWindow::processPeriodicThetas()
{
  QString name = QFileDialog::getOpenFileName(this, "Explorer", "", "Text files(*.txt)");

  if (name.contains("theta.txt"))
  {
    QFile file(name);

    if (file.open(QIODevice::ReadOnly))
    {
      QTextStream streamIn(&file);
      QString tmpStr = streamIn.readLine();
      const int eqNum = tmpStr.split(" ", QString::SkipEmptyParts).size();

      QVector<QVector<double>> theta;
      QVector<double> tmpVec(eqNum);

      for (int i = 0; i < eqNum; i++)
        tmpVec[i] = tmpStr.section(" ", i, i, QString::SectionSkipEmpty).toDouble();

      theta.append(tmpVec);

      int i = 0; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      while (!streamIn.atEnd())
      {
        tmpStr = streamIn.readLine();

        for (int i = 0; i < eqNum; i++)
          tmpVec[i] = tmpStr.section(" ", i, i, QString::SectionSkipEmpty).toDouble();

        i+=1; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        if (i % 20000 == 0)// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        qDebug() << tmpVec; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        theta.append(tmpVec);
      }

      QFileInfo processedFileInfo(file);
      QFile processedFile(processedFileInfo.path() + "/processed_periodic_theta.txt");

      if (processedFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
      {
        QVector<int> periods(eqNum, 0);
        QVector<bool> notYetPeriod(eqNum, true);
        QTextStream streamOut(&processedFile);

        for (int i = theta.size() - 2, generalCount = eqNum; (i >= 0) && (generalCount > 0); i--)
          for (int j = 0; j < eqNum; j++)
            if (notYetPeriod[j])
            {
              qDebug() << i << j << " | " <<periods[j]; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

              if (theta[i][j] - theta[i+1][j] < 1e-07)
              {
                notYetPeriod[j] = false;
                generalCount--;
              }
              else
                periods[j]++;
            }
        // добавить запись периодических тета в файл!!!!!!!!!!!!!!!!!!!!
        processedFile.close();


        for (int i = 0; i < eqNum; i++)   // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
          qDebug() << i << periods[i];    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      }

      file.close();

      QImage img(1900, 1000, QImage::Format_ARGB32);
      img.fill(Qt::white);

      QPainter pntr(&img);
      pntr.setRenderHint(QPainter::Antialiasing);
      pntr.translate(20, 500);
      pntr.scale(1,-1);
      pntr.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap));

          qDebug() << theta.size();

      const double scaleX = 1850.0 / theta.size();

      for (int i = 0; i < theta.size(); i++)
        for (int j = 0; j < eqNum; j++)
        {
            //if (i % 20000 == 0)
              //  qDebug() << theta[i][0];

          qreal xi = scaleX * i;
          qreal yi = 13 *  theta[i][j];

                // pntr.drawPoint(QPointF(0, 0));

          pntr.drawPoint(QPointF(xi, yi));
        }

      QLabel* wdgt = new QLabel(this, Qt::Window);
      wdgt->setAttribute(Qt::WA_DeleteOnClose);
      wdgt->setGeometry(20, 30, 1900, 1000);
      wdgt->setPixmap(QPixmap::fromImage(img));
      wdgt->show();
    }
  }
}

void MainWindow::readThetasFromFile(const QString& axs)
{
  if ((axs != "") && axs.contains(QRegExp("^\\d+\\s\\d+$")))
  {
    const int numX = axs.split(" ").at(0).toInt();
    const int numY = axs.split(" ").at(1).toInt();

    QFile file(thetasFileName);

    if (file.open(QIODevice::ReadOnly))
    {
      QTextStream stream(&file);
      QString tmp = stream.readLine();

      const int eqNum = tmp.split(" ", QString::SkipEmptyParts).size();

      if (0 <= numX && numX < eqNum && 0 <= numY && numY < eqNum)
      {
        QImage img(1920, 1080, QImage::Format_ARGB32);
        img.fill(Qt::white);

        QPainter pntr(&img);
        pntr.setRenderHint(QPainter::Antialiasing);
        pntr.translate(960, 720);
        pntr.scale(1,-1);
        pntr.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap));

        const int scaleX = 20; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        const int scaleY = 50; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        qreal xi = scaleX * tmp.section(" ", numX, numX, QString::SectionSkipEmpty).toDouble();
        qreal yi = scaleY * tmp.section(" ", numY, numY, QString::SectionSkipEmpty).toDouble();

        int i = 0;// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        while (!stream.atEnd())
        {
          i+=1;// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
          if (i % 20000 == 0)// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            qDebug() << i << xi/scaleX << yi/scaleY << " | " << xi << yi; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

          tmp = stream.readLine();

          qreal xii = scaleX * tmp.section(" ", numX, numX, QString::SectionSkipEmpty).toDouble();
          qreal yii = scaleY * tmp.section(" ", numY, numY, QString::SectionSkipEmpty).toDouble();

          //if (i > 120000) // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            pntr.drawLine(QPointF(xi,yi), QPointF(xii,yii));

          xi = xii;
          yi = yii;
        }

        QLabel* wdgt = new QLabel(this, Qt::Window);
        wdgt->setAttribute(Qt::WA_DeleteOnClose);
        wdgt->setGeometry(50,50, 1800, 900);
        wdgt->setPixmap(QPixmap::fromImage(img));
        wdgt->show();
      }
      file.close();
    }
  }
}

void MainWindow::dialogReadThetasFromFile()
{
  QString name = QFileDialog::getOpenFileName(this, "Explorer", "", "Text files(*.txt)");

  if (name.contains("theta.txt"))
  {
    thetasFileName = name;

    Dialog* dialog = new Dialog(this);
    connect(dialog, &Dialog::Axes, this, &MainWindow::readThetasFromFile);
    dialog->exec();
  }
}

void MainWindow::slotOpen()
{
  QString name = QFileDialog::getOpenFileName(this, "Explorer", "", "Input files(*.opn *.crte)");

  if (name != "")
  {
    menuBar->actions()[0]->menu()->actions()[0]->setEnabled(false);

    if (openedFirstTime)
    {
      openedFirstTime = false;

      thread = new QThread;              // инициализировали поток, который отвечает за реальную работу
      engnShell = new MainEngineShell;   // инициализировали начинку
      engnShell->moveToThread(thread);   // поместили начинку в рабочий поток

      connect(this, SIGNAL(wantToOpenFile(const QString&)), engnShell, SLOT(openInitialFile(const QString&)));
      connect(engnShell, SIGNAL(toBorders(const drawing_struct&)), this->centralWidget(), SLOT(brdr(const drawing_struct&)));
      connect(engnShell, SIGNAL(toCurve(const QVector<QVector<particle>>&)), this->centralWidget(), SLOT(crv(const QVector<QVector<particle>>&)));
      connect(engnShell, SIGNAL(toDots(const QVector<particle>&)), this->centralWidget(), SLOT(prtcls(const QVector<particle>&)));

      connect(engnShell, SIGNAL(partAmount(int)), this, SLOT(fillComboBox(int)));
      connect(this, SIGNAL(partAmount(int)), this->centralWidget(), SLOT(setParticleAmont(int)));
      connect(this->centralWidget(), SIGNAL(readyForComputations()), this, SLOT(activateToolBox()));
      connect(this, SIGNAL(readyForComputations()), engnShell, SLOT(computeAll()));
      connect(engnShell, &MainEngineShell::unblockOpen, this, &MainWindow::unblockOpen);

      connect(engnShell, SIGNAL(currentStep(const Pair&)), this, SLOT(setTxtEdit(const Pair&)));

      thread->start();
    }
    else
    {
      comboBox->clear();
      comboBox->addItem("MASTER");
      comboBox->setEnabled(false);
      txtEdit->setText(" ");
    }

    emit wantToOpenFile(name);
  }
}

void MainWindow::unblockOpen()
{
  menuBar->actions()[0]->menu()->actions()[0]->setEnabled(true);
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
  txtEdit->setText("computations started...");

  emit readyForComputations();
}

void MainWindow::setTxtEdit(const Pair& stepData)
{
  txtEdit->setText("step " + QString::number(stepData.first) + ", " + QString::number(stepData.second) + " ms");
}

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent)
{
  qRegisterMetaType<drawing_struct>();
  qRegisterMetaType<particle>();
  qRegisterMetaType<QVector<QVector<particle>>>();
  qRegisterMetaType<QVector<particle>>();
  qRegisterMetaType<Pair>();

  setFixedSize(750, 750);

  createMenuBar(this);

  Widget* scene = new Widget(this);
  setCentralWidget(scene);

  createConnections();
}
