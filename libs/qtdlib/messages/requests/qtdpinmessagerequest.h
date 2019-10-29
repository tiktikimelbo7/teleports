#ifndef QTDPINMESSAGEREQUEST_H
#define QTDPINMESSAGEREQUEST_H

#include <QObject>
#include "common/qtdrequest.h"

class QTdPinMessageRequest : public QTdOkRequest
{
    Q_OBJECT
    qint64 m_messageId;
    qint64 m_chatId;
public:
    explicit QTdPinMessageRequest(QObject *parent = nullptr);
    void setMessageId(const qint64 &messageId);
    void setChatId(const qint64 &chatId);
    QJsonObject marshalJson() Q_DECL_FINAL;
};

#endif // QTDPINMESSAGEREQUEST_H
