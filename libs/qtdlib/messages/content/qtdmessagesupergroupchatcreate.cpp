#include "qtdmessagesupergroupchatcreate.h"
#include <QDebug>
#include <QJsonArray>
#include <QScopedPointer>
#include "user/requests/qtdgetuserrequest.h"
#include "utils/await.h"

QTdMessageSuperGroupChatCreate::QTdMessageSuperGroupChatCreate(QObject *parent)
    : QTdMessageContent(parent)
{
    setType(MESSAGE_SUPER_GROUP_CHAT_CREATE);
}

QString QTdMessageSuperGroupChatCreate::title() const
{
    return m_title;
}

void QTdMessageSuperGroupChatCreate::unmarshalJson(const QJsonObject &json)
{
    m_title = json["title"].toString();
    emit titleChanged();
}
