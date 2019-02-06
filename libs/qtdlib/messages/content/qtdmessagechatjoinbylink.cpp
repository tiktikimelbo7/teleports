#include "qtdmessagechatjoinbylink.h"

QTdMessageChatJoinByLink::QTdMessageChatJoinByLink(QObject *parent) : QTdMessageContent(parent)
{
    setType(MESSAGE_CHAT_JOIN_BY_LINK);
}
