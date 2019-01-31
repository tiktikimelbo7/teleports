#include "qtdmessagechatdeletemember.h"
#include <QScopedPointer>
#include "user/requests/qtdgetuserrequest.h"
#include "utils/await.h"

QTdMessageChatDeleteMember::QTdMessageChatDeleteMember(QObject *parent) : QTdMessageContent(parent),
    m_user(Q_NULLPTR)
{
    setType(MESSAGE_CHAT_DELETE_MEMBER);
}

QTdUser *QTdMessageChatDeleteMember::user() const
{
    return m_user;
}

QString QTdMessageChatDeleteMember::qmlUserId() const
{
    return m_uid.toQmlValue();
}

qint32 QTdMessageChatDeleteMember::userId() const
{
    return m_uid.value();
}

void QTdMessageChatDeleteMember::unmarshalJson(const QJsonObject &json)
{
    if (json.contains("user_id")) {
        m_uid = json["user_id"];
        emit contentChanged();

        auto *user = QTdUsers::instance()->model()->getByUid(m_uid.toQmlValue());
        if (user) {
            m_user = user;
            emit userChanged();
            return;
        }

        QScopedPointer<QTdGetUserRequest> request(new QTdGetUserRequest);
        request->setUserId(m_uid.value());
        QFuture<QTdResponse> resp = request->sendAsync();
        await(resp);
        if (resp.result().isError()) {
            qWarning() << resp.result().errorString();
            return;
        }
        m_user = new QTdUser(this);
        m_user->unmarshalJson(resp.result().json());
        emit userChanged();
    }
}


