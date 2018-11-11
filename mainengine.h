#ifndef MAINENGINE_H
#define MAINENGINE_H

#include <QObject>
#include <QFile>
#include <QTextStream>

#include "input.h"
#include "eq_rhs_creation.h"
#include "particle.h"

#include "runge-kutta.h"

class MainEngine: public QObject
{
  Q_OBJECT

  private:
    int equationAmount;                  // количество уравнений в системе
    int particleAmount;                  // количество частиц жидкости для расчёта траекторий
    int stepAmount;                      // кол-во шагов

    double L;                            // число Рэлея для уравнений theta_i

    double tbegin, tend;                 // временной промежуток

    typeRhs rhs;                         // правые части системы
    typeData data;                       // значения всех искомых величин задачи

  public:
    void readFromFile(const QString& fileName);
    void rk4();

    typeData getData() {return data;}
    typeRhs getRhs() {return rhs;}
    int getStep() {return stepAmount;}
    int getPart() {return particleAmount;}

};

#endif // MAINENGINE_H
