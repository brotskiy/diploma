#ifndef INPUT_H
#define INPUT_H

#include "cellcoord.h"
#include "celldiff.h"
#include "typerhs.h"
#include "typedata.h"

#include <QString>
#include <QStringList>
#include <QRegExp>

QVector<cellDiff> proccessOneDiff(const QString& eqStr);

QVector<cellCoord> proccessOneCoord(const QString& eqStr);

#endif // INPUT_H
