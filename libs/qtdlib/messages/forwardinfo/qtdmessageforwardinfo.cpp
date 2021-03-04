#include "common/qabstracttdobject.h"
#include <QJsonValue>
#include <QDateTime>
#include "qtdmessageforwardinfo.h"
#include "qtdmessageforwardoriginfactory.h"
#include "common/qtdhelpers.h"
#include "user/requests/qtdgetuserrequest.h"
#include "chat/requests/qtdgetchatrequest.h"
#include "chat/qtdchat.h"
#include "utils/await.h"
#include "user/qtdusers.h"

QTdMessageForwardInfo::QTdMessageForwardInfo(QObject *parent)
    : QTdObject(parent)
    , m_origin(Q_NULLPTR)
    , m_date(0)
    , m_fromChatId(0)
    , m_fromMessageId(0)
{
}

QTdMessageForwardOrigin *QTdMessageForwardInfo::origin() const
{
    return m_origin;
}

QDateTime QTdMessageForwardInfo::qmlDate() const
{
    return QDateTime::fromTime_t(m_date);
}
qint32 QTdMessageForwardInfo::date() const
{
    return m_date;
}

QString QTdMessageForwardInfo::qmlFromChatId() const
{
    return m_fromChatId.toQmlValue();
}
qint64 QTdMessageForwardInfo::fromChatId() const
{
    return m_fromChatId.value();
}

QString QTdMessageForwardInfo::qmlFromMessageId() const
{
    return m_fromMessageId.toQmlValue();
}
qint64 QTdMessageForwardInfo::fromMessageId() const
{
    return m_fromMessageId.value();
}

QString QTdMessageForwardInfo::displayedName() const
{
    return m_displayedName;
}

void QTdMessageForwardInfo::unmarshalJson(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }
    const QJsonObject origin = json["origin"].toObject();
    m_origin = QTdMessageForwardOriginFactory::create(origin, this);
    if (m_origin) {
        switch (m_origin->type()) {
        case QTdMessageForwardOrigin::Type::MESSAGE_FORWARD_ORIGIN_CHAT:
        case QTdMessageForwardOrigin::Type::MESSAGE_FORWARD_ORIGIN_CHANNEL:
            auto *chat = QTdChats::instance()->model()->getByUid(QString::number(originId));
            if (!chat) {
                QScopedPointer<QTdGetChatRequest> req(new QTdGetChatRequest);
                req->setChatId(originId);
                QFuture<QTdResponse> resp = req->sendAsync();
                await(resp, 2000);
                if (resp.result().isError()) {
                    qWarning() << "Failed to get chat with error: " << resp.result().errorString();
                    m_displayedName = "";
                } else {
                    if (!resp.result().json().isEmpty()) {
                        auto tempChat = new QTdChat(this);
                        tempChat->unmarshalJson(resp.result().json());
                        m_displayedName = tempChat->title();
                        delete tempChat;
                    }
                }
            } else {
                m_displayedName = chat->title();
            }
            break;
        case QTdMessageForwardOrigin::Type::MESSAGE_FORWARD_ORIGIN_HIDDEN_USER:
            auto hiddenUserOrigin = qobject_cast<QTdMessageForwardOriginHiddenUser *>(m_origin);
            m_displayedName = hiddenUserOrigin->senderName();
            break;
        case QTdMessageForwardOrigin::Type::MESSAGE_FORWARD_ORIGIN_USER:
            auto originId = qobject_cast<QTdMessageForwardOriginUser *>(m_origin)->senderUserId();
            auto *user = QTdUsers::instance()->model()->getByUid(QString::number(originId));
            if (!user) {
                QScopedPointer<QTdGetUserRequest> req(new QTdGetUserRequest);
                req->setUserId(originId);
                QFuture<QTdResponse> resp = req->sendAsync();
                await(resp, 2000);
                if (resp.result().isError()) {
                    qWarning() << "Failed to get user with error: " << resp.result().errorString();
                    m_displayedName = "";
                } else {
                    if (!resp.result().json().isEmpty()) {
                        auto tempUser = new QTdUser(this);
                        tempUser->unmarshalJson(resp.result().json());
                        m_displayedName = tempUser->firstName() + " " + tempUser->lastName();
                        delete tempUser;
                    }
                }
            } else {
                m_displayedName = user->firstName() + " " + user->lastName();
            }
            break;
        }
    }
    m_date = json["date"].toInt();
    m_fromChatId = json["forwarded_from_chat_id"].toInt();
    m_fromMessageId = json["forwarded_from_message_id"].toInt();
    emit forwardInfoChanged();
}