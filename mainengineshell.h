#ifndef MAINENGINESHELL_H
#define MAINENGINESHELL_H

#include <QObject>
#include <QFileInfo>
#include <QMetaType>

#include <QString>
#include <QPointF>
#include "drawing_struct.h"

#include <QPair>
typedef QPair<int, int> Pair;

#include "mainengine.h"
#include "particle.h"

class MainEngineShell: public QObject
{
  Q_OBJECT

  private:
    MainEngine* engn = nullptr;

  public:
    MainEngineShell(QObject* parent = nullptr);     // ++

    ~MainEngineShell();                             // ++

  void drawBorders();                               //  ++
  void rk4();                                       //  ++
  void writeDataToFiles();                    //  ++
  void calcNusselts(const QVector<QVector<double> >& thetas, const double a, const double b, const int thetaAmount,
                    QVector<double>& nuv, QVector<double>& nuh);                                                // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  public slots:
    void openInitialFile(const QString& fileName);   // ++
    void computeAll();

  signals:
    void toBorders(const drawing_struct& a);                   // ++
    void toCurve(const QVector<QVector<particle>>& a);  // ++
    void toDots(const QVector<particle>& a);             // ++
    void partAmount(int prtAm);
    void currentStep(const Pair& stepData);
    void unblockOpen();

};


#endif // MAINENGINESHELL_H
