#include "qtdsendmessagerequest.h"
#include <QJsonArray>

QTdSendMessageRequest::QTdSendMessageRequest(QObject *parent) : QTdRequest(parent),
  m_chatId(0)
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

QJsonObject QTdSendMessageRequest::marshalJson()
{
    return QJsonObject{
        {"@type", "sendMessage"},
        {"chat_id", m_chatId},
        {"reply_to_message_id", 0}, // TODO: Implement replys
        {"disable_notification", false},
        {"from_background", false},
        {"input_message_content", QJsonObject{
            {"@type", "inputMessageText"},
             {"text", QJsonObject{
                  {"@type", "formattedText"},
                  {"text", m_text},
                  {"entities", QJsonArray()}
              }},
            {"disable_web_page_preview", true},
            {"clear_draft", false}
        }}
    };
}
