#include "qtdsupergroupchat.h"
#include <QDateTime>
#include "chat/requests/qtdgetsupergrouprequest.h"
#include "chat/requests/qtdgetsupergroupfullinforequest.h"
#include "client/qtdclient.h"
#include "common/qabstracttdobject.h"

QTdSuperGroupChat::QTdSuperGroupChat(QObject *parent)
    : QTdChat(parent)
    , m_sgId(0)
    , m_date(0)
    , m_status(Q_NULLPTR)
    , m_memberCount(0)
    , m_signMessages(false)
    , m_isChannel(false)
    , m_isVerified(false)
    , m_adminCount(0)
    , m_restrictedCount(0)
    , m_bannedCount(0)
    , m_canGetMembers(false)
    , m_canSetUsername(false)
    , m_canSetStickerSet(false)
    , m_historyAvailable(false)
    , m_stickerSet(0)
    , m_pinnedMessageId(0)
    , m_upgradeGroupId(0)
    , m_upgradeMaxMsgId(0)
{
    connect(QTdClient::instance(), &QTdClient::superGroup, this, &QTdSuperGroupChat::updateSuperGroup);
    connect(QTdClient::instance(), &QTdClient::updateSuperGroup, this, &QTdSuperGroupChat::updateSuperGroup);
    connect(QTdClient::instance(), &QTdClient::updateSupergroupFullInfo, this, &QTdSuperGroupChat::updateSuperGroupFullInfo);
    connect(QTdClient::instance(), &QTdClient::supergroupFullInfo, this, &QTdSuperGroupChat::handleSuperGroupFullInfo);
}

QString QTdSuperGroupChat::qmlSuperGroupId() const
{
    return m_sgId.toQmlValue();
}

qint64 QTdSuperGroupChat::superGroupId() const
{
    return m_sgId.value();
}

QString QTdSuperGroupChat::userName() const
{
    return m_username;
}

QDateTime QTdSuperGroupChat::qmlDate() const
{
    return QDateTime::fromTime_t(m_date);
}

qint32 QTdSuperGroupChat::date() const
{
    return m_date;
}

QTdChatMemberStatus *QTdSuperGroupChat::status() const
{
    return m_status;
}

QString QTdSuperGroupChat::qmlMemberCount() const
{
    return m_memberCount.toQmlValue();
}

qint32 QTdSuperGroupChat::memberCount() const
{
    return m_memberCount.value();
}

bool QTdSuperGroupChat::signMessages() const
{
    return m_signMessages;
}

bool QTdSuperGroupChat::isChannel() const
{
    return m_isChannel;
}

bool QTdSuperGroupChat::isVerified() const
{
    return m_isVerified;
}

bool QTdSuperGroupChat::isWritable() const
{
    auto atLeastAdmin = m_status.data()->type() == QTdObject::Type::CHAT_MEMBER_STATUS_ADMIN
            || m_status.data()->type() == QTdObject::Type::CHAT_MEMBER_STATUS_CREATOR;
    switch (m_status->type()) {
    case QTdObject::Type::CHAT_MEMBER_STATUS_ADMIN: {
        auto adminStatus = qobject_cast<QTdChatMemberStatusAdmin *>(m_status);
        if (m_isChannel) {
            return adminStatus->canPostMessages();
        }
        return true;
        break;
    }
    case QTdObject::Type::CHAT_MEMBER_STATUS_CREATOR: {
        auto creatorStatus = qobject_cast<QTdChatMemberStatusCreator *>(m_status);
        return creatorStatus->isMember();
        break;
    }
    case QTdObject::Type::CHAT_MEMBER_STATUS_BANNED:
    case QTdObject::Type::CHAT_MEMBER_STATUS_LEFT: {
        return false;
        break;
    }
    case QTdObject::Type::CHAT_MEMBER_STATUS_MEMBER: {
        return !m_isChannel;
        break;
    }
    case QTdObject::Type::CHAT_MEMBER_STATUS_RESTRICTED: {
        auto restrictedStatus = qobject_cast<QTdChatMemberStatusRestricted *>(m_status);
        if (restrictedStatus->isMember()) {
            // TODO implement chatPermission in QTdChatMemberStatusRestricted and read canSendMessages
            // until them return always true as the message sending will just fail if canSendMessages == false
            return true;
        }
        return false;
        break;
    }
    }
}

QString QTdSuperGroupChat::restrictionReason() const
{
    return m_restrictionReason;
}

QString QTdSuperGroupChat::description() const
{
    return m_description;
}

QString QTdSuperGroupChat::qmlAdminCount() const
{
    return m_adminCount.toQmlValue();
}

qint32 QTdSuperGroupChat::adminCount() const
{
    return m_adminCount.value();
}

QString QTdSuperGroupChat::qmlRestrictedCount() const
{
    return m_restrictedCount.toQmlValue();
}

qint32 QTdSuperGroupChat::restrictedCount() const
{
    return m_restrictedCount.value();
}

QString QTdSuperGroupChat::qmlBannedCount() const
{
    return m_bannedCount.toQmlValue();
}

qint32 QTdSuperGroupChat::bannedCount() const
{
    return m_bannedCount.value();
}

bool QTdSuperGroupChat::isHistoryAvailable() const
{
    return m_historyAvailable;
}

