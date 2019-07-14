#ifndef QTDMESSAGEFORWARDINFO_H
#define QTDMESSAGEFORWARDINFO_H

#include "common/qabstractint64id.h"
#include <QPointer>
#include <QDateTime>

/**
 * @brief The QTdMessageForwardOrigin class
 *
 * https://web.archive.org/web/20181124110556/https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1_message_forward_info.html
 */
class QTdMessageForwardInfo : public QTdObject
{
    Q_OBJECT
public:
    explicit QTdMessageForwardInfo(QObject *parent = nullptr);
};

/**
 * @brief The QTdMessageForwardedFromUser class
 *
 * https://web.archive.org/web/20181124110746/https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1message_forwarded_from_user.html
 */
class QTdMessageForwardedFromUser : public QTdMessageForwardInfo
{
    Q_OBJECT
    Q_PROPERTY(QString senderUserId READ qmlSenderUserId NOTIFY forwardInfoChanged)
    Q_PROPERTY(QDateTime date READ qmlDate NOTIFY forwardInfoChanged)
    Q_PROPERTY(QString forwardedFromChatId READ qmlForwardedFromChatId NOTIFY forwardInfoChanged)
    Q_PROPERTY(QString forwardedFromMessageId READ qmlForwardedFromMessageId NOTIFY forwardInfoChanged)
public:
    explicit QTdMessageForwardedFromUser(QObject *parent = nullptr);

    QString qmlSenderUserId() const;
    qint32 senderUserId() const;
    QDateTime qmlDate() const;
    qint32 date() const;
    QString qmlForwardedFromChatId() const;
    qint64 forwardedFromChatId() const;
    QString qmlForwardedFromMessageId() const;
    qint64 forwardedFromMessageId() const;

    void unmarshalJson(const QJsonObject &json);

signals:
    void forwardInfoChanged();

private:
    QTdInt32 m_senderUserId;
    qint32 m_date;
    QTdInt64 m_forwardedFromChatId;
    QTdInt64 m_forwardedFromMessageId;
};
/**
 * @brief The QTdMessageForwardedFromUser class
 *
 * https://web.archive.org/web/20181124110627/https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1message_forwarded_post.html
 */
class QTdMessageForwardedPost : public QTdMessageForwardInfo
{
    Q_OBJECT
    Q_PROPERTY(QString chatId READ qmlChatId NOTIFY forwardInfoChanged)
    Q_PROPERTY(QString authorSignature READ authorSignature NOTIFY forwardInfoChanged)
    Q_PROPERTY(QDateTime date READ qmlDate NOTIFY forwardInfoChanged)
    Q_PROPERTY(QString messageId READ qmlMessageId NOTIFY forwardInfoChanged)
    Q_PROPERTY(QString forwardedFromChatId READ qmlForwardedFromChatId NOTIFY forwardInfoChanged)
    Q_PROPERTY(QString forwardedFromMessageId READ qmlForwardedFromMessageId NOTIFY forwardInfoChanged)
public:
    explicit QTdMessageForwardedPost(QObject *parent = nullptr);

    QString qmlChatId() const;
    qint64 chatId() const;
    QString authorSignature() const;
    QDateTime qmlDate() const;
    qint32 date() const;
    QString qmlMessageId() const;
    qint64 messageId() const;
    QString qmlForwardedFromChatId() const;
    qint64 forwardedFromChatId() const;
    QString qmlForwardedFromMessageId() const;
    qint64 forwardedFromMessageId() const;

    void unmarshalJson(const QJsonObject &json);

signals:
    void forwardInfoChanged();

private:
    QTdInt64 m_chatId;
    QString m_authorSignature;
    qint32 m_date;
    QTdInt64 m_messageId;
    QTdInt64 m_forwardedFromChatId;
    QTdInt64 m_forwardedFromMessageId;
};

#endif // QTDMESSAGEFORWARDINFO_H
