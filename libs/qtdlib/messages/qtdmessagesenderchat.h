#ifndef QTDMESSAGESENDERCHAT_H
#define QTDMESSAGESENDERCHAT_H

#include "qtdmessagesender.h"
#include "chat/qtdchat.h"

/**
 * @brief The QTdMessageSenderChat class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1message_sender_chat.html
 */
class QTdMessageSenderChat : public QTdMessageSender
{
    Q_OBJECT
    Q_PROPERTY(QString chatId READ qmlChatId NOTIFY chatIdChanged)
public:
    explicit QTdMessageSenderChat(QObject *parent = nullptr);

    QString qmlChatId() const;
    qint64 chatId() const;
    QString id() const;
    QString displayName() const;
    QString fullName() const;
    QTdPhoto *photo() const;
    QString avatarColor() const;
    QString initials() const;

    void unmarshalJson(const QJsonObject &json);
signals:
    void chatIdChanged();
    void chatChanged();

private:
    Q_DISABLE_COPY(QTdMessageSenderChat)
    QTdInt64 m_chatId;
    QTdChat *m_chat;
    bool m_waitingForChat;

    void updateChat(const qint64 &chatId);
};

#endif // QTDMESSAGESENDERCHAT_H
