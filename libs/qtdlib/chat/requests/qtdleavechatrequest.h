#ifndef QTDLEAVECHATREQUEST_H
#define QTDLEAVECHATREQUEST_H

#include <QObject>
#include "common/qtdrequest.h"

class QTdLeaveChatRequest : public QTdRequest
{
    Q_OBJECT
    qint64 m_chatId;
public:
    explicit QTdLeaveChatRequest(QObject *parent = nullptr);
    void setChatId(const qint64 &id);
    QJsonObject marshalJson() Q_DECL_FINAL;
};

#endif // QTDLEAVECHATREQUEST_H
