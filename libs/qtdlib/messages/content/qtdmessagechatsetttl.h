#ifndef QTDMESSAGECHATSETTTL_H
#define QTDMESSAGECHATSETTTL_H

#include <QObject>
#include "messages/qtdmessagecontent.h"

/**
 * @brief The QTdMessageChatSetTTL class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1message_chat_set_ttl.html
 */
class QTdMessageChatSetTTL : public QTdMessageContent
{
    Q_OBJECT
    Q_DISABLE_COPY(QTdMessageChatSetTTL)
    Q_PROPERTY(QString ttl READ ttl NOTIFY contentChanged)
public:
    explicit QTdMessageChatSetTTL(QObject *parent = nullptr);

    QString ttl() const;

    void unmarshalJson(const QJsonObject &json);

signals:
    void contentChanged();

private:
    qint32 m_ttl;
};

#endif // QTDMESSAGECHATSETTTL_H
