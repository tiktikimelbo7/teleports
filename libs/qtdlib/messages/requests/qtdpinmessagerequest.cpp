#include "qtdpinmessagerequest.h"

QTdPinMessageRequest::QTdPinMessageRequest(QObject *parent) : QTdOkRequest(parent),
    m_messageId(0), m_superGroupId(0)
{
}

void QTdPinMessageRequest::setMessageId(const qint64 &id)
{
    m_messageId = id;
}

void QTdPinMessageRequest::setSuperGroupId(const qint32 &id)
{
    m_superGroupId = id;
}

QJsonObject QTdPinMessageRequest::marshalJson()
{
    return QJsonObject{
        {"@type", "pinSupergroupMessage"},
        {"supergroup_id", m_superGroupId},
        {"message_id", m_messageId},
        {"disable_notification", false},
    };
}
