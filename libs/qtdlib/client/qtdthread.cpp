#include "qtdthread.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QDebug>
#include <QCoreApplication>

QTdWorker::QTdWorker(QObject *parent) : QObject(parent),
    m_tdlibProxy(QTdProxyProvider::instance("direct"))
{
}

QTdWorker::~QTdWorker()
{
    m_tdlibProxy.clear();
}

void QTdWorker::run()
{
    // enter the tdlib event loop
    forever {
        const QByteArray rcv = QByteArray(m_tdlibProxy->receive(1));
        if (!rcv.isEmpty()) {
            const QJsonObject json = QJsonDocument::fromJson(rcv).object();
            if (!json.isEmpty()) {
                emit recv(json);
                // if this is an authorizationStateClosed we should exit the event loop and destroy
                // the client.
                if (json["@type"] == "updateAuthorizationState" ) {
                    const QJsonObject state = json["authorization_state"].toObject();
                    if (state["type"] == "authorizationStateClosed") {
                        qDebug() << "Closing connection";
                        break;
                    }
                }
            }
        }
    }
}
