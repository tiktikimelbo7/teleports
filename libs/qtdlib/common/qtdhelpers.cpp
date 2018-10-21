#include "qtdhelpers.h"

QString QTdHelpers::formatDate(const QDateTime &dt)
{
    const QDateTime now = QDateTime::currentDateTimeUtc().toLocalTime();
    const QDateTime localdt = dt.toLocalTime();
    if (now.daysTo(localdt) == 0) {
        return localdt.toString("hh:mm");
    }
    return localdt.toString("ddd hh:mm");
}
