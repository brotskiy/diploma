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

    double abnd, bbnd;                   // границы области [0; abnd] x [0; bbnd]

    double L;                            // число Рэлея для уравнений theta_i

    double tbegin, tend;                 // временной промежуток

    typeRhs rhs;                         // правые части системы
    typeData data;                       // значения всех искомых величин задачи

    void crtBas(QVector<basisCell>& basis);
    void crtEq1(const QVector<basisCell>& basis, QVector<QVector<psiCell>>& psi);
    void crtEq2(const QVector<basisCell>& basis, const QVector<QVector<psiCell>>& psi);

    void crtEq2Sub32(const QVector<QVector<psiCell>>& psi, const double xpr1, const int ik, const int jk,
                    const int i, const int j, const int ii, const int jj, const int n, const int k, const int count);

    void crtEq2Sub5(const QVector<QVector<psiCell>>& psi, const double xpr1, const int ik, const int jk,
                    const int i, const int j, const int k, const int count);

    void crtEq2Sub4(const double xpr1, const int ik, const int jk, const int count);

    void crtCoords(const QVector<basisCell>& basis, const QVector<QVector<psiCell>>& psi);

  public:
    MainEngine(QObject* parent = nullptr) : QObject(parent) {}

    void readConsts(QTextStream& stream);

    void readFromFile(const QString& fileName);
    void createEqRhs(const QString& fileName);
    void rk4_step(const double h, const int step);
    void writeCoordsToFile() const;
    void writeThetasToFile() const;

    const QVector<QVector<particle>>& getParticles() const { return data.coords; }
    const QVector<particle>& getParticlesAtStep(int step) const { return data.coords.at(step); }
    int getStep() const {return stepAmount;}
    int getPart() const {return particleAmount;}
    double getA() const { return abnd; }
    double getB() const { return bbnd; }
    double getTBegin() const { return tbegin; }
    double getTEnd() const { return tend; }
};

#endif // MAINENGINE_H
