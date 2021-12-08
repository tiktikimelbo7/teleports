#include "qtdmessagesenderuser.h"
#include "client/qtdclient.h"
#include "common/qtdhelpers.h"
#include "user/qtdusers.h"

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
