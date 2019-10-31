#ifndef QTDMESSAGEPINMESSAGE_H
#define QTDMESSAGEPINMESSAGE_H

#include <QObject>
#include <QPointer>
#include "models/QmlObjectListModel.h"
#include "messages/qtdmessagecontent.h"
#include "qtdmessagechataddmembers.h"
#include "user/qtdusers.h"
#include "messages/qtdmessage.h"

/**
 * @brief The QTdMessageBasicGroupChatCreate class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1message_pin_message.html
 */
class QTdMessagePinMessage : public QTdMessageContent
{
    Q_OBJECT
    Q_DISABLE_COPY(QTdMessagePinMessage)
    Q_PROPERTY(QString message READ message NOTIFY messageChanged)
    Q_PROPERTY(QString messageId READ messageId NOTIFY messageIdChanged)
    Q_PROPERTY(qint64 chatId WRITE setChatId)
public:
    explicit QTdMessagePinMessage(QObject *parent = nullptr);

    QString message() const;
    qint64 messageId() const;
    void setChatId(const qint64 &chatId);

    void unmarshalJson(const QJsonObject &json) Q_DECL_FINAL;
    
signals:
    void messageChanged();
    void messageIdChanged();

private:
    QString getMessageString();
    QPointer<QTdMessage> m_message;
    QString m_messageString;
    qint64 m_messageId;
    qint64 m_chatId;
};

#endif // QTDMESSAGEPINMESSAGE_H
