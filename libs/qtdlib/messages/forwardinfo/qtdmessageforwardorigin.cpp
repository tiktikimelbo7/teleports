#include "common/qabstracttdobject.h"
#include "qtdmessageforwardorigin.h"
#include "common/qtdhelpers.h"
#include "chat/qtdchats.h"
#include "user/qtdusers.h"

QTdMessageForwardOrigin::QTdMessageForwardOrigin(QObject *parent)
    : QTdObject(parent)
{
}

QTdMessageForwardOriginChat::QTdMessageForwardOriginChat(QObject *parent)
    : QTdMessageForwardOrigin(parent)
    , m_senderChatId(0)
{
    setType(MESSAGE_FORWARD_ORIGIN_CHAT);
}

QString QTdMessageForwardOriginChat::originSummary() const
{
    auto *chat = QTdChats::instance()->chatById(m_senderChatId.value());
    if (chat) {
        return chat->title();
    }
    return "";
}

QString QTdMessageForwardOriginChat::qmlChatId() const
{
    return m_senderChatId.toQmlValue();
}
qint64 QTdMessageForwardOriginChat::chatId() const
{
    return m_senderChatId.value();
}

QString QTdMessageForwardOriginChat::authorSignature() const
{
    return m_authorSignature;
}

void QTdMessageForwardOriginChat::unmarshalJson(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }
    m_senderChatId = json["sender_chat_id"].toDouble();
    m_authorSignature = json["author_signature"].toString();
    emit forwardOriginChanged();
}

QTdMessageForwardOriginChannel::QTdMessageForwardOriginChannel(QObject *parent)
    : QTdMessageForwardOrigin(parent)
    , m_chatId(0)
    , m_messageId(0)
{
    setType(MESSAGE_FORWARD_ORIGIN_CHANNEL);
}

QString QTdMessageForwardOriginChannel::originSummary() const
{
    auto *chat = QTdChats::instance()->chatById(m_chatId.value());
    if (chat) {
        return chat->title();
    }
    return "";
}

QString QTdMessageForwardOriginChannel::qmlChatId() const
{
    return m_chatId.toQmlValue();
}
qint64 QTdMessageForwardOriginChannel::chatId() const
{
    return m_chatId.value();
}

QString QTdMessageForwardOriginChannel::qmlMessageId() const
{
    return m_messageId.toQmlValue();
}
qint64 QTdMessageForwardOriginChannel::messageId() const
{
    return m_messageId.value();
}

QString QTdMessageForwardOriginChannel::authorSignature() const
{
    return m_authorSignature;
}

void QTdMessageForwardOriginChannel::unmarshalJson(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }
    m_chatId = json["chat_id"].toDouble();
    m_messageId = json["message_id"].toInt();
    m_authorSignature = json["author_signature"].toString();
    emit forwardOriginChanged();
}

QTdMessageForwardOriginHiddenUser::QTdMessageForwardOriginHiddenUser(QObject *parent)
    : QTdMessageForwardOrigin(parent)
{
    setType(MESSAGE_FORWARD_ORIGIN_HIDDEN_USER);
}

QString QTdMessageForwardOriginHiddenUser::originSummary() const
{
    return senderName();
}

QString QTdMessageForwardOriginHiddenUser::senderName() const
{
    return m_senderName;
}

void QTdMessageForwardOriginHiddenUser::unmarshalJson(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }
    m_senderName = json["sender_name"].toString();
    emit senderNameChanged();
}

QTdMessageForwardOriginUser::QTdMessageForwardOriginUser(QObject *parent)
    : QTdMessageForwardOrigin(parent)
    , m_senderUserId(0)
{
    setType(MESSAGE_FORWARD_ORIGIN_USER);
}

QString QTdMessageForwardOriginUser::originSummary() const
{
    auto *user = QTdUsers::instance()->model()->getByUid(QString::number(m_senderUserId.value()));
    if (user) {
        return user->firstName() + " " + user->lastName();
    }
    return "";
}

QString QTdMessageForwardOriginUser::qmlSenderUserId() const
{
    return m_senderUserId.toQmlValue();
}

qint32 QTdMessageForwardOriginUser::senderUserId() const
{
    return m_senderUserId.value();
}

void QTdMessageForwardOriginUser::unmarshalJson(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }
    m_senderUserId = json["sender_user_id"].toInt();
    emit senderUserIdChanged();
}
