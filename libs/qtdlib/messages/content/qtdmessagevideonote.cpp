#include "qtdmessagevideonote.h"

QTdMessageVideoNote::QTdMessageVideoNote(QObject *parent)
    : QTdMessageContent(parent)
    , m_videoNote(new QTdVideoNote)
    , m_isViewed(new bool)
    , m_isSecret(new bool)
{
    setType(MESSAGE_VIDEO_NOTE);
}

QTdVideoNote *QTdMessageVideoNote::videoNote() const
{
    return m_videoNote.data();
}
bool QTdMessageVideoNote::isViewed() const
{
    return m_isViewed;
}
bool QTdMessageVideoNote::isSecret() const
{
    return m_isSecret;
}
void QTdMessageVideoNote::unmarshalJson(const QJsonObject &json)
{
    m_videoNote->unmarshalJson(json["video_note"].toObject());
    m_isViewed = json["is_viewed"].toBool();
    m_isSecret = json["is_secret"].toBool();
}
