#ifndef QTDMESSAGECHATDELETEMEMBER_H
#define QTDMESSAGECHATDELETEMEMBER_H

#include <QObject>
#include <QFutureWatcher>
#include <QPointer>
#include "common/qtdresponse.h"
#include "messages/qtdmessagecontent.h"
#include "user/qtdusers.h"

/**
 * @brief The QTdMessageChatDeleteMember class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1message_chat_delete_member.html
 */
class QTdMessageChatDeleteMember : public QTdMessageContent
{
    Q_OBJECT
    Q_PROPERTY(QTdUser *user READ user NOTIFY userChanged)
    Q_PROPERTY(QString userId READ qmlUserId NOTIFY contentChanged)
public:
    explicit QTdMessageChatDeleteMember(QObject *parent = nullptr);

    QTdUser *user() const;
    QString qmlUserId() const;
    qint32 userId() const;
    void setSenderUserId(const qint32 senderUserId);

    void unmarshalJson(const QJsonObject &json) Q_DECL_FINAL;

signals:
    void contentChanged();
    void userChanged();

private slots:
    void handleResponse();

private:
    void updateTypeText();
    QPointer<QTdUser> m_user;
    QTdInt32 m_uid;
    QFutureWatcher<QTdResponse> m_watcher;
    qint32 m_senderUserId;
};

#endif // QTDMESSAGECHATDELETEMEMBER_H
