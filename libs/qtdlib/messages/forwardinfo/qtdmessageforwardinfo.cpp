#include "common/qabstracttdobject.h"
#include <QJsonValue>
#include <QDateTime>
#include "qtdmessageforwardinfo.h"
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
    if (!origin.isEmpty()) {
        qint64 originId = 0;
        const QString originType = origin["@type"].toString();
        if (originType == "messageForwardOriginChannel") {
            auto tempOrigin = new QTdMessageForwardOriginChannel(this);
            tempOrigin->unmarshalJson(origin);
            m_origin = tempOrigin;
            originId = tempOrigin->chatId();
            QScopedPointer<QTdGetChatRequest> req(new QTdGetChatRequest);
            req->setChatId(originId);
            QFuture<QTdResponse> resp = req->sendAsync();
            await(resp, 2000);
            if (resp.result().isError()) {
                qWarning() << "Failed to get chat with error: " << resp.result().errorString();
                m_displayedName = "";
            } else if (!resp.result().json().isEmpty()) {
                auto tempChat = new QTdChat(this);
                tempChat->unmarshalJson(resp.result().json());
                m_displayedName = tempChat->title();
                delete tempChat;
            }
        } else if (originType == "messageForwardOriginHiddenUser") {
            auto tempOrigin = new QTdMessageForwardOriginHiddenUser(this);
            tempOrigin->unmarshalJson(origin);
            m_origin = tempOrigin;
            m_displayedName = tempOrigin->senderName();
        } else if (originType == "messageForwardOriginUser") {
            auto tempOrigin = new QTdMessageForwardOriginUser(this);
            tempOrigin->unmarshalJson(origin);
            m_origin = tempOrigin;
            originId = tempOrigin->senderUserId();
            auto *user = QTdUsers::instance()->model()->getByUid(QString::number(originId));
            if (!user) {
                QScopedPointer<QTdGetUserRequest> req(new QTdGetUserRequest);
                req->setUserId(originId);
                QFuture<QTdResponse> resp = req->sendAsync();
                await(resp, 2000);
                if (resp.result().isError()) {
                    qWarning() << "Failed to get user with error: " << resp.result().errorString();
                    m_displayedName = "";
                } else if (!resp.result().json().isEmpty()) {
                    auto tempUser = new QTdUser(this);
                    tempUser->unmarshalJson(resp.result().json());
                    m_displayedName = tempUser->firstName() + " " + tempUser->lastName();
                    delete tempUser;
                }
            } else {
                m_displayedName = user->firstName() + " " + user->lastName();
            }
        }
    }
    m_date = json["date"].toInt();
    m_fromChatId = json["forwarded_from_chat_id"].toInt();
    m_fromMessageId = json["forwarded_from_message_id"].toInt();
    emit forwardInfoChanged();
}