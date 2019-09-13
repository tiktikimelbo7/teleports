#include "qtdinputmessagetext.h"

QTdInputMessageText::QTdInputMessageText(QObject *parent)
    : QTdInputMessageContent(parent)
    , m_text(QString())
    , m_entities(QJsonArray())
    , m_disable_web_page_preview(new bool)
    , m_clear_draft(new bool)
{
}

void QTdInputMessageText::setText(const QString &text)
{
    m_text = text;
}
void QTdInputMessageText::setEntities(const QJsonArray &entities)
{
    m_entities = entities;
}
QJsonObject QTdInputMessageText::marshalJson()
{
    return QJsonObject{
        { "@type", "inputMessageText" },
        { "text", QJsonObject{
                          { "@type", "formattedText" },
                          { "text", m_text },
                          { "entities", m_entities } } },
        { "disable_web_page_preview", false },
        { "clear_draft", true }
    };
}
