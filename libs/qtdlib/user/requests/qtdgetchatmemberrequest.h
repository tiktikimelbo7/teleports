#ifndef QTDGETCHATMEMBERREQUEST_H
#define QTDGETCHATMEMBERREQUEST_H

#include <QObject>
#include "common/qtdrequest.h"
#include "common/qtdint.h"

/**
 * @brief The QTdGetChatMemberRequest class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1get_chat_member.html
 */
class QTdGetChatMemberRequest : public QTdRequest
{
    Q_OBJECT
public:
    explicit QTdGetChatMemberRequest(QObject *parent = nullptr);

    void setChatId(const QString &chatId);
    void setChatId(const qint64 &chatId);
    void setUserId(const QString &userId);
    void setUserId(const qint32 &userId);

    QJsonObject marshalJson() Q_DECL_FINAL;
    QFuture<QTdResponse> sendAsync() Q_DECL_FINAL;

private:
    QTdInt64 m_chatId;
    QTdInt32 m_userId;
};

#endif // QTDGETCHATMEMBERREQUEST_H
