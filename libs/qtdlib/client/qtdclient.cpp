#include "qtdclient.h"
#include <QDebug>
#include <QPointer>
#include <QEventLoop>
#include <QMetaObject>
#include <QJsonDocument>
#include <QtConcurrent>
#include <QtGui/QGuiApplication>
#include "qtdthread.h"
#include "qtdhandle.h"
#include "auth/qtdauthstatefactory.h"
#include "connections/qtdconnectionstatefactory.h"
#include <QJsonDocument>


QJsonObject execTd(const QJsonObject &json) {
    qDebug() << "[EXEC]" << json;
    const QByteArray  tmp = (QJsonDocument(json).toJson(QJsonDocument::Compact) % '\0');
    QSharedPointer<Handle> tdlib = QTdHandle::instance();
    const QByteArray  str = QByteArray(td_json_client_execute(tdlib->handle(), tmp.constData()));
    const QJsonObject ret = QJsonDocument::fromJson(str).object();
    qDebug() << "[EXEC RESULT]" << ret;
    return ret;
}

void sendTd(const QJsonObject &json) {
    qDebug() << "[SEND] :" << json;
    const QByteArray msg = QJsonDocument(json).toJson(QJsonDocument::Compact).append('\0');
    QSharedPointer<Handle> tdlib = QTdHandle::instance();
    td_json_client_send(tdlib->handle(), msg.constData());
}

QTdClient::QTdClient(QObject *parent) : QObject(parent),
    m_worker(Q_NULLPTR),
    m_authState(Q_NULLPTR),
    m_connectionState(Q_NULLPTR),
    m_tagcounter(0)
{
}

static QPointer<QTdClient> s_tdclient;
QTdClient *QTdClient::instance()
{
    if (s_tdclient.isNull()) {
        qDebug() << "Creating new tdclient instance";
        s_tdclient = new QTdClient();
        s_tdclient->init();
    }
    return s_tdclient;
}

void QTdClient::setInstance(QTdClient *instance)
{
    s_tdclient = instance;
}

QTdAuthState *QTdClient::authState() const
{
    return m_authState;
}

QTdConnectionState *QTdClient::connectionState() const
{
    return m_connectionState;
}

void QTdClient::send(QTdRequest *obj)
{
    send(obj->marshalJson());
}

void QTdClient::send(const QJsonObject &json)
{
    if (json.isEmpty()) {
        qDebug() << "Empty Json object, nothing to send?";
        return;
    }
    QtConcurrent::run(sendTd, json);
}

