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
    QTdMessageContent::unmarshalJson(json);
    if (m_typeString == "messageAnimatedEmoji") {
        auto tempAnimatedEmoji = json["animated_emoji"].toObject();
        if (!tempAnimatedEmoji.isEmpty() && tempAnimatedEmoji["@type"] == "animatedEmoji") {
            m_sticker->unmarshalJson(tempAnimatedEmoji["sticker"].toObject());
        }
    } else {
        m_sticker->unmarshalJson(json["sticker"].toObject());
    }
    m_infoImageUrl = QUrl("file://" + m_sticker->thumbnail()->file()->local()->path());
    m_typeText = m_sticker->emoji() + " " + gettext("Sticker");
}
