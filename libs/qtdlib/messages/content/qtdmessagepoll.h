#ifndef QTDMESSAGEPOLL_H
#define QTDMESSAGEPOLL_H

#include <QObject>
#include <QScopedPointer>
#include "messages/qtdmessagecontent.h"

class QTdMessagePoll : public QTdMessageContent
{
    Q_OBJECT
public:
    explicit QTdMessagePoll(QObject *parent = nullptr);

    void unmarshalJson(const QJsonObject &json);

private:
    Q_DISABLE_COPY(QTdMessagePoll)
};

#endif // QTDMESSAGEPOLL_H
