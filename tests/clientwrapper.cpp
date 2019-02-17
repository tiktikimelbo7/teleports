#include "clientwrapper.h"
#include <QDebug>

ClientWrapper::ClientWrapper(QObject *parent) : QTdClient(parent)
{
}

QTdAuthState *ClientWrapper::authState() const
{
}

QTdConnectionState *ClientWrapper::connectionState() const
{
}

void ClientWrapper::send(QTdRequest *obj)
{
}

void ClientWrapper::send(const QJsonObject &json)
{
}

QFuture<QTdResponse> ClientWrapper::sendAsync(QTdRequest *obj, void (QTdClient::*signal)(QJsonObject))
{
}

QFuture<QJsonObject> ClientWrapper::exec(QTdRequest *obj)
{
}

QFuture<QJsonObject> ClientWrapper::exec(const QJsonObject &json)
{
}

void ClientWrapper::init()
{
    qDebug() << "Initializing client wrapper";
    initSignalMap();
}
