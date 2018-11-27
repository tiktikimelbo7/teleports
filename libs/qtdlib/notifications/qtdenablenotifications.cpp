#include "qtdenablenotifications.h"
#include <QDebug>
#include <QJsonArray>
#include "client/qtdclient.h"

QTdEnableNotifications::QTdEnableNotifications(QObject *parent) : QTdObject(parent)
{
}

void QTdEnableNotifications::enable(QString token)
{
    QTdClient::instance()->send(QJsonObject{
                                    {"@type", "deviceTokenUbuntuPush"},
                                    {"token", token}
                                    });
}
