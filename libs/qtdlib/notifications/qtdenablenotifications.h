#ifndef QTDENABLENOTIFICATIONS_H
#define QTDENABLENOTIFICATIONS_H

#include <QObject>
#include <QFutureWatcher>
#include "auth/qtdauthstate.h"
#include "common/qtdrequest.h"
#include "common/qabstracttdobject.h"

class QTdEnableNotifications : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged)
public:
    explicit QTdEnableNotifications(QObject *parent = nullptr);

    QString token() const;

    void setToken(const QString &token);

signals:
    void enabled();
    void error(QString error);
    void tokenChanged();

private slots:
    void registerDevice();
    void handleResponse();

private:
    Q_DISABLE_COPY(QTdEnableNotifications)
    QString m_token;
    QFutureWatcher<QTdResponse> m_watcher;
};



#endif // QTDENABLENOTIFICATIONS_H
