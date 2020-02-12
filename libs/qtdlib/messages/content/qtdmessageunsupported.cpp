#include "qtdmessageunsupported.h"
#include "utils/i18n.h"

QTdMessageUnsupported::QTdMessageUnsupported(QObject *parent)
    : QTdMessageContent(parent)
{
    setType(MESSAGE_UNSUPPORTED);
}

void QTdMessageUnsupported::unmarshalJson(const QJsonObject &json)
{
    m_typeText = gettext("Unsupported message");
}
