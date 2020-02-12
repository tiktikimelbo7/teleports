#include "qtdmessagelocation.h"
#include "utils/i18n.h"

QTdMessageLocation::QTdMessageLocation(QObject *parent)
    : QTdMessageContent(parent)
    , m_location(new QTdLocation)
{
    setType(MESSAGE_LOCATION);
}

QTdLocation *QTdMessageLocation::location() const
{
    return m_location.data();
}

void QTdMessageLocation::unmarshalJson(const QJsonObject &json)
{
    m_location->unmarshalJson(json["location"].toObject());
    m_typeText = gettext("Location");
}
