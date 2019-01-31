#include "qtdmessagevideo.h"

QTdMessageVideo::QTdMessageVideo(QObject *parent) : QTdMessageContent(parent),
    m_video(new QTdVideo), m_caption(new QTdFormattedText)
{
    setType(MESSAGE_VIDEO);
}

QTdVideo *QTdMessageVideo::video() const
{
    return m_video.data();
}
QTdFormattedText *QTdMessageVideo::caption() const
{
  return m_caption.data();
}
void QTdMessageVideo::unmarshalJson(const QJsonObject &json)
{
    m_video->unmarshalJson(json["video"].toObject());
    m_caption->unmarshalJson(json["caption"].toObject());
}
