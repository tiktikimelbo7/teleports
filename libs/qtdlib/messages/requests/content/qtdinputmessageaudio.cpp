#include "qtdinputmessageaudio.h"

QTdInputMessageAudio::QTdInputMessageAudio(QObject *parent)
    : QTdInputMessageContent(parent)
    , m_audio(QString())
    , m_caption(QString())
    , m_captionEntities(QJsonArray())
{
}
void QTdInputMessageAudio::setAttachmentPath(const QString &url)
{

    m_audio = url;
}
void QTdInputMessageAudio::setCaption(const QString &caption)
{
    m_caption = caption;
}
void QTdInputMessageAudio::setCaptionEntities(const QJsonArray &entities)
{
    m_captionEntities = entities;
}
QJsonObject QTdInputMessageAudio::marshalJson()
{
    return QJsonObject{
        { "@type", "inputMessageAudio" },
        { "audio", QJsonObject{
                           { "@type", "inputFileLocal" },
                           { "path", m_audio } } },
        { "caption", QJsonObject{ { "@type", "formattedText" }, { "text", m_caption }, { "entities", m_captionEntities } } },
    };
}
