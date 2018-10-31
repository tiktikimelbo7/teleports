#include "qtdhelpers.h"

QString QTdHelpers::formatDate(const QDateTime &dt)
{
    const QDateTime now = QDateTime::currentDateTimeUtc().toLocalTime();
    const QDateTime localdt = dt.toLocalTime();
    auto daysDiff = now.daysTo(localdt);
    if ( daysDiff == 0) {
        return localdt.toString("hh:mm");
    } else if (daysDiff > -7) {
        return localdt.toString("ddd");
    } else
        return localdt.toString("dd.MM.yy");
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

QString QTdHelpers::selfColor()
{
    return "#65aadd";
}
