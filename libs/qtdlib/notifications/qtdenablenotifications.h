#ifndef QTDENABLENOTIFICATIONS_H
#define QTDENABLENOTIFICATIONS_H

#include <QObject>
#include "common/qtdrequest.h"
#include "common/qabstracttdobject.h"

class QTdEnableNotifications : public QTdRequest
{
    Q_OBJECT

public:
    explicit QTdEnableNotifications(QObject *parent = nullptr);


    Q_INVOKABLE void enable(QString token);
    QJsonObject marshalJson();

signals:
    void enabled();

private:
    Q_DISABLE_COPY(QTdEnableNotifications)
    QString m_token;
};

#endif // QTDENABLENOTIFICATIONS_H
