#ifndef QTDCHATINVITELINKINFO_H
#define QTDCHATINVITELINKINFO_H

#include <QObject>
#include "common/qtdrequest.h"
#include "qtdchat.h"
#include "user/qtduser.h"

/**
 * @brief The QTdChatInviteLinkInfo class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1chat_invite_link_info.html
 */
class QTdChatInviteLinkInfo : public QObject
{
    Q_OBJECT
public:
    explicit QTdChatInviteLinkInfo(QObject *parent = nullptr);

    QString inviteLink() const;
    qint64 chatId() const;
    QString qmlChatId() const;
    QTdChatType *type() const;
    QString title() const;
    QTdChatPhoto *photo() const;
    qint32 memberCount() const;
    QString qmlMemberCount() const;
    // QQmlObjectListModel<QTdUser> *memberUsers() const;
    bool isPublic() const;

    void unmarshalJson(QJsonObject &json);

private:
    QString m_inviteLink;
    QTdInt64 m_chatId;
    QScopedPointer<QTdChatType> m_type;
    QString m_title;
    QScopedPointer<QTdChatPhoto> m_photo;
    QTdInt32 m_memberCount;
    // QScopedPointer<QQmlObjectListModel<QTdUser>> m_memberUsers;
    bool m_isPublic;
};

#endif // QTDCHATINVITELINKINFO_H
