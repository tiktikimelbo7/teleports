#ifndef QTDENABLENOTIFICATIONS_H
#define QTDENABLENOTIFICATIONS_H

#include <QObject>
#include "common/qabstracttdobject.h"

class QTdEnableNotifications : public QTdObject
{
    Q_OBJECT
public:
    explicit QTdEnableNotifications(QObject *parent = nullptr);

    void enable(QString token);

signals:
    void enabled();

private:
    Q_DISABLE_COPY(QTdEnableNotifications)
};

#endif // QTDENABLENOTIFICATIONS_H
