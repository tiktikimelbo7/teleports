#include "common/qabstracttdobject.h"
#include <QJsonValue>
#include <QDateTime>
#include "qtdmessageforwardinfo.h"
#include "common/qtdhelpers.h"

QTdMessageForwardInfo::QTdMessageForwardInfo(QObject *parent) : QTdObject(parent)
{
}


QTdMessageForwardedFromUser::QTdMessageForwardedFromUser(QObject *parent) : QTdMessageForwardInfo(parent),
    m_senderUserId(0), m_date(0), m_forwardedFromChatId(0), m_forwardedFromMessageId(0)
{
    setType(MESSAGE_FORWARDED_FROM_USER);
}

QString QTdMessageForwardedFromUser::qmlSenderUserId() const
{
    return m_senderUserId.toQmlValue();
}
qint32 QTdMessageForwardedFromUser::senderUserId() const
{
    return m_senderUserId.value();
}

QDateTime QTdMessageForwardedFromUser::qmlDate() const
{
    return QDateTime::fromTime_t(m_date);
}
qint32 QTdMessageForwardedFromUser::date() const
{
    return m_date;
}

QString QTdMessageForwardedFromUser::qmlForwardedFromChatId() const
{
    return m_forwardedFromChatId.toQmlValue();
}
qint64 QTdMessageForwardedFromUser::forwardedFromChatId() const
{
    return m_forwardedFromChatId.value();
}

QString QTdMessageForwardedFromUser::qmlForwardedFromMessageId() const
{
    return m_forwardedFromMessageId.toQmlValue();
}
qint64 QTdMessageForwardedFromUser::forwardedFromMessageId() const
{
    return m_forwardedFromMessageId.value();
}

void QTdMessageForwardedFromUser::unmarshalJson(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }
    m_senderUserId = json["sender_user_id"].toInt();
    m_date = json["date"].toInt();
    m_forwardedFromChatId = json["forwarded_from_chat_id"].toInt();
    m_forwardedFromMessageId = json["forwarded_from_message_id"].toInt();
    emit forwardInfoChanged();
}



QTdMessageForwardedPost::QTdMessageForwardedPost(QObject *parent) : QTdMessageForwardInfo(parent),
    m_chatId(0), m_date(0), m_messageId(0),
    m_forwardedFromChatId(0), m_forwardedFromMessageId(0)
{
    setType(MESSAGE_FORWARDED_POST);
}


QString QTdMessageForwardedPost::qmlChatId() const
{
    return m_chatId.toQmlValue();
}
qint64 QTdMessageForwardedPost::chatId() const
{
    return m_chatId.value();
}

QString QTdMessageForwardedPost::authorSignature() const
{
    return m_authorSignature;
}

QDateTime QTdMessageForwardedPost::qmlDate() const
{
    return QDateTime::fromTime_t(m_date);
}
qint32 QTdMessageForwardedPost::date() const
{
    return m_date;
}

QString QTdMessageForwardedPost::qmlMessageId() const
{
    return m_messageId.toQmlValue();
}
qint64 QTdMessageForwardedPost::messageId() const
{
    return m_messageId.value();
}

QString QTdMessageForwardedPost::qmlForwardedFromChatId() const
{
    return m_forwardedFromChatId.toQmlValue();
}
qint64 QTdMessageForwardedPost::forwardedFromChatId() const
{
    return m_forwardedFromChatId.value();
}

QString QTdMessageForwardedPost::qmlForwardedFromMessageId() const
{
    return m_forwardedFromMessageId.toQmlValue();
}
qint64 QTdMessageForwardedPost::forwardedFromMessageId() const
{
    return m_forwardedFromMessageId.value();
}

void QTdMessageForwardedPost::unmarshalJson(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }
    m_chatId = json["chat_id"];
    m_authorSignature = json["author_signature"].toString();
    m_date = json["date"].toInt();
    m_messageId = json["message_id"].toInt();
    m_forwardedFromChatId = json["forwarded_from_chat_id"].toInt();
    m_forwardedFromMessageId = json["forwarded_from_message_id"].toInt();
    emit forwardInfoChanged();
}
