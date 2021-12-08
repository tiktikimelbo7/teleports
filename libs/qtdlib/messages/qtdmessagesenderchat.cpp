#include "qtdmessagesenderchat.h"
#include "client/qtdclient.h"
#include "common/qtdhelpers.h"
#include "chat/qtdchatlistmodel.h"

QTdMessageSenderChat::QTdMessageSenderChat(QObject *parent)
    : QTdMessageSender(parent)
    , m_chatId(0)
{
    setType(MESSAGE_SENDER_CHAT);
}

QString QTdMessageSenderChat::qmlChatId() const
{
    return m_chatId.toQmlValue();
}

qint64 QTdMessageSenderChat::chatId() const
{
    return m_chatId.value();
}

void QTdMessageSenderChat::unmarshalJson(const QJsonObject &json)
{
    m_chatId = json["chat_id"].toVariant().toLongLong();
    emit chatIdChanged();
    updateChat(m_chatId.value());
    QTdMessageSender::unmarshalJson(json);
}

void QTdMessageSenderChat::updateChat(const qint64 &chatId)
{
    if (chatId != m_chatId.value()) {
        return;
    }
    if (m_chat) {
        m_chat = nullptr;
    }

    auto *chats = QTdChatListModel::instance();
    m_chat = chats->chatById(chatId);
    if (m_chat) {
        emit chatChanged();
        emit dataChanged();
        if (m_waitingForChat) {
            disconnect(QTdChatListModel::instance(), &QTdChatListModel::chatCreated, this, &QTdMessageSenderChat::updateChat);
            m_waitingForChat = false;
        }
        return;
    }
    connect(QTdChatListModel::instance(), &QTdChatListModel::chatCreated, this, &QTdMessageSenderChat::updateChat);
    QTdClient::instance()->send(QJsonObject {
            { "@type", "getChat" },
            { "chat_id", chatId } });
    m_waitingForChat = true;
}

QString QTdMessageSenderChat::id() const
{
    return m_chatId.toQmlValue();
}

QString QTdMessageSenderChat::displayName() const
{
    if (!m_chat) {
        return QString();
    }

    return m_chat->title();
}

QString QTdMessageSenderChat::fullName() const
{
    return displayName();
}

QTdPhoto *QTdMessageSenderChat::photo() const
{
    if (!m_chat) {
        return nullptr;
    }

    return m_chat->chatPhoto();
}

QString QTdMessageSenderChat::avatarColor() const
{
    return QTdHelpers::avatarColor(m_chatId.value());
}

QString QTdMessageSenderChat::initials() const
{
    if (!m_chat) {
        return QString();
    }

    return m_chat->initials();
}
