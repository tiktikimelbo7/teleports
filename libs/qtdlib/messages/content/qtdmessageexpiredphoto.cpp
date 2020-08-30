#include "qtdmessageexpiredphoto.h"
#include "utils/i18n.h"

QTdMessageExpiredPhoto::QTdMessageExpiredPhoto(QObject *parent)
    : QTdMessageContent(parent)
{
    setType(MESSAGE_EXPIRED_PHOTO);
}

void QTdMessageExpiredPhoto::unmarshalJson(const QJsonObject &json)
{
    m_typeText = gettext("Expired photo");
}
