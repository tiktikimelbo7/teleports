#include "qtdmessagechatchangetitle.h"

QTdMessageChatChangeTitle::QTdMessageChatChangeTitle(QObject *parent)
    : QTdMessageContent(parent)
{
    setType(MESSAGE_CHAT_CHANGE_TITLE);
}

QString QTdMessageChatChangeTitle::title() const
{
    return m_title;
}

void QTdMessageChatChangeTitle::unmarshalJson(const QJsonObject &json)
{
    m_title = json["title"].toString();
    QTdMessageContent::unmarshalJson(json);
    emit contentChanged();
}
