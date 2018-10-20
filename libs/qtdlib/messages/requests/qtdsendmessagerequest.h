#ifndef QTDSENDMESSAGEREQUEST_H
#define QTDSENDMESSAGEREQUEST_H

#include <QObject>
#include "common/qtdrequest.h"

class QTdSendMessageRequest : public QTdRequest
{
    Q_OBJECT
    qint64 m_chatId;
    QString m_text;
public:
    explicit QTdSendMessageRequest(QObject *parent = nullptr);
    void setText(const QString &text);
    void setChatId(const qint64 &id);
    QJsonObject marshalJson() Q_DECL_FINAL;
};

#endif // QTDSENDMESSAGEREQUEST_H
