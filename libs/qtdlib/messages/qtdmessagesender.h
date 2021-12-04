#ifndef QTDMESSAGESENDER_H
#define QTDMESSAGESENDER_H

#include <QObject>
#include "common/qabstracttdobject.h"
#include "common/qtdint.h"
#include "user/qtduser.h"

/**
 * @brief The QTdMessageSender class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1_message_sender.html
 */
class QTdMessageSender : public QTdObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QTdMessageSender)
    Q_PROPERTY(QString id READ id NOTIFY dataChanged)
    Q_PROPERTY(QString fullName READ fullName NOTIFY dataChanged)
    Q_PROPERTY(QString displayName READ displayName NOTIFY dataChanged)
    Q_PROPERTY(QTdPhoto *photo READ photo NOTIFY dataChanged)
    Q_PROPERTY(QString avatarColor READ avatarColor NOTIFY dataChanged)
    Q_PROPERTY(QString initials READ initials NOTIFY dataChanged)
public:
    explicit QTdMessageSender(QObject *parent = nullptr);

    static QTdMessageSender *create(const QJsonObject &json, QObject *parent = nullptr);

    virtual QString id() const = 0;
    virtual QString displayName() const = 0;
    virtual QString fullName() const = 0;
    virtual QTdPhoto *photo() const = 0;
    virtual QString avatarColor() const = 0;
    virtual QString initials() const = 0;
signals:
    void dataChanged();
};

/**
 * @brief The QTdMessageSenderUnimplemented class
 *
 * Stops the app from crashing if Telegram adds an unsupported sender type
 */
class QTdMessageSenderUnimplemented : public QTdMessageSender
{
    Q_OBJECT
public:
    explicit QTdMessageSenderUnimplemented(QObject *parent = nullptr);

    QString id() const;
    QString displayName() const;
    QString fullName() const;
    QTdPhoto *photo() const;
    QString avatarColor() const;
    QString initials() const;

private:
    Q_DISABLE_COPY(QTdMessageSenderUnimplemented)
};

/**
 * @brief The QTdMessageSenderUser class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1message_sender_user.html
 */
class QTdMessageSenderUser : public QTdMessageSender
{
    Q_OBJECT
    Q_PROPERTY(QString userId READ qmlUserId NOTIFY userIdChanged)
    Q_PROPERTY(QTdUser *user READ user NOTIFY userChanged)
public:
    explicit QTdMessageSenderUser(QObject *parent = nullptr);

    QString qmlUserId() const;
    qint64 userId() const;
    QTdUser *user() const;
    QString id() const;
    QString displayName() const;
    QString fullName() const;
    QTdPhoto *photo() const;
    QString avatarColor() const;
    QString initials() const;

    void unmarshalJson(const QJsonObject &json);
signals:
    void userIdChanged();
    void userChanged();

private:
    Q_DISABLE_COPY(QTdMessageSenderUser)
    QTdInt64 m_userId;
    QTdUser *m_user;
    bool m_waitingForUser;

    void updateUser(const qint64 &userId);
};

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

private:
    Q_DISABLE_COPY(QTdMessageSenderChat)
    QTdInt64 m_chatId;
};

#endif // QTDMESSAGESENDER_H
