#include "qtdchatinvitelinkinfo.h"
#include "client/qtdclient.h"

QTdChatInviteLinkInfo::QTdChatInviteLinkInfo(QObject *parent)
    : QObject(parent)
{
}

QString QTdChatInviteLinkInfo::inviteLink() const
{
    return m_inviteLink;
}

qint64 QTdChatInviteLinkInfo::chatId() const
{
    return m_chatId.value();
}

QString QTdChatInviteLinkInfo::qmlChatId() const
{
    return m_chatId.toQmlValue();
}

QTdChatType *QTdChatInviteLinkInfo::type() const
{
    return m_type.data();
}

QString QTdChatInviteLinkInfo::title() const
{
    return m_title;
}

QTdChatPhoto *QTdChatInviteLinkInfo::photo() const
{
    return m_photo.data();
}

qint32 QTdChatInviteLinkInfo::memberCount() const
{
    return m_memberCount.value();
}

QString QTdChatInviteLinkInfo::qmlMemberCount() const
{
    return m_memberCount.toQmlValue();
}

bool QTdChatInviteLinkInfo::isPublic() const
{
    return m_isPublic;
}

void QTdChatInviteLinkInfo::unmarshalJson(QJsonObject &json)
{
    qDebug() << json;
    m_inviteLink = json["invite_link"].toString();
    m_chatId = json["chat_id"].toInt();
    // m_type = json["type"].toObject()->unmarshalJson();
    m_title = json["title"].toString();
    // m_photo = json["photo"].toObject();
    m_memberCount = json["member_count"].toInt();
    m_isPublic = json["is_public"].toBool();
}
