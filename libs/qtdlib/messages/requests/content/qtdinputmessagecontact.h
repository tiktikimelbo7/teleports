#ifndef QTDINPUTMESSAGECONTACT_H
#define QTDINPUTMESSAGECONTACT_H

#include <QObject>
#include <QJsonArray>
#include <QString>
#include <QDebug>

#include "imessageattachmentcontent.h"
#include "imessagecaptioncontent.h"
#include "../qtdinputmessagecontent.h"
#include "../../../messages/content/qtdcontact.h"

class QTdInputMessageContact : public QTdInputMessageContent
{
    Q_OBJECT

public:
    explicit QTdInputMessageContact(QObject *parent = nullptr);
    QJsonObject marshalJson() Q_DECL_FINAL;
    void setContact(QTdContact *contact);

private:
    Q_DISABLE_COPY(QTdInputMessageContact)
    QTdContact* m_contact;
};

#endif // QTDINPUTMESSAGECONTACT_H
