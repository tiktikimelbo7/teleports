#include "qtdpinmessagerequest.h"

QTdPinMessageRequest::QTdPinMessageRequest(QObject *parent) : QTdOkRequest(parent),
    m_messageId(0), m_chatId(0)
{
}

void QTdPinMessageRequest::setMessageId(const qint64 &id)
{
    m_messageId = id;
}

void QTdPinMessageRequest::setChatId(const qint64 &id)
{
    m_chatId = id;
}

QJsonObject QTdPinMessageRequest::marshalJson()
{
    return QJsonObject{
        {"@type", "pinChatMessage"},
        {"chat_id", m_chatId},
        {"message_id", m_messageId},
        {"disable_notification", false},
    };
}
