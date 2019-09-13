#include "qtdinputmessagephoto.h"

QTdInputMessagePhoto::QTdInputMessagePhoto(QObject *parent)
    : QTdInputMessageContent(parent)
    , m_photo(QString())
    , m_caption(QString())
    , m_captionEntities(QJsonArray())
{
}
void QTdInputMessagePhoto::setAttachmentPath(const QString &url)
{

    m_photo = url;
}
void QTdInputMessagePhoto::setCaption(const QString &caption)
{
    m_caption = caption;
}
void QTdInputMessagePhoto::setCaptionEntities(const QJsonArray &entities)
{
    m_captionEntities = entities;
}
QJsonObject QTdInputMessagePhoto::marshalJson()
{
    qDebug() << "inputMessagePhoto";
    return QJsonObject{
        { "@type", "inputMessagePhoto" },
        { "photo", QJsonObject{
                           { "@type", "inputFileLocal" },
                           { "path", m_photo } } },
        { "caption", QJsonObject{ { "@type", "formattedText" }, { "text", m_caption }, { "entities", m_captionEntities } } },
    };
}
