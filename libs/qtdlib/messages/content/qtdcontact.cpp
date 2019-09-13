#include "qtdcontact.h"
#include "user/qtdusers.h"
#include "client/qtdclient.h"

QTdContact::QTdContact(QObject *parent)
    : QTdObject(parent)
    , m_first_name("")
    , m_last_name("")
    , m_phone_number("")
    , m_vcard("")
    , m_user_id(0)
{
    setType(CONTACT);
}

QString QTdContact::first_name() const
{
    return m_first_name;
}

QString QTdContact::last_name() const
{
  return m_last_name;
}

QString QTdContact::phone_number() const
{
    return m_phone_number;
}

QString QTdContact::vcard() const
{
    return m_vcard;
}

qint32 QTdContact::user_id() const
{
    return m_user_id.value();
}

void QTdContact::set_user_id(qint32 value)
{
    m_user_id = QTdInt32(value);
}

void QTdContact::set_first_name(QString value)
{
    m_first_name = value;
}

void QTdContact::set_last_name(QString value)
{
    m_last_name = value;
}

void QTdContact::set_phone_number(QString value)
{
    m_phone_number = value;
}

void QTdContact::set_vcard(QString value)
{
    m_vcard = value;
}

QTdUser *QTdContact::user() const
{
    return QTdUsers::instance()->model()->getByUid(qmlUserId());
}

QString QTdContact::qmlUserId() const
{
    return m_user_id.toQmlValue();
}

QJsonObject QTdContact::marshalJson() {
    return QJsonObject{
        { "@type", "contact" },
        { "phone_number", m_phone_number },
        { "first_name", m_first_name },
        { "last_name", m_last_name },
        { "vcard", m_vcard },
        { "user_id", m_user_id.toJsonValue() },
    };
}

void QTdContact::unmarshalJson(const QJsonObject &json)
{
    m_first_name = json["first_name"].toString();
    m_last_name = json["last_name"].toString();
    m_phone_number = json["phone_number"].toString();
    m_vcard = json["vcard"].toString();
    m_user_id = json["user_id"].toInt();
    emit dataChanged();
    auto *users = QTdUsers::instance()->model();
    if (users->getByUid(qmlUserId())) {
        emit userChanged();
        return;
    }
    connect(QTdUsers::instance(), &QTdUsers::userCreated, this, &QTdContact::isUserAvailable);
    QTdClient::instance()->send(QJsonObject{
            { "@type", "getUser" },
            { "user_id", m_user_id.value() } });
    m_waitingForUser = true;
}

void QTdContact::isUserAvailable(const qint32 &userId)
{
    if (userId != m_user_id.value()) {
        return;
    }
    if (m_waitingForUser) {
        disconnect(QTdUsers::instance(), &QTdUsers::userCreated, this, &QTdContact::isUserAvailable);
        m_waitingForUser = false;
    }
    emit userChanged();
}
