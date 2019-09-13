#include "qtdinputmessagecontact.h"

QTdInputMessageContact::QTdInputMessageContact(QObject *parent)
    : QTdInputMessageContent(parent)
    , m_contact(nullptr)
{
}
void QTdInputMessageContact::setContact(QTdContact *contact)
{

    m_contact = contact;
}

QJsonObject QTdInputMessageContact::marshalJson()
{
    return QJsonObject{
        { "@type", "inputMessageContact" },
        { "contact", m_contact->marshalJson()},
    };
}
