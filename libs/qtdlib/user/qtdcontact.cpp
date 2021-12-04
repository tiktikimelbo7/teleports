#include "qtdcontact.h"
#include <QVariant>

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

void QTdContact::set_first_name(QString value)
{
    m_first_name = value;
}

QString QTdContact::last_name() const
{
  return m_last_name;
}

void QTdContact::set_last_name(QString value)
{
    m_last_name = value;
}

QString QTdContact::phone_number() const
{
    return m_phone_number;
}

void QTdContact::set_phone_number(QString value)
{
    m_phone_number = value;
}

QString QTdContact::vcard() const
{
    return m_vcard;
}

void QTdContact::set_vcard(QString value)
{
    m_vcard = value;
}

qint64 QTdContact::user_id() const
{
    return m_user_id;
}

void QTdContact::set_user_id(qint64 value)
{
    m_user_id = value;
}

void QTdContact::unmarshalJson(const QJsonObject &json)
{
    m_first_name = json["first_name"].toString();
    m_last_name = json["last_name"].toString();
    m_phone_number = json["phone_number"].toString();
    m_vcard = json["vcard"].toString();
    m_user_id = json["user_id"].toVariant().toLongLong();
    ;
    emit dataChanged();
}

QJsonObject QTdContact::marshalJson() {
    return QJsonObject{
        { "@type", "contact" },
        { "first_name", m_first_name },
        { "last_name", m_last_name },
        { "phone_number", m_phone_number },
        { "vcard", m_vcard },
        { "user_id", m_user_id }
    };
}
