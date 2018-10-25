#ifndef QTDVIEWMESSAGESREQUEST_H
#define QTDVIEWMESSAGESREQUEST_H

#include <QObject>
#include "common/qtdrequest.h"

class QTdViewMessagesRequest : public QTdRequest
{
    Q_OBJECT
    qint64 m_chatId;
    QList<qint32> m_messageIds;

public:
    explicit QTdViewMessagesRequest(QObject *parent = nullptr);
    void setMessageIds(const QList<qint32> &messageIds);
    void setChatId(const qint64 &id);
    QJsonObject marshalJson() Q_DECL_FINAL;
};

#endif // QTDVIEWMESSAGESREQUEST_H
