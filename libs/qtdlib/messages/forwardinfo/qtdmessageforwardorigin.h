#ifndef QTDMESSAGEFORWARDORIGIN_H
#define QTDMESSAGEFORWARDORIGIN_H

#include "common/qabstractint64id.h"
#include <QPointer>

/**
 * @brief The QTdMessageForwardOrigin class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1_message_forward_origin.html
 */
class QTdMessageForwardOrigin : public QTdObject
{
    Q_OBJECT
    // Q_DISABLE_COPY(QTdMessageForwardOrigin)
public:
    explicit QTdMessageForwardOrigin(QObject *parent = nullptr);
};

/**
     * @brief The QTdMessageForwardOriginChat class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1message_forward_origin_chat.html
 */
class QTdMessageForwardOriginChat : public QTdMessageForwardOrigin
{
    Q_OBJECT
    // Q_DISABLE_COPY(QTdMessageForwardOriginChat)
    Q_PROPERTY(QString chatId READ qmlChatId NOTIFY forwardOriginChanged)
    Q_PROPERTY(QString authorSignature READ authorSignature NOTIFY forwardOriginChanged)
public:
    explicit QTdMessageForwardOriginChat(QObject *parent = nullptr);

    QString qmlChatId() const;
    qint64 chatId() const;
    QString authorSignature() const;

    void unmarshalJson(const QJsonObject &json);

signals:
    void forwardOriginChanged();

private:
    QTdInt64 m_chatId;
    QTdInt64 m_messageId;
    QString m_authorSignature;
};

/**
     * @brief The QTdMessageForwardOriginChannel class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1message_forward_origin_channel.html
 */
class QTdMessageForwardOriginChannel : public QTdMessageForwardOrigin
{
    Q_OBJECT
    // Q_DISABLE_COPY(QTdMessageForwardOriginChannel)
    Q_PROPERTY(QString chatId READ qmlChatId NOTIFY forwardOriginChanged)
    Q_PROPERTY(QString messageId READ qmlMessageId NOTIFY forwardOriginChanged)
    Q_PROPERTY(QString authorSignature READ authorSignature NOTIFY forwardOriginChanged)
public:
    explicit QTdMessageForwardOriginChannel(QObject *parent = nullptr);

    QString qmlChatId() const;
    qint64 chatId() const;
    QString qmlMessageId() const;
    qint64 messageId() const;
    QString authorSignature() const;

    void unmarshalJson(const QJsonObject &json);

signals:
    void forwardOriginChanged();

private:
    QTdInt64 m_chatId;
    QTdInt64 m_messageId;
    QString m_authorSignature;
};

/**
 * @brief The QTdMessageForwardOriginHiddenUser class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1message_forward_origin_hidden_user.html
 */
class QTdMessageForwardOriginHiddenUser : public QTdMessageForwardOrigin
{
    Q_OBJECT
    // Q_DISABLE_COPY(QTdMessageForwardOriginHiddenUser)
    Q_PROPERTY(QString senderName READ senderName NOTIFY senderNameChanged)
public:
    explicit QTdMessageForwardOriginHiddenUser(QObject *parent = nullptr);

    QString senderName() const;

    void unmarshalJson(const QJsonObject &json);

signals:
    void senderNameChanged();

private:
    QString m_senderName;
};

/**
 * @brief The QTdMessageForwardOriginUser class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1message_forward_origin_user.html
 */
class QTdMessageForwardOriginUser : public QTdMessageForwardOrigin
{
    Q_OBJECT
    // Q_DISABLE_COPY(QTdMessageForwardOriginUser)
    Q_PROPERTY(QString senderUserId READ qmlSenderUserId NOTIFY senderUserIdChanged)
public:
    explicit QTdMessageForwardOriginUser(QObject *parent = nullptr);

    QString qmlSenderUserId() const;
    qint32 senderUserId() const;

    void unmarshalJson(const QJsonObject &json);

signals:
    void senderUserIdChanged();

private:
    QTdInt32 m_senderUserId;
};

#endif // QTDMESSAGEFORWARDORIGIN_H
