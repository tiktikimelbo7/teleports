#ifndef QTDSENDMESSAGEREQUEST_H
#define QTDSENDMESSAGEREQUEST_H

#include <QObject>
#include <QJsonArray>

#include "common/qtdrequest.h"

class QTdSendMessageRequest : public QTdRequest
{
    Q_OBJECT
    qint64 m_chatId;
    QString m_text;
    QJsonArray m_entities;
  
  public
      :
    explicit QTdSendMessageRequest(QObject *parent = nullptr);
    void setText(const QString &text);
    void setChatId(const qint64 &id);
    void setEntities(const QJsonArray &entities);
    QJsonObject marshalJson() Q_DECL_FINAL;
};

#endif // QTDSENDMESSAGEREQUEST_H
