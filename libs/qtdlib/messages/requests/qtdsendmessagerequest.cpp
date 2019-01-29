#include "qtdsendmessagerequest.h"
#include <QJsonArray>
#include "client/qtdclient.h"

QTdSendMessageRequest::QTdSendMessageRequest(QObject *parent) : QTdRequest(parent),
  m_chatId(0), m_entities(QJsonArray()), m_replyMessageId(0)
{
}

void QTdSendMessageRequest::setText(const QString &text)
{
    m_text = text;
}

void QTdSendMessageRequest::setChatId(const qint64 &id)
{
  m_chatId = id;
}

void QTdSendMessageRequest::setEntities(const QJsonArray &entities)
{
    m_entities = entities;
}

void QTdSendMessageRequest::setReplyToMessageId(const qint64 &id)
{
    m_replyMessageId = id;
}

QJsonObject QTdSendMessageRequest::marshalJson()
{
    return QJsonObject{
        {"@type", "sendMessage"},
        {"chat_id", m_chatId},
        {"reply_to_message_id", m_replyMessageId},
        {"disable_notification", false},
        {"from_background", false},
        {"input_message_content", QJsonObject{
            {"@type", "inputMessageText"},
             {"text", QJsonObject{
                  {"@type", "formattedText"},
                  {"text", m_text},
                  {"entities", m_entities}
              }},
            {"disable_web_page_preview", false},
            {"clear_draft", true}
        }}
    };
}

QFuture<QTdResponse> QTdSendMessageRequest::sendAsync()
{
    return QTdClient::instance()->sendAsync(this, &QTdClient::message);
}
