#include "qtdmessagewebsiteconnected.h"
#include "utils/i18n.h"

QTdMessageWebsiteConnected::QTdMessageWebsiteConnected(QObject *parent)
    : QTdMessageContent(parent)
{
    setType(MESSAGE_WEBSITE_CONNECTED);
}

void QTdMessageWebsiteConnected::unmarshalJson(const QJsonObject &json)
{
    m_domain_name = json["domain_name"].toString();
    m_typeText = gettext("Connected Website");
    emit domainNameChanged();
}

QString QTdMessageWebsiteConnected::domainName() const
{
    return m_domain_name;
}
