#ifndef QTDCREATESECRETCHATREQUEST_H
#define QTDCREATESECRETCHATREQUEST_H

#include <QObject>
#include "common/qtdrequest.h"

class QTdCreateSecretChatRequest : public QTdRequest
{
    Q_OBJECT
public:
    explicit QTdCreateSecretChatRequest(QObject *parent = nullptr);

    void setUserId(const qint32 &id);
    QJsonObject marshalJson() Q_DECL_FINAL;
private:
    Q_DISABLE_COPY(QTdCreateSecretChatRequest)
    qint32 m_userId;
};

#endif // QTDCREATESECRETCHATREQUEST_H