QFuture<QTdResponse> QTdClient::sendAsync(QTdRequest *obj, void (QTdClient::*signal)(QJsonObject)) {
    QJsonObject data = obj->marshalJson();
    const QString tag = getTag();
    data["@extra"] = tag;
    QFuture<QTdResponse> f = QtConcurrent::run([](void(QTdClient::*s)(QJsonObject), const QJsonObject &data, const QString &tag)->QTdResponse{
        // TODO: Should we wrap this up in a QRunnable instead of using an event loop
        QEventLoop loop;

        QTdResponse result;
        auto respSlot = [&](QJsonObject resp){
            if (resp.contains("@extra")) {
                QString extra = resp.value("@extra").toString();
                if (extra == tag) {
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
        sendTd(data);

        loop.exec();
        disconnect(con1);
        disconnect(con2);
        return result;
    }, signal, data, tag);
    return f;
}

QFuture<QJsonObject> QTdClient::exec(QTdRequest *obj)
{
    return exec(obj->marshalJson());
}

QFuture<QJsonObject> QTdClient::exec(const QJsonObject &json)
{
    return QtConcurrent::run(execTd, json);
}

void QTdClient::handleRecv(const QJsonObject &data)
{
    static bool DEBUG_TDLIB = true;
    if (!DEBUG_TDLIB) {
        DEBUG_TDLIB = qgetenv("TDLIB_DEBUG") == QByteArrayLiteral("1");
    }
    const QString type = data["@type"].toString();

    if (DEBUG_TDLIB) {
        qDebug() << "-------------[ RCV ]-----------------------";
        qDebug() << "TYPE >> " << type;
        qDebug() << "DATA >> " << data;
        qDebug() << "-------------------------------------------";
    }

    if (m_events.contains(type)) {
        m_events.value(type)(data);
        return;
    }
    if (DEBUG_TDLIB) {
        qDebug() << "---------[UNHANDLED]-------------";
        qDebug() << type;
        qDebug() << "---------------------------------";
    }
}

void QTdClient::initSignalMap()
{
    m_events.insert(QStringLiteral("updateAuthorizationState"), [=](const QJsonObject &data) {
        QTdAuthState *state = QTdAuthStateFactory::create(data, this);
        if (!state) {
            qDebug() << "Unknown auth state: " << data;
            return;
        }
        if (!m_authState || (state->type() != m_authState->type())) {
            if (m_authState) {
                delete m_authState;
                m_authState = 0;
            }
            m_authState = state;
            emit authStateChanged(m_authState);
        }
    });

    m_events.insert(QStringLiteral("updateConnectionState"), [=](const QJsonObject &data) {
        qDebug() << "[ConnectionStateChanged] >> " << data;
        if (m_connectionState) {
            delete m_connectionState;
            m_connectionState = 0;
        }
        m_connectionState = QTdConnectionStateFactory::create(data, this);
        emit connectionStateChanged(m_connectionState);
    });

    m_events.insert(QStringLiteral("updateUser"), [=](const QJsonObject &data){
        emit updateUser(data["user"].toObject());
    });

    m_events.insert(QStringLiteral("updateUserStatus"), [=](const QJsonObject &data){
        const QString userId = QString::number(qint32(data["user_id"].toInt()));
        emit updateUserStatus(userId, data["status"].toObject());
    });

    m_events.insert(QStringLiteral("updateFile"), [=](const QJsonObject &data){
        emit updateFile(data["file"].toObject());
    });

    m_events.insert(QStringLiteral("updateNewChat"), [=](const QJsonObject &data){
        emit updateNewChat(data["chat"].toObject());
    });

    m_events.insert(QStringLiteral("updateBasicGroup"), [=](const QJsonObject &data){
        emit updateBasicGroup(data["basic_group"].toObject());
    });

    m_events.insert(QStringLiteral("updateBasicGroupFullInfo"), [=](const QJsonObject &data){
        emit updateBasicGroupFullInfo(data);
    });
    m_events.insert(QStringLiteral("basicGroup"), [=](const QJsonObject &data){ emit updateBasicGroup(data); });
    m_events.insert(QStringLiteral("secretChat"), [=](const QJsonObject &data){ emit secretChat(data); });
    m_events.insert(QStringLiteral("updateSecretChat"), [=](const QJsonObject &data){ emit updateSecretChat(data["secret_chat"].toObject()); });
    m_events.insert(QStringLiteral("supergroup"), [=](const QJsonObject &data){ emit superGroup(data); });
    m_events.insert(QStringLiteral("updateSupergroupFullInfo"), [=](const QJsonObject &data){ emit updateSupergroupFullInfo(data); });
    m_events.insert(QStringLiteral("supergroupFullInfo"), [=](const QJsonObject &data){ emit supergroupFullInfo(data); });
    m_events.insert(QStringLiteral("updateSupergroup"), [=](const QJsonObject &data){ emit updateSuperGroup(data["supergroup"].toObject()); });
    m_events.insert(QStringLiteral("updateChatOrder"), [=](const QJsonObject &data){ emit updateChatOrder(data); });
    m_events.insert(QStringLiteral("updateChatLastMessage"), [=](const QJsonObject &data){ emit updateChatLastMessage(data); });
    m_events.insert(QStringLiteral("updateMessageContent"), [=](const QJsonObject &data){ emit updateMessageContent(data); });
    m_events.insert(QStringLiteral("updateMessageSendSucceeded"), [=](const QJsonObject &data){ emit updateMessageSendSucceeded(data); });
    m_events.insert(QStringLiteral("updateChatReadInbox"), [=](const QJsonObject &data){ emit updateChatReadInbox(data); });
    m_events.insert(QStringLiteral("updateChatIsPinned"), [=](const QJsonObject &data){ emit updateChatIsPinned(data); });
    m_events.insert(QStringLiteral("updateChatPhoto"), [=](const QJsonObject &data){ emit updateChatPhoto(data); });
    m_events.insert(QStringLiteral("updateChatReadOutbox"), [=](const QJsonObject &data){ emit updateChatReadOutbox(data); });
    m_events.insert(QStringLiteral("updateChatReplyMarkup"), [=](const QJsonObject &data){ emit updateChatReplyMarkup(data); });
    m_events.insert(QStringLiteral("updateChatTitle"), [=](const QJsonObject &data){ emit updateChatTitle(data); });
    m_events.insert(QStringLiteral("updateChatUnreadMentionCount"), [=](const QJsonObject &data){ emit updateChatUnreadMentionCount(data); });
    m_events.insert(QStringLiteral("updateUserChatAction"), [=](const QJsonObject &data){ emit updateUserChatAction(data); });
    m_events.insert(QStringLiteral("updateChatNotificationSettings"), [=](const QJsonObject &data){ emit updateChatNotificationSettings(data); });

    m_events.insert(QStringLiteral("messages"), [=](const QJsonObject &data){ emit messages(data); });
    m_events.insert(QStringLiteral("message"), [=](const QJsonObject &data){ emit message(data); });

    //Option handling - more or less global constants, still could change during execution
    m_events.insert(QStringLiteral("updateOption"), [=](const QJsonObject &data){ emit updateOption(data); });

    //Message updates to add to existing chats or channel views
    m_events.insert(QStringLiteral("updateNewMessage"), [=](const QJsonObject &data){ emit updateNewMessage(data); });
    m_events.insert(QStringLiteral("chats"), [=](const QJsonObject &data){ emit chats(data); });
    m_events.insert(QStringLiteral("error"), [=](const QJsonObject &data){ emit error(data); });
    m_events.insert(QStringLiteral("ok"), [=](const QJsonObject &data){ emit ok(data); });
    m_events.insert(QStringLiteral("basicGroup"), [=](const QJsonObject &group){ emit basicGroup(group); });
    m_events.insert(QStringLiteral("file"), [=](const QJsonObject &data){
        emit updateFile(data);
        emit file(data);
    });
    m_events.insert(QStringLiteral("user"), [=](const QJsonObject &data){
        emit updateUser(data);
        emit user(data);
    });
}

void QTdClient::initWorker()
{
    qDebug() << "Initializing tdlib worker";
    m_worker = new QThread();
    QTdWorker *w = new QTdWorker;
    w->moveToThread(m_worker);
    connect(m_worker, &QThread::started, w, &QTdWorker::run);
    connect(w, &QTdWorker::recv, this, &QTdClient::handleRecv);
    connect(this, &QTdClient::updateOption, this, &QTdClient::handleUpdateOption);
    connect(qGuiApp, &QGuiApplication::applicationStateChanged, this,
            &QTdClient::handleApplicationStateChanged);
    m_worker->start();
}

void QTdClient::handleUpdateOption(const QJsonObject &json)
{

    QJsonDocument doc(json);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    QString option_name = json["name"].toString();
    auto option_value = QVariant();
    auto value_obj = json["value"].toObject();
    auto type = value_obj["@type"].toString();
    if (type == "optionValueString") {
        option_value = value_obj["value"].toString();
    } else if (type == "optionValueInteger") {
        option_value = value_obj["value"].toInt();
    } else if (type == "optionValueBoolean") {
        option_value = value_obj["value"].toBool();
    } else {
        qWarning() << "Unknown option type: " << type;
    }
    m_options[option_name] = option_value;
    qWarning() << "received option" << option_name << ", value" << option_value;
}

QString QTdClient::getTag()
{
    m_tagcounter++;
    return QString("req-%1").arg(QString::number(m_tagcounter));
}

QVariant QTdClient::getOption(const QString name)
{
    if (m_options.contains(name))
    {
        return m_options[name];
    }
    else
        return QVariant();
}

void QTdClient::init()
{
    initSignalMap();
    initWorker();
}

void QTdClient::handleApplicationStateChanged(Qt::ApplicationState state)
{
    switch(state) {
        case Qt::ApplicationState::ApplicationSuspended:
            qWarning() << "Application has been suspended!";
            send(QJsonObject{
                {"@type", "setNetworkType"},
                {"type", QJsonObject{{"@type", "networkTypeNone"}}}});
            break;
        case Qt::ApplicationState::ApplicationActive:
            qWarning() << "Application has been activated!";
            send(QJsonObject{
                {"@type", "setNetworkType"},
                {"type", QJsonObject{{"@type", "networkTypeMobile"}}}});
            break;
    }

}
