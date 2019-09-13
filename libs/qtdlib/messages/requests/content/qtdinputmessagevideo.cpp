#include "qtdinputmessagevideo.h"

QTdInputMessageVideo::QTdInputMessageVideo(QObject *parent)
    : QTdInputMessageContent(parent)
    , m_photo(QString())
    , m_caption(QString())
    , m_captionEntities(QJsonArray())
{
}
void QTdInputMessageVideo::setAttachmentPath(const QString &url)
{

    m_photo = url;
}
void QTdInputMessageVideo::setCaption(const QString &caption)
{
    m_caption = caption;
}
void QTdInputMessageVideo::setCaptionEntities(const QJsonArray &entities)
{
    m_captionEntities = entities;
}
QJsonObject QTdInputMessageVideo::marshalJson()
{
    return QJsonObject{
        { "@type", "inputMessageVideo" },
        { "video", QJsonObject{
                           { "@type", "inputFileLocal" },
                           { "path", m_photo } } },
        { "caption", QJsonObject{ { "@type", "formattedText" }, { "text", m_caption }, { "entities", m_captionEntities } } },
    };
}
