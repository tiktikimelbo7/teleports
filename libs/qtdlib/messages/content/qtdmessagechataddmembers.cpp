#include "qtdmessagechataddmembers.h"
#include <QDebug>
#include <QJsonArray>
#include <QScopedPointer>
#include "user/requests/qtdgetuserrequest.h"
#include "utils/await.h"
#include "utils/i18n.h"

QTdMessageChatAddMembers::QTdMessageChatAddMembers(QObject *parent)
    : QTdMessageContent(parent)
    , m_model(new QTdUsersSortFilterModel)
{
    setType(MESSAGE_CHAT_ADD_MEMBERS);
    m_model->setSourceModel(QTdUsers::instance()->model());
}

QObject *QTdMessageChatAddMembers::members() const
{
    return m_model.data();
}

QList<qint32> QTdMessageChatAddMembers::memberUserIds() const
{
    return m_member_user_ids;
}

qint32 QTdMessageChatAddMembers::firstMemberId() const
{
    return m_member_user_ids.first();
}

void QTdMessageChatAddMembers::setSenderUserId(const qint32 senderUserId)
{
    m_senderUserId = senderUserId;
}

void QTdMessageChatAddMembers::unmarshalJson(const QJsonObject &json)
{
    const QJsonArray ids = json["member_user_ids"].toArray();
    QScopedPointer<QTdGetUserRequest> request(new QTdGetUserRequest);
    for (const QJsonValue &val : ids) {
        m_member_user_ids << qint32(val.toInt());
    }
    m_model->setAllowedUsers(m_member_user_ids);
    emit membersChanged();

    // Now fetch any users that aren't already in the model
    for (const qint32 &id : m_member_user_ids) {
        const QTdUser *user = QTdUsers::instance()->model()->getByUid(QString::number(id));
        if (!user) {
            QScopedPointer<QTdGetUserRequest> req(new QTdGetUserRequest);
            req->setUserId(id);
            /**
             * We don't need to wait on a response as it will make its
             * way to the users model anyway and then the sort filter will
             * kick in.
             */
            req->send();
        }
    }
    m_typeText = m_senderUserId == firstMemberId() ? gettext("joined the group") : gettext("added one or more members");
}
