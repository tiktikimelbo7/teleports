#ifndef QTDINPUTMESSAGECONTENTFACTORY_H
#define QTDINPUTMESSAGECONTENTFACTORY_H

#include "qtdinputmessagecontent.h"

struct QTdInputMessageContentFactory
{
    static QTdInputMessageContent *create(const QJsonObject &json, QObject *parent = Q_NULLPTR);
};

#endif // QTDINPUTMESSAGECONTENTFACTORY_H
