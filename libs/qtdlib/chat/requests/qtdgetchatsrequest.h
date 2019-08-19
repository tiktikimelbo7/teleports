#ifndef QTDGETCHATSREQUEST_H
#define QTDGETCHATSREQUEST_H

#include <QObject>
#include <QtConcurrent>
#include "common/qtdrequest.h"
#include "common/qtdresponse.h"

/**
 * @brief The QTdGetChatsRequest class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1get_chats.html
 */
class QTdGetChatsRequest : public QTdRequest
{
    Q_OBJECT
public:
    explicit QTdGetChatsRequest(QObject *parent = nullptr);

    void setOffsetOrder(const qint64 value);

    void setOffsetChatId(const qint64 value);

    QJsonObject marshalJson() Q_DECL_FINAL;

    QFuture<QTdResponse> sendAsync() Q_DECL_FINAL;

private:
    qint64 m_offset_order;
    qint64 m_offset_chat_id;
};

#endif // QTDGETCHATSREQUEST_H
