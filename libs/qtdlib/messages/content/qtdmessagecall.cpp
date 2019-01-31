#include "qtdmessagecall.h"
#include <QDebug>

QTdMessageCall::QTdMessageCall(QObject *parent) : QTdMessageContent(parent),
    m_discardReason(Q_NULLPTR), m_duration(0)
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

void QTdMessageCall::unmarshalJson(const QJsonObject &json)
{
    m_duration = json["duration"].toInt();
    const QJsonObject reasonObj = json["discard_reason"].toObject();
    const QString reason = reasonObj["@type"].toString();
    if (reason == QStringLiteral("callDiscardReasonDeclined")) {
        m_discardReason = new QTdCallDiscardReasonDeclined(this);
    } else if (reason == QStringLiteral("callDiscardReasonDisconnected")) {
        m_discardReason = new QTdCallDiscardReasonDisconnected(this);
    } else if (reason == QStringLiteral("callDiscardReasonEmpty")) {
        m_discardReason = new QTdCallDiscardReasonEmpty(this);
    } else if (reason == QStringLiteral("callDiscardReasonHungUp")) {
        m_discardReason = new QTdCallDiscardReasonHungUp(this);
    } else if (reason == QStringLiteral("callDiscardReasonMissed")) {
        m_discardReason = new QTdCallDiscardReasonMissed(this);
    } else {
        qWarning() << "Unknown call discard reason";
    }
    QTdMessageContent::unmarshalJson(json);
    emit contentChanged();
}
