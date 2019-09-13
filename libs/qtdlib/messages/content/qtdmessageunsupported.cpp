#include "qtdmessageunsupported.h"

QTdMessageUnsupported::QTdMessageUnsupported(QObject *parent)
    : QTdMessageContent(parent)
{
    setType(MESSAGE_UNSUPPORTED);
}

void QTdMessageUnsupported::unmarshalJson(const QJsonObject &json)
{
}
