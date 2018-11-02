#ifndef QTDSENDCHATACTIONREQUEST_H
#define QTDSENDCHATACTIONREQUEST_H

#include "common/qtdrequest.h"
#include "chat/qtdchatactionfactory.h"
#include <QObject>

class QTdSendChatActionRequest : public QTdRequest
{
    Q_OBJECT
    qint64 m_chatId;
public:
  explicit QTdSendChatActionRequest(QObject *parent = nullptr);
  void setChatId(const qint64 &id);
  void setAction(const QTdChatAction &action);
  QJsonObject marshalJson() Q_DECL_FINAL;
};

#endif // QTDCLOSECHATREQUEST_H
