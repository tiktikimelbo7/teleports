#include "qtdgetchatmemberrequest.h"
#include "client/qtdclient.h"

QTdGetChatMemberRequest::QTdGetChatMemberRequest(QObject *parent)
    : QTdRequest(parent)
    , m_chatId(0)
    , m_userId(0)
{
}

void QTdGetChatMemberRequest::setChatId(const QString &chatId)
{
    setChatId(chatId.toInt());
}

void QTdGetChatMemberRequest::setChatId(const qint64 &chatId)
{
    m_chatId = chatId;
}

void QTdGetChatMemberRequest::setUserId(const QString &userId)
{
    setUserId(userId.toInt());
}

void QTdGetChatMemberRequest::setUserId(const qint32 &userId)
{
    m_userId = userId;
}

QJsonObject QTdGetChatMemberRequest::marshalJson()
{
    return QJsonObject {
        { "@type", "getChatMember" },
        { "chat_id", m_chatId.value() },
        { "user_id", m_userId.value() }
    };
}

QFuture<QTdResponse> QTdGetChatMemberRequest::sendAsync()
{
    return QTdClient::instance()->sendAsync(this, &QTdClient::chatMember);
}
