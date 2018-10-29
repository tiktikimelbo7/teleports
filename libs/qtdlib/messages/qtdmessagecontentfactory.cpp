#include "qtdmessagecontentfactory.h"
#include <QDebug> // TEMP

QTdMessageContent *QTdMessageContentFactory::create(const QJsonObject &json, QObject *parent)
{
    const QString type = json["@type"].toString();

    // qDebug() << "Message delegate type: " << type << json;

    if (type == "messageText") {
        return new QTdMessageText(parent);
    } else if (type == "messageSticker") {
        return new QTdMessageSticker(parent);
    } else if (type == "messageContactRegistered"){
        return new QTdMessageAction(parent);
    }
    return new QTdMessageContent(parent);
}
