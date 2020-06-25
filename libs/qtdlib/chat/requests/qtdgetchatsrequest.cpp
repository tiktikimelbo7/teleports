#include "qtdgetchatsrequest.h"
#include "client/qtdclient.h"

QTdGetChatsRequest::QTdGetChatsRequest(QObject *parent)
    : QTdRequest(parent)
    , m_offset_chat_id(0)
    , m_offset_order(std::numeric_limits<std::int64_t>::max())
    , m_chatList(QStringLiteral("chatListMain"))
{
}

QJsonObject QTdGetChatsRequest::marshalJson()
{
    return QJsonObject {
        { "@type", "getChats" },
        { "chat_list", QJsonObject { { "@type", m_chatList } } },
        { "offset_order", QString::number(m_offset_order) },
        { "offset_chat_id", m_offset_chat_id },
        { "limit", 25 },
    };
}

void QTdGetChatsRequest::setOffsetOrder(const qint64 value)
{
    m_offset_order = value;
}

void QTdGetChatsRequest::setOffsetChatId(const qint64 value)
{
    m_offset_chat_id = value;
}

void QTdGetChatsRequest::setChatList(const QString chatList)
{
    m_chatList = chatList;
}

QFuture<QTdResponse> QTdGetChatsRequest::sendAsync()
{
    return QTdClient::instance()->sendAsync(this, &QTdClient::chats);
}
