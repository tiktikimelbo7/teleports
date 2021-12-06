#ifndef QTDSETCHATMESSAGETTLSETTINGREQUEST_H
#define QTDSETCHATMESSAGETTLSETTINGREQUEST_H

#include "common/qtdrequest.h"
#include <QObject>

/**
 * @brief The QTdSetChatMessageTtlSettingRequest class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1send_chat_set_ttl_message.html
 */
class QTdSetChatMessageTtlSettingRequest : public QTdOkRequest
{
    Q_OBJECT
    qint64 m_chatId;
    qint32 m_ttl;

public:
    explicit QTdSetChatMessageTtlSettingRequest(QObject *parent = nullptr);
    void setChatId(const qint64 &id);
    void setTTL(const qint32 &ttl);
    QJsonObject marshalJson() Q_DECL_FINAL;
};

#endif // QTDSETCHATMESSAGETTLSETTINGREQUEST_H
