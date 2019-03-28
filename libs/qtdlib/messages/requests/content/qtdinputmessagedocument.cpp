#include "qtdinputmessagedocument.h"

QTdInputMessageDocument::QTdInputMessageDocument(QObject *parent) : QTdInputMessageContent(parent),
    m_document(QString()), m_caption(QString()), m_captionEntities(QJsonArray())
{
}
void QTdInputMessageDocument::setDocument(const QString &url)
{

    m_document = url;
}
void QTdInputMessageDocument::setCaption(const QString &caption)
{
    m_caption = caption;
}
void QTdInputMessageDocument::setCaptionEntities(const QJsonArray &entities)
{
    m_captionEntities = entities;
}
QJsonObject QTdInputMessageDocument::marshalJson()
{
    qDebug()<<"inputMessageDocument";
    return QJsonObject{
      {"@type", "inputMessageDocument"},
      {"document", QJsonObject{
        {"@type", "inputFileLocal"},
        {"path", m_document}
      }},
      {"caption", QJsonObject{
        {"@type", "formattedText"},
        {"text", m_caption},
        {"entities", m_captionEntities}
      }},
    };
}
