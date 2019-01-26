#ifndef QTDEDITMESSAGEREQUEST_H
#define QTDEDITMESSAGEREQUEST_H

#include <QObject>
#include <QJsonArray>

#include "common/qtdrequest.h"

class QTdEditMessageRequest : public QTdRequest
{
    Q_OBJECT
    qint64 m_chatId;
    qint64 m_messageId;
    QString m_text;
    QJsonArray m_entities;

  public:
    explicit QTdEditMessageRequest(QObject *parent = nullptr);
    void setText(const QString &text);
    void setChatId(const qint64 &id);
    void setMessageId(const qint64 &id);
    void setEntities(const QJsonArray &entities);
    QJsonObject marshalJson() Q_DECL_FINAL;
};

#endif // QTDEDITMESSAGEREQUEST_H
