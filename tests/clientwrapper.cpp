#include "clientwrapper.h"
#include <QJsonObject>
#include <QtConcurrent>
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
    send(obj->marshalJson());
}

void ClientWrapper::send(const QJsonObject &json)
{
    const QString type = json["@type"].toString();

    if (m_sendmap.contains(type)) {
        m_sendmap.value(type)(json);
        return;
    }
    qWarning() << "[TESTING] Unhandled send type:" << type;
    qWarning() << "Unhandled req: " << json;
}

QFuture<QTdResponse> ClientWrapper::sendAsync(QTdRequest *obj, void (QTdClient::*signal)(QJsonObject))
{
    QJsonObject data = obj->marshalJson();
    const QString tag = getTag();
    data["@extra"] = tag;
    QFuture<QTdResponse> f = QtConcurrent::run([=](void(QTdClient::*s)(QJsonObject), const QJsonObject &d, const QString &t)->QTdResponse{
        QEventLoop loop;

        QTdResponse result;
        auto respSlot = [&](QJsonObject resp){
            if (resp.contains("@extra")) {
                QString extra = resp.value("@extra").toString();
                if (extra == t) {
                    result.setJson(resp);
                    loop.quit();
                }
            }
        };

        QMetaObject::Connection con1 = QObject::connect(QTdClient::instance(), s, respSlot);
        QMetaObject::Connection con2 = QObject::connect(QTdClient::instance(), &QTdClient::error, respSlot);

        /**
         * We send the data from within the thread due to how QtConcurrent allocates
         * one thread per cpu core. Which we don't want to start up the response loop and it
         * take up the last remaining thread and then no request is actually sent.
         *
         * This ensures that if we are either in the last available thread or waiting
         * for the next available thread the request is always sent "after" setting up the response
         * slots.
         */
        send(d);

        loop.exec();
        disconnect(con1);
        disconnect(con2);
        return result;
    }, signal, data, tag);
    return f;
}

QFuture<QJsonObject> ClientWrapper::exec(QTdRequest *obj)
{
}

QFuture<QJsonObject> ClientWrapper::exec(const QJsonObject &json)
{
}

void ClientWrapper::init()
{
    qDebug() << "Initializing test client wrapper";
    initSignalMap();
    initSendMap();
}

void ClientWrapper::initSendMap()
{
    m_sendmap.insert(QStringLiteral("getUser"), [=](const QJsonObject &req) {
        const QJsonObject user = m_utils.getUserJson(req["user_id"].toString());
        if (!user.isEmpty()) {
            QJsonObject resp {
                {"@type", "user"},
                {"user", user},
            };
            if (req.contains(QLatin1String("@extra"))) {
                resp[QLatin1String("@extra")] = req[QLatin1String("@extra")];
            }
            handleRecv(resp);
        }
    });
}
