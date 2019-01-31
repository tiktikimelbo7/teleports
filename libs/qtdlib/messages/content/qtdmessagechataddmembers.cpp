#include "qtdmessagechataddmembers.h"
#include <QDebug>
#include <QJsonArray>
#include <QScopedPointer>
#include "user/requests/qtdgetuserrequest.h"
#include "utils/await.h"

QTdMessageChatAddMembers::QTdMessageChatAddMembers(QObject *parent) : QTdMessageContent(parent),
    m_model(Q_NULLPTR)
{
    setType(MESSAGE_CHAT_ADD_MEMBERS);
    m_model = new QQmlObjectListModel<QTdUser>(this, "", "id");
}

QObject *QTdMessageChatAddMembers::members() const
{
    return m_model;
}

QList<QTdInt32> QTdMessageChatAddMembers::memberUserIds() const
{
    return m_member_user_ids;
}

void QTdMessageChatAddMembers::unmarshalJson(const QJsonObject &json)
{
    const QJsonArray ids = json["member_user_ids"].toArray();
    QScopedPointer<QTdGetUserRequest> request(new QTdGetUserRequest);
    for (auto val : ids) {
        QTdInt32 id(qint32(val.toInt()));
        m_member_user_ids << id;
        request->setUserId(id.value());
        QFuture<QTdResponse> user = request->sendAsync();
        await(user);
        if (user.result().isError()) {
            qWarning() << user.result().errorString();
            continue;
        }
        QTdUser *u = new QTdUser();
        u->unmarshalJson(user.result().json());
        m_model->append(u);
    }
    emit membersChanged();
}
