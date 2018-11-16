#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QFileDialog>
#include <QFileInfo>

#include <QFile>
#include <QTextStream>
#include <QString>

#include <QPointF>
#include "drawing_struct.h"
#include "widget.h"

#include "mainengine.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

  private:
    MainEngine engn;

    QMenuBar* menuBar;

    void createConnections();
    void createMenuBar(MainWindow* parent);

    void drawTrajectory();
    void drawBorders();
    void drawParticles(const int step);

    void rk4();

  public:
    explicit MainWindow(QWidget *parent = nullptr);

  private slots:
    void slotOpen();

  signals:
    void toBorders(drawing_struct a);
    void toCurve(const QVector<QVector<particle>>& a);
};

#endif // MAINWINDOW_H
