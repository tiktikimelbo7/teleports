#ifndef QTDMESSAGESENDERUSER_H
#define QTDMESSAGESENDERUSER_H

#include "qtdmessagesender.h"
#include "user/qtduser.h"

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

#endif // QTDMESSAGESENDERUSER_H
