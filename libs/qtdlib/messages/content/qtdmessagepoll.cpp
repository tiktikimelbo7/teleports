#include "qtdmessagepoll.h"
#include "utils/i18n.h"

QTdMessagePoll::QTdMessagePoll(QObject *parent)
    : QTdMessageContent(parent)
{
    setType(MESSAGE_POLL);
}

void QTdMessagePoll::unmarshalJson(const QJsonObject &json)
{
    m_typeText = gettext("Poll");
}
