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

QString QTdHelpers::avatarColor(unsigned int userId)
{
    QStringList colorPallete = {
        "#8179d7", // violet
        "#f2749a", // pink
        "#7ec455", // green
        "#f3c34a", // yellow
        "#5b9dd8", // blue
        "#62b8cd", // aqua
        "#ed8b4a", // orange
        "#d95848" // red
    };
    return colorPallete.at(userId % colorPallete.size());
}
