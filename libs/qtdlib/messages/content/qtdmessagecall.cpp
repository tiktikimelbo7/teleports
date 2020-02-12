#include "qtdmessagecall.h"
#include "utils/i18n.h"
#include <QDebug>
#include <QtMath>

QTdMessageCall::QTdMessageCall(QObject *parent)
    : QTdMessageContent(parent)
    , m_discardReason(Q_NULLPTR)
    , m_duration(0)
    , m_minutes(0)
    , m_seconds(0)
{
    setType(MESSAGE_CALL);
}

QTdCallDiscardReason *QTdMessageCall::discardReason() const
{
    return m_discardReason;
}

qint32 QTdMessageCall::duration() const
{
    return m_duration;
}

QString QTdMessageCall::minutes() const
{
    return (m_minutes < 10 ? "0" : "") + QString::number(m_minutes);
}

QString QTdMessageCall::seconds() const
{
    return (m_seconds < 10 ? "0" : "") + QString::number(m_seconds);
}

QString QTdMessageCall::typeText() const
{
    return m_typeText;
}

void QTdMessageCall::setOutgoing(const bool isOutgoing)
{
    m_isOutgoing = isOutgoing;
}

void QTdMessageCall::unmarshalJson(const QJsonObject &json)
{
    m_duration = json["duration"].toInt();
    m_minutes = qFloor(m_duration / 60);
    m_seconds = m_duration - (m_minutes * 60);
    const QJsonObject reasonObj = json["discard_reason"].toObject();
    const QString reason = reasonObj["@type"].toString();
    if (reason == QStringLiteral("callDiscardReasonDeclined")) {
        m_typeText = gettext("Call Declined");
        m_discardReason = new QTdCallDiscardReasonDeclined(this);
    } else if (reason == QStringLiteral("callDiscardReasonDisconnected")) {
        m_typeText = gettext("Call Disconnected");
        m_discardReason = new QTdCallDiscardReasonDisconnected(this);
    } else if (reason == QStringLiteral("callDiscardReasonEmpty")) {
        m_typeText = gettext("Call Ended");
        m_discardReason = new QTdCallDiscardReasonEmpty(this);
    } else if (reason == QStringLiteral("callDiscardReasonHungUp")) {
        m_typeText = QString(gettext("%1 (%2:%3)")).arg(m_isOutgoing ? gettext("Outgoing Call") : gettext("Incoming Call"), minutes(), seconds());
        m_discardReason = new QTdCallDiscardReasonHungUp(this);
    } else if (reason == QStringLiteral("callDiscardReasonMissed")) {
        m_typeText = m_isOutgoing ? gettext("Cancelled Call") : gettext("Missed Call");
        m_discardReason = new QTdCallDiscardReasonMissed(this);
    } else {
        m_typeText = gettext("Call");
        qWarning() << "Unknown call discard reason";
    }
    QTdMessageContent::unmarshalJson(json);
    emit contentChanged();
}
