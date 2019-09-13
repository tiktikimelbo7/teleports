#ifndef QTDGETSECRETCHATREQUEST_H
#define QTDGETSECRETCHATREQUEST_H

#include <QObject>
#include "common/qtdrequest.h"

/**
 * @brief The QTdGetSecretChatRequest class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1get_secret_chat.html
 */
class QTdGetSecretChatRequest : public QTdRequest
{
    Q_OBJECT
public:
    explicit QTdGetSecretChatRequest(QObject *parent = nullptr);

    void setSecretChatId(const qint32 &id);
    QJsonObject marshalJson() Q_DECL_FINAL;

    QFuture<QTdResponse> sendAsync() Q_DECL_FINAL;

private:
    Q_DISABLE_COPY(QTdGetSecretChatRequest)
    qint32 m_chatId;
};

#endif // QTDGETSECRETCHATREQUEST_H
