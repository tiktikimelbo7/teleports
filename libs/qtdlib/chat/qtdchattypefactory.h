#ifndef QTDCHATTYPEFACTORY_H
#define QTDCHATTYPEFACTORY_H
#include "qtdchattype.h"
#include "qtdchat.h"

struct QTdChatFactory
{
    static QTdChatType *createType(const QJsonObject &json, QObject *parent);
    static QTdChat *createChat(const QJsonObject &json, QObject *parent = Q_NULLPTR);
};

#endif // QTDCHATTYPEFACTORY_H
