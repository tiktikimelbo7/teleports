#include "qtdmessagechatjoinbylink.h"
#include "utils/i18n.h"

QTdMessageChatJoinByLink::QTdMessageChatJoinByLink(QObject *parent)
    : QTdMessageContent(parent)
{
    setType(MESSAGE_CHAT_JOIN_BY_LINK);
    m_typeText = gettext("changed the chat title");
}
