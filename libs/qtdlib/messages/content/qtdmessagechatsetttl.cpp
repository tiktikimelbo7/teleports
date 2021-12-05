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
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            return QString(ngettext("%1 second", "%1 seconds", m_ttl)).arg(m_ttl);
        case 16:
            return QString(ngettext("%1 second", "%1 seconds", m_ttl)).arg(m_ttl);
        case 17:
            return gettext("1 minute");
        case 18:
            return gettext("1 hour");
        case 19:
            return gettext("1 day");
        case 20:
            return gettext("1 week");
        default:
            return gettext("Off");
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
