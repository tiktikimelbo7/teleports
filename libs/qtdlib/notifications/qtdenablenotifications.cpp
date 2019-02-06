#include "qtdenablenotifications.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include "client/qtdclient.h"

QTdEnableNotifications::QTdEnableNotifications(QObject *parent) : QTdRequest(parent)
{
}

Q_INVOKABLE void QTdEnableNotifications::enable(QString token)
{
    m_token = token;
    QTdClient::instance()->send(this);
}

QJsonObject QTdEnableNotifications::marshalJson()
{

    return QJsonObject{
        {"@type", "registerDevice"},
        {"device_token", QJsonObject{
            {"@type", "deviceTokenUbuntuPush"},
            {"token", m_token}
        }}
    };

}
