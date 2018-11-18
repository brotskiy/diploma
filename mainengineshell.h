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
  void writeDataToFiles() const;                    //  ++
  void drawTrajectory();                            //  ++

  public slots:
//    void beginWork();                                // ++
    void openInitialFile(const QString& fileName);   // ++
    void computeAll();

  signals:
    void toBorders(drawing_struct a);                   // ++
    void toCurve(const QVector<QVector<particle>>& a);  // ++
    void toDots(const QVector<particle>& a);             // ++
    void partAmount(int prtAm);
    void currentStep(const Pair& stepData);

};


#endif // MAINENGINESHELL_H
