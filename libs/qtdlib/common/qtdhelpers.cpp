#include "qtdhelpers.h"
#include "client/qtdclient.h"
#include <QJsonArray>

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

QString QTdHelpers::selfColor() { return "#65aadd"; }

QRegExp QTdHelpers::rxEntity;
QRegExp QTdHelpers::rxLinebreaks;

void QTdHelpers::getEntitiesFromMessage(const QString &messageText, QString &plainText, QJsonArray &entities)
{
    if (rxEntity.isEmpty())
    {
        rxEntity = QRegExp("\\*\\*.+\\*\\*|__.+__|```.+```|`.+`");
        rxEntity.setMinimal(true);
        rxLinebreaks = QRegExp("\\n|\\r");
    }
    int offsetCorrection = 0;
    int pos = 0;
    plainText = "";
    while ((pos = rxEntity.indexIn(messageText, pos)) != -1)
    {
        auto match = rxEntity.cap(0);
        QJsonObject entity;
        entity["@type"] = "textEntity";
        entity["offset"] = (pos - offsetCorrection);
        QJsonObject entityType;
        if (match.startsWith("*"))
        {
          entityType["@type"] = "textEntityTypeBold";
          entity["length"] = (rxEntity.matchedLength() - 4);
          offsetCorrection += 4;
        } else if(match.startsWith("_"))
        {
            entityType["@type"] = "textEntityTypeItalic";
            entity["length"] = (rxEntity.matchedLength() - 4);
            offsetCorrection += 4;
        } else if(match.startsWith("`") && !match.startsWith("``"))
        {
            entityType["@type"] = "textEntityTypeCode";
            entity["length"] = (rxEntity.matchedLength() - 2);
            offsetCorrection += 2;
        } else if(match.startsWith("```"))
        {
            entityType["@type"] = "textEntityTypePre";
            entity["length"] = (rxEntity.matchedLength() - 6);
            offsetCorrection += 6;
        }
        entity["type"] = entityType;
        entities << entity;
        pos += rxEntity.matchedLength();
    }

    plainText = messageText;
    plainText = plainText.replace("**", "").replace("__", "").replace("`", "");
}

QJsonArray QTdHelpers::formatPlainTextMessage(const QString &message, QString &plainText)
{
    //First call tdlib to markup all complex entities
    auto parseRequest = QJsonObject {
        {"@type", "getTextEntities"},
        {"text", message}
    };
    auto result = QTdClient::instance()->exec(parseRequest);
    result.waitForFinished();

    QJsonArray entities = result.result()["entities"].toArray();

    //Then do the text formatting
    QTdHelpers::getEntitiesFromMessage(message, plainText, entities);

    return entities;
}
