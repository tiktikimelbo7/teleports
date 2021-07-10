#include "common/qabstracttdobject.h"
#include <QJsonValue>
#include <QDateTime>
#include "qtdmessageforwardinfo.h"
#include "qtdmessageforwardoriginfactory.h"
#include "common/qtdhelpers.h"
#include "user/requests/qtdgetuserrequest.h"
#include "chat/requests/qtdgetchatrequest.h"
#include "chat/qtdchat.h"
#include "chat/qtdchats.h"
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
        m_displayedName = m_origin->originSummary();
    }
    m_date = json["date"].toInt();
    m_fromChatId = json["forwarded_from_chat_id"].toInt();
    m_fromMessageId = json["forwarded_from_message_id"].toInt();
    emit forwardInfoChanged();
}