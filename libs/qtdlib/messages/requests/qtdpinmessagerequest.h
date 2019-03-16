#ifndef QTDPINMESSAGEREQUEST_H
#define QTDPINMESSAGEREQUEST_H

#include <QObject>
#include "common/qtdrequest.h"

class QTdPinMessageRequest : public QTdOkRequest
{
    Q_OBJECT
    qint64 m_messageId;
    qint32 m_superGroupId;
public:
    explicit QTdPinMessageRequest(QObject *parent = nullptr);
    void setMessageId(const qint64 &messageId);
    void setSuperGroupId(const qint32 &superGroupId);
    QJsonObject marshalJson() Q_DECL_FINAL;
};

#endif // QTDPINMESSAGEREQUEST_H
