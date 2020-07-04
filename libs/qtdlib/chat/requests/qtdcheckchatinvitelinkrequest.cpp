#include "qtdcheckchatinvitelinkrequest.h"
#include "client/qtdclient.h"

QTdCheckChatInviteLinkRequest::QTdCheckChatInviteLinkRequest(QObject *parent)
    : QTdRequest(parent)
    , m_inviteLink("")
{
}

void QTdCheckChatInviteLinkRequest::setInviteLink(const QString &inviteLink)
{
    m_inviteLink = inviteLink;
}

QJsonObject QTdCheckChatInviteLinkRequest::marshalJson()
{
    return QJsonObject {
        { "@type", "checkChatInviteLink" },
        { "invite_link", m_inviteLink }
    };
}

QFuture<QTdResponse> QTdCheckChatInviteLinkRequest::sendAsync()
{
    return QTdClient::instance()->sendAsync(this, &QTdClient::chatInviteLinkInfo);
}
