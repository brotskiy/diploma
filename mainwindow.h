#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QFileDialog>
#include <QThread>
#include <QMetaType>

#include <QString>

#include <QPointF>
#include "drawing_struct.h"
#include "widget.h"
#include <QPair>

#include "mainengine.h"
#include "mainengineshell.h"

#include "dialog.h"

#define MY_ZERO 1e-9

typedef QPair<int, int> Pair;

Q_DECLARE_METATYPE(drawing_struct)
Q_DECLARE_METATYPE(particle)
Q_DECLARE_METATYPE(QVector<particle>)
Q_DECLARE_METATYPE(QVector<QVector<particle>>)
Q_DECLARE_METATYPE(Pair)

class MainWindow : public QMainWindow
{
  Q_OBJECT

  private:
    MainEngineShell* engnShell;
    QThread* thread;

    bool openedFirstTime = true;

    QString thetasFileName;

    QMenuBar* menuBar;
    QToolBar* toolBar;
    QAction* txtEdit;
    QComboBox* comboBox;

    void createConnections();                 // ++
    void createMenuBar(MainWindow* parent);   // ++

  public:
    explicit MainWindow(QWidget *parent = nullptr);

  private slots:
    void slotOpen();                           // ++
    void fillComboBox(int prtAm);
    void setTxtEdit(const Pair& stepData);
    void activateToolBox();
    void unblockOpen();

    void dialogReadThetasFromFile();
    void readThetasFromFile(const QString& axs);

    void processPeriodicThetas();

    void calculateNusselts();

  signals:
    void wantToOpenFile(const QString& fileName);    // ++
    void partAmount(int prtAm);
    void readyForComputations();

};

#endif // MAINWINDOW_H
