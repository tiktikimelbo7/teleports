#include "qtdmessagechatsetttl.h"
#include "utils/i18n.h"

QTdMessageChatSetTTL::QTdMessageChatSetTTL(QObject *parent)
    : QTdMessageContent(parent)
{
    setType(MESSAGE_CHAT_SET_TTL);
}

QString QTdMessageChatSetTTL::ttl() const
{
    switch (m_ttl) {
        case 0:
            return gettext("Off");
        case 60:
            return gettext("1 minute");
        case 3600:
            return gettext("1 hour");
        case 86400:
            return gettext("1 day");
        case 604800:
            return gettext("1 week");
        default:
            return QString(ngettext("%1 second", "%1 seconds", m_ttl)).arg(m_ttl);
        }
}

void QTdMessageChatSetTTL::unmarshalJson(const QJsonObject &json)
{
    if (json.contains("ttl")) {
        m_ttl = json["ttl"].toInt();
        m_typeText = gettext("Self-destruct timer was set");
        emit contentChanged();
    }
}
