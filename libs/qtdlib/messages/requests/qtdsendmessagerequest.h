#ifndef QTDSENDMESSAGEREQUEST_H
#define QTDSENDMESSAGEREQUEST_H

#include <QObject>
#include <QJsonArray>

#include "common/qtdrequest.h"

/**
 * @brief The QTdSendMessageRequest class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1send_message.html
 */
class QTdSendMessageRequest : public QTdRequest
{
    Q_OBJECT
    Q_DISABLE_COPY(QTdSendMessageRequest)
    qint64 m_chatId;
    qint64 m_replyMessageId;
    QString m_text;
    QJsonArray m_entities;
  
  public:
    explicit QTdSendMessageRequest(QObject *parent = nullptr);
    void setText(const QString &text);
    void setChatId(const qint64 &id);
    void setEntities(const QJsonArray &entities);
    void setReplyToMessageId(const qint64 &id);
    QJsonObject marshalJson() Q_DECL_FINAL;
    QFuture<QTdResponse> sendAsync();
};

#endif // QTDSENDMESSAGEREQUEST_H
