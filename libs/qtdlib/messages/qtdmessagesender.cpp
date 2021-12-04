#include "messages/qtdmessagesender.h"
#include "client/qtdclient.h"
#include "common/qtdhelpers.h"
#include "user/qtdusers.h"

QTdMessageSender::QTdMessageSender(QObject *parent)
    : QTdObject(parent)
{
}

QTdMessageSender *QTdMessageSender::create(const QJsonObject &json, QObject *parent)
{
    const QString type = json["@type"].toString();

    if (type == "messageSenderUser") {
        return new QTdMessageSenderUser(parent);
    } else if (type == "messageSenderChat") {
        return new QTdMessageSenderChat(parent);
    }

    return new QTdMessageSenderUnimplemented(parent);
}

QTdMessageSenderUnimplemented::QTdMessageSenderUnimplemented(QObject *parent)
    : QTdMessageSender(parent)
{
}

QString QTdMessageSenderUnimplemented::id() const
{
    return QString("unimplemented");
}

QString QTdMessageSenderUnimplemented::displayName() const
{
    return QString("unimplemented");
}

QString QTdMessageSenderUnimplemented::fullName() const
{
    return QString("unimplemented");
}

QTdPhoto *QTdMessageSenderUnimplemented::photo() const
{
    return nullptr;
}

QString QTdMessageSenderUnimplemented::avatarColor() const
{
    return QString("#ffffff");
}

QString QTdMessageSenderUnimplemented::initials() const
{
    return "N/A";
}

QTdMessageSenderUser::QTdMessageSenderUser(QObject *parent)
    : QTdMessageSender(parent)
    , m_userId(0)
    , m_user(nullptr)
    , m_waitingForUser(false)
{
    setType(MESSAGE_SENDER_USER);
}

QString QTdMessageSenderUser::qmlUserId() const
{
    return m_userId.toQmlValue();
}

qint64 QTdMessageSenderUser::userId() const
{
    return m_userId.value();
}

QTdUser *QTdMessageSenderUser::user() const
{
    return m_user;
}

void QTdMessageSenderUser::unmarshalJson(const QJsonObject &json)
{
    m_userId = json["user_id"].toVariant().toLongLong();
    emit userIdChanged();
    updateUser(m_userId.value());
    QTdMessageSender::unmarshalJson(json);
}

void QTdMessageSenderUser::updateUser(const qint64 &userId)
{
    if (userId != m_userId.value()) {
        return;
    }
    if (m_user) {
        m_user = nullptr;
    }

    auto *users = QTdUsers::instance()->model();
    m_user = users->getByUid(QString::number(userId));
    if (m_user) {
        emit userChanged();
        emit dataChanged();
        if (m_waitingForUser) {
            disconnect(QTdUsers::instance(), &QTdUsers::userCreated, this, &QTdMessageSenderUser::updateUser);
            m_waitingForUser = false;
        }
        return;
    }
    connect(QTdUsers::instance(), &QTdUsers::userCreated, this, &QTdMessageSenderUser::updateUser);
    QTdClient::instance()->send(QJsonObject {
            { "@type", "getUser" },
            { "user_id", userId } });
    m_waitingForUser = true;
}

QString QTdMessageSenderUser::id() const
{
    return qmlUserId();
}

QString QTdMessageSenderUser::displayName() const
{
    if (!m_user) {
        return QString();
    }

    QString name = m_user->firstName();

    if (name.isEmpty()) {
        name = m_user->username();
    }

    return name;
}

QString QTdMessageSenderUser::fullName() const
{
    if (!m_user) {
        return QString();
    }

    return m_user->fullName();
}

QTdPhoto *QTdMessageSenderUser::photo() const
{
    if (!m_user) {
        return nullptr;
    }

    return m_user->profilePhoto();
}

QString QTdMessageSenderUser::avatarColor() const
{
    return QTdHelpers::avatarColor(m_userId.value());
}

QString QTdMessageSenderUser::initials() const
{
    if (!m_user) {
        return QString("N/A");
    }

    return m_user->initials();
}

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
    QTdMessageSender::unmarshalJson(json);
}

QString QTdMessageSenderChat::id() const
{
    return m_chatId.toQmlValue();
}

QString QTdMessageSenderChat::displayName() const
{
    // TODO: resolve chat name
    return m_chatId.toQmlValue();
}

QString QTdMessageSenderChat::fullName() const
{
    return displayName();
}

QTdPhoto *QTdMessageSenderChat::photo() const
{
    return nullptr;
}

QString QTdMessageSenderChat::avatarColor() const
{
    return QTdHelpers::avatarColor(m_chatId.value());
}

QString QTdMessageSenderChat::initials() const
{
    return QString("N/A");
}