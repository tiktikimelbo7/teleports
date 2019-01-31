#include "qtdmessageaudio.h"

QTdMessageAudio::QTdMessageAudio(QObject *parent) : QTdMessageContent(parent),
    m_audio(new QTdAudio), m_caption(new QTdFormattedText)
{
    setType(MESSAGE_AUDIO);
}

QTdAudio *QTdMessageAudio::audio() const
{
    return m_audio.data();
}
QTdFormattedText *QTdMessageAudio::caption() const
{
  return m_caption.data();
}
void QTdMessageAudio::unmarshalJson(const QJsonObject &json)
{
    m_audio->unmarshalJson(json["audio"].toObject());
    m_caption->unmarshalJson(json["caption"].toObject());
}
