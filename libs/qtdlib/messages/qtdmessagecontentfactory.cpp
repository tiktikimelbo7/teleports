#include "qtdmessagecontentfactory.h"
#include <QDebug> // TEMP

QTdMessageContent *QTdMessageContentFactory::create(const QJsonObject &json, QObject *parent)
{
    const QString type = json["@type"].toString();
    qDebug() << "Message delegate type: " << type;

    if (type == "messageText") {
        return new QTdMessageText(parent);
    }
    else if (type == "messageContactRegistered"){
      return new QTdMessageAction(parent);
    }
    return new QTdMessageContent(parent);
}
