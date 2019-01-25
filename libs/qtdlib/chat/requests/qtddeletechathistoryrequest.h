#ifndef QTDDELETECHATHISTORYREQUEST_H
#define QTDDELETECHATHISTORYREQUEST_H

#include <QObject>
#include "common/qtdrequest.h"

class QTdDeleteChatHistoryRequest : public QTdRequest
{
    Q_OBJECT
    qint64 m_chatId;
    bool m_removeFromChatList;
public:
    explicit QTdDeleteChatHistoryRequest(QObject *parent = nullptr);
    void setChatId(const qint64 &id);
    void setRemoveFromChatList(const bool &remove);
    QJsonObject marshalJson() Q_DECL_FINAL;
};

#endif // QTDDELETECHATHISTORYREQUEST_H
