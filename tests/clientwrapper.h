#ifndef CLIENTWRAPPER_H
#define CLIENTWRAPPER_H

#include <QObject>
#include "client/qtdclient.h"
#include "testutils.h"

class ClientWrapper : public QTdClient
{
    Q_OBJECT
public:
    explicit ClientWrapper(QObject *parent = nullptr);

    // QTdClient interface
public:
    QTdAuthState *authState() const override;
    QTdConnectionState *connectionState() const override;
    void send(QTdRequest *obj) override;
    void send(const QJsonObject &json) override;
    QFuture<QTdResponse> sendAsync(QTdRequest *obj, void (QTdClient::*signal)(QJsonObject)) override;
    QFuture<QJsonObject> exec(QTdRequest *obj) override;
    QFuture<QJsonObject> exec(const QJsonObject &json) override;
    void init() override;

protected:
    void initSendMap();
private:
    QHash<QString, ReceiveCallback> m_sendmap;
    TestUtils m_utils;
};

#endif // CLIENTWRAPPER_H
