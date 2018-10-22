#include "input.h"

QVector<cellDiff> proccessOneDiff(const QString& eqStr)      // обрабатываем одно из уравнение (d theta)/dt = ...
{
  QVector<cellDiff> equation;                                // моё представление уравнения

  QString tmp = eqStr;
  tmp.replace(QRegExp("\\s\\-\\s"), " + -");                 // заменяем все минусы на плюсы с последующим минус число

  QStringList terms = tmp.split(QRegExp("\\s\\+\\s"));       // поделили строку на список слагаемых

  for (int i = 0; i < terms.size(); i++)                     // перебираем все слагаемые
  {
    cellDiff tmpCell;

    QStringList factors = terms[i].split(QRegExp("\\s\\*\\s"));      // поделили слагаемое на сомножители

    for (int j = 0; j < factors.size(); j++)                // перебираем все сомножители
      if (factors[j].contains("u"))                         // если это theta_i
      {
        factors[j].remove("u");                             // избавляемся от буквы отвечающей за theta
        tmpCell.indices.append(factors[j].toInt() - 1);     // запоминаем индексы theta, входящих в слагаемое. u1 у меня u0 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      }
      else                                                  // если это числовой коеффициент
        if (factors[j].contains(QRegExp("\\d+")))
          tmpCell.val = factors[j].toDouble();              // то просто запоминаем его
        else
          if (factors[j].contains("L"))                     // если же это число Рэлея !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            tmpCell.hasL = true;                            // ставим флаг, что слагаемое имеет L !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    equation.append(tmpCell);                               // добавляем текущее слагаемое в представление уравнения
  }

  return equation;
}

QVector<cellCoord> proccessOneCoord(const QString& eqStr)
{
  QVector<cellCoord> equation;

  QString tmp = eqStr;
  tmp.replace(QRegExp("\\s\\-\\s"), " + -");

  QStringList terms = tmp.split(QRegExp("\\s\\+\\s"));

  for (int i = 0; i < terms.size(); i++)
  {
    cellCoord tmpCell;

    QStringList factors = terms[i].split(QRegExp("\\s\\*\\s"));

    for (int j = 0; j < factors.size(); j++)
      if (factors[j].contains("u"))
      {
        factors[j].remove("u");
        tmpCell.index = factors[j].toInt() - 1;  // u1 у меня u0 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      }
      else
        if (factors[j].contains("sin"))
        {
          factors[j].remove("sin(");
          tmpCell.valSin = factors[j].toDouble();
        }
        else
          if (factors[j].contains("cos"))
          {
            factors[j].remove("cos(");
            tmpCell.valCos = factors[j].toDouble();
          }
          else
            if (factors[j].contains(QRegExp("^\\-?\\d+\\.\\d+e\\-?\\d+$")))
              tmpCell.val = factors[j].toDouble();

    equation.append(tmpCell);
  }

  return equation;
}
