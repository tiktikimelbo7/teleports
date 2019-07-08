#include "qtdmessagecontact.h"

QTdMessageContact::QTdMessageContact(QObject *parent)
    : QTdMessageContent(parent)
    , m_contact(new QTdContact)
{
    setType(MESSAGE_CONTACT);
}

QTdContact *QTdMessageContact::contact() const
{
    return m_contact.data();
}

void QTdMessageContact::unmarshalJson(const QJsonObject &json) {
    m_contact->unmarshalJson(json["contact"].toObject());
}
