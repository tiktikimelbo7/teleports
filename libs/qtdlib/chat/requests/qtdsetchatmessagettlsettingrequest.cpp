#include "qtdsetchatmessagettlsettingrequest.h"

QTdSetChatMessageTtlSettingRequest::QTdSetChatMessageTtlSettingRequest(QObject *parent)
    : QTdOkRequest(parent)
    , m_chatId(0)
{
}

void QTdSetChatMessageTtlSettingRequest::setChatId(const qint64 &id)
{
    m_chatId = id;
}

void QTdSetChatMessageTtlSettingRequest::setTTL(const qint32 &ttl)
{
    m_ttl = ttl;
}

QJsonObject QTdSetChatMessageTtlSettingRequest::marshalJson()
{
    return QJsonObject{
        { "@type", "setChatMessageTtlSetting" },
        { "chat_id", m_chatId },
        { "ttl", m_ttl }
    };
}
