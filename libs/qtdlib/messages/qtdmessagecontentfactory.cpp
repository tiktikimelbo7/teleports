#include "qtdmessagecontentfactory.h"
#include <QDebug> // TEMP

QTdMessageContent *QTdMessageContentFactory::create(const QJsonObject &json, QObject *parent)
{
    const QString type = json["@type"].toString();


    if (type == "messageText") {
        return new QTdMessageText(parent);
    } else if (type == "messageSticker") {
        return new QTdMessageSticker(parent);
    } else if (type == "messagePhoto") {
          return new QTdMessagePhoto(parent);
    } else if (type == "messageContactRegistered"){
        return new QTdMessageAction(parent);
    }
    else if (type =="messageChatJoinByLink"){
      return new QTdMessageHidden(parent);
    }
    return new QTdMessageContent(parent);
}
