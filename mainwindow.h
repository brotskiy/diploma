#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QFileDialog>
#include <QThread>

#include <QString>

#include <QPointF>
#include "drawing_struct.h"
#include "widget.h"

#include "mainengine.h"
#include "mainengineshell.h"

Q_DECLARE_METATYPE(drawing_struct)
Q_DECLARE_METATYPE(particle)
Q_DECLARE_METATYPE(QVector<QVector<particle>>)
Q_DECLARE_METATYPE(QVector<particle>)
class MainWindow : public QMainWindow
{
  Q_OBJECT

  private:
    MainEngineShell* engnShell;
    QThread* thread;

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
    void setTxtEdit(int step);
    void activateToolBox();

  signals:
    void wantToOpenFile(const QString& fileName);    // ++
    void partAmount(int prtAm);
    void readyForComputations();

};

#endif // MAINWINDOW_H
