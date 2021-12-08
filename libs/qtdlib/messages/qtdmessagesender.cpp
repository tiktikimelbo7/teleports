#include "qtdmessagesender.h"
#include "qtdmessagesenderuser.h"
#include "qtdmessagesenderchat.h"

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
