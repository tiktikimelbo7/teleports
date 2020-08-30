#include "qtdmessageexpiredvideo.h"
#include "utils/i18n.h"

QTdMessageExpiredVideo::QTdMessageExpiredVideo(QObject *parent)
    : QTdMessageContent(parent)
{
    setType(MESSAGE_EXPIRED_VIDEO);
}

void QTdMessageExpiredVideo::unmarshalJson(const QJsonObject &json)
{
    m_typeText = gettext("Expired video");
}
