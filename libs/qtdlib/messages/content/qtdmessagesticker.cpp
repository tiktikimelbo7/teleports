#include "qtdmessagesticker.h"
#include "utils/i18n.h"

QTdMessageSticker::QTdMessageSticker(QObject *parent)
    : QTdMessageContent(parent)
    , m_sticker(new QTdSticker)
{
    setType(MESSAGE_STICKER);
}

QTdSticker *QTdMessageSticker::sticker() const
{
    return m_sticker.data();
}

void QTdMessageSticker::unmarshalJson(const QJsonObject &json)
{
    m_sticker->unmarshalJson(json["sticker"].toObject());
    m_infoImageUrl = QUrl("file://" + m_sticker->thumbnail()->photo()->local()->path());
    m_typeText = m_sticker->emoji() + " " + gettext("Sticker");
}
