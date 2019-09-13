#ifndef QTDMESSAGECALL_H
#define QTDMESSAGECALL_H

#include <QObject>
#include <QPointer>
#include "messages/qtdmessagecontent.h"
#include "call/qtdcalldiscardreason.h"

/**
 * @brief The QTdMessageCall class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1message_call.html
 */
class QTdMessageCall : public QTdMessageContent
{
    Q_OBJECT
    Q_PROPERTY(QTdCallDiscardReason *discardReason READ discardReason NOTIFY contentChanged)
    Q_PROPERTY(qint32 duration READ duration NOTIFY contentChanged)
public:
    explicit QTdMessageCall(QObject *parent = nullptr);

    QTdCallDiscardReason *discardReason() const;

    qint32 duration() const;

    void unmarshalJson(const QJsonObject &json);

signals:
    void contentChanged();

private:
    QPointer<QTdCallDiscardReason> m_discardReason;
    qint32 m_duration;
};

#endif // QTDMESSAGECALL_H