QString QTdSuperGroupChat::qmlStickerSetId() const
{
    return m_stickerSet.toQmlValue();
}

qint64 QTdSuperGroupChat::stickerSetId() const
{
    return m_stickerSet.value();
}

QString QTdSuperGroupChat::inviteLink() const
{
    return m_inviteLink;
}

QString QTdSuperGroupChat::qmlPinnedMessageId() const
{
    return m_pinnedMessageId.toQmlValue();
}

qint64 QTdSuperGroupChat::pinnedMessageId() const
{
    return m_pinnedMessageId.value();
}

QString QTdSuperGroupChat::qmlUpgradedFromBasicGroupId() const
{
    return m_upgradeGroupId.toQmlValue();
}

qint64 QTdSuperGroupChat::upgradedFromBasicGroupId() const
{
    return m_upgradeGroupId.value();
}

QString QTdSuperGroupChat::qmlUpgradedFromMaxMessageId() const
{
    return m_upgradeMaxMsgId.toQmlValue();
}

qint64 QTdSuperGroupChat::upgradedFromMaxMessageId() const
{
    return m_upgradeMaxMsgId.value();
}

void QTdSuperGroupChat::onChatDeserialized()
{
    getSuperGroupData();
}

void QTdSuperGroupChat::parseSuperGroupId() {
    QTdChatTypeSuperGroup *group = qobject_cast<QTdChatTypeSuperGroup *>(chatType());
    if (group && group->superGroupId() > 0) {
        m_sgId = group->superGroupId();
    }
}
void QTdSuperGroupChat::getSuperGroupFullInfo()
{
    if (m_sgId.value() == 0) {
        parseSuperGroupId();
    }
    QScopedPointer<QTdGetSuperGroupFullInfoRequest> req(new QTdGetSuperGroupFullInfoRequest);
    req->setSupergroupId(superGroupId());
    QTdClient::instance()->send(req.data());
}

void QTdSuperGroupChat::getSuperGroupData()
{
    if (m_sgId.value() == 0) {
        parseSuperGroupId();
    }
    QScopedPointer<QTdGetSuperGroupRequest> req(new QTdGetSuperGroupRequest);
    req->setSuperGroupId(superGroupId());
    QTdClient::instance()->send(req.data());
}

void QTdSuperGroupChat::updateSuperGroup(const QJsonObject &json)
{
    const qint64 gid = json["id"].toVariant().toLongLong();
    if (gid != superGroupId()) {
        return;
    }

    if (m_status) {
        delete m_status;
        m_status = nullptr;
    }
    const QJsonObject status = json["status"].toObject();
    const QString type = status["@type"].toString();
    if (type == "chatMemberStatusAdministrator") {
        m_status = new QTdChatMemberStatusAdmin(this);
    } else if (type == "chatMemberStatusBanned") {
        m_status = new QTdChatMemberStatusBanned(this);
    } else if (type == "chatMemberStatusCreator") {
        m_status = new QTdChatMemberStatusCreator(this);
    } else if (type == "chatMemberStatusLeft") {
        m_status = new QTdChatMemberStatusLeft(this);
    } else if (type == "chatMemberStatusMember") {
        m_status = new QTdChatMemberStatusMember(this);
    } else if (type == "chatMemberStatusRestricted") {
        m_status = new QTdChatMemberStatusRestricted(this);
    }
    if (m_status) {
        m_status->unmarshalJson(status);
    }

    m_signMessages = json["sign_messages"].toBool();
    m_isChannel = json["is_channel"].toBool();
    m_isVerified = json["is_verified"].toBool();
    m_restrictionReason = json["restriction_reason"].toString();
    m_username = json["username"].toString();
    emit isWritableChanged();
    emit superGroupChanged();
    emit chatStatusChanged();
    getSuperGroupFullInfo();
}

void QTdSuperGroupChat::updateSuperGroupFullInfo(const QJsonObject &json)
{
    const qint64 sid = json["supergroup_id"].toVariant().toLongLong();
    if (sid != superGroupId()) {
        return;
    }
    const QJsonObject info = json["supergroup_full_info"].toObject();
    superGroupFullInfo(info);
}

void QTdSuperGroupChat::handleSuperGroupFullInfo(const QJsonObject &json)
{
    const QString sgid = json["@extra"].toString();
    if (sgid != QString::number(superGroupId())) {
        return;
    }
    superGroupFullInfo(json);
}

void QTdSuperGroupChat::superGroupFullInfo(const QJsonObject &info)
{
    m_description = info["description"].toString();
    m_memberCount = info["member_count"];
    m_adminCount = info["administrator_count"];
    m_restrictedCount = info["restricted_count"];
    m_bannedCount = info["banned_count"];
    m_canGetMembers = info["can_get_members"].toBool();
    m_canSetUsername = info["can_set_username"].toBool();
    m_canSetStickerSet = info["can_set_sticker_set"].toBool();
    m_historyAvailable = info["is_all_history_available"].toBool();
    m_stickerSet = info["sticker_set_id"];
    m_inviteLink = info["invite_link"].toString();
    m_upgradeGroupId = info["upgraded_from_basic_group_id"];
    m_upgradeMaxMsgId = info["upgraded_from_max_message_id"];
    emit superGroupInfoChanged();
}
