#include "qtdmessagechatsetttl.h"
#include "utils/i18n.h"

QTdMessageChatSetTTL::QTdMessageChatSetTTL(QObject *parent)
    : QTdMessageContent(parent)
{
    setType(MESSAGE_CHAT_SET_TTL);
}

QString QTdMessageChatSetTTL::ttl() const
{
    return m_ttl;
}

void QTdMessageChatSetTTL::unmarshalJson(const QJsonObject &json)
{
    if (json.contains("ttl")) {
        m_ttl = json["ttl"].toString();
        m_typeText = gettext("message TTL has been changed");
        emit contentChanged();
    }
}
