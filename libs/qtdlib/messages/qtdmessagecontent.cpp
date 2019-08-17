#include "qtdmessagecontent.h"
#include <libintl.h>
#include "utils/i18n.h"

QTdMessageContent::QTdMessageContent(QObject *parent) : QTdObject(parent)
{
    m_infoText = gettext("Message");
    m_infoImageUrl = QUrl();
}

QString QTdMessageContent::infoText() const
{
    return m_infoText;
}

QUrl QTdMessageContent::infoImageUrl() const
{
    return m_infoImageUrl;
}

QTdMessageAction::QTdMessageAction(QObject *parent) : QTdMessageContent(parent)
{
    setType(MESSAGE_ACTION);
}

QTdMessageHidden::QTdMessageHidden(QObject *parent) : QTdMessageContent(parent)
{
    setType(MESSAGE_HIDDEN);
}

