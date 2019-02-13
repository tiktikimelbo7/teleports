#ifndef QTDCLIENT_H
#define QTDCLIENT_H

#include <functional>
#include <QObject>
#include <QPointer>
#include <QSharedPointer>
#include <QScopedPointer>
#include <QHash>
#include <QFuture>
#include <QThread>
#include <QJsonObject>
#include <QtConcurrent>
#include "auth/qtdauthstate.h"
#include "connections/qtdconnectionstate.h"
#include "common/qtdrequest.h"
#include "common/qtdresponse.h"

// callback to trigger on received messages from tdlib.
typedef std::function<void(QJsonObject)> ReceiveCallback;

/**
 * @brief The QTdClient class
 *
 * QTdClient is the main interface for communication with
 * the tdlib backend. A static instance is used to easily share
 * between components.
 */
class QTdClient : public QObject
{
    Q_OBJECT
public:
    explicit QTdClient(QObject *parent = nullptr);

    /**
     * @brief Shared static instance in the main thread.
     */
    static QTdClient *instance();

    /**
     * @brief setInstance
     * @param instance to set as the global instance
     * 
     * Only really useful for testing when you don't want
     * the default instance.
     */
    static void setInstance(QTdClient *instance);

    /**
     * @brief The current authentication state
     */
    virtual QTdAuthState *authState() const;
    /**
     * @brief The current connection state
     */
    virtual QTdConnectionState *connectionState() const;

    /**
     * @brief send
     *
     * Send a request in another thread.
     */
    virtual void send(QTdRequest *obj);
    virtual void send(const QJsonObject &json);

    /**
     * @brief sendAsync
     * @param obj is the QTdRequest to send
     * @param void(QTdClient::*s)(QJsonObject) is the signal the response will come down
     * @return QFuture<QTdResponse> - QTdResponse will either contain an `error` or the requested return types json object
     *
     * Send an asynchronous request that can be waited on for a response.
     *
     * @example
     * QScopedPointer<QTdGetChatsRequest> req(new QTdGetChatsRequest);
     *
     * The response to getChats comes down the `chats` signal
     * QFuture<QTdResponse> resp = QTdClient::instance()->sendAsync(req, &QTdClient::chats);
     *
     * You can either use a QFutureWatcher to wait for the resp or you can use the new
     * `await` helper `#include "utils/await.h"` which uses QFutureWatcher internally but also allows a timeout
     * so you don't wait forever.
     * await(resp);
     *
     * if (resp.result().isError()) {
     *   // handle error
     *   qWarning() << resp.result().errorCode() << resp.result().errorString();
     * }
     * // access json
     * qDebug() << resp.result().json()
     *
     */
    virtual QFuture<QTdResponse> sendAsync(QTdRequest *obj, void(QTdClient::*signal)(QJsonObject));

    /**
     * @brief Execute synchronous request in another Thread
     *
     * This is a synchronous action in another
     * thread and will return the result via the
     * returned QFuture
     *
     * Note: there is only a limited set of actions that can be sent
     * through exec. Still need to figure out what they are. Once
     * found it will be indicated in each QTdRequest derived class
     * documentation
     */
    virtual QFuture<QJsonObject> exec(QTdRequest *obj);
    virtual QFuture<QJsonObject> exec(const QJsonObject &json);

    /**
     * @brief Retrieve a Telegram server option by name
     *
     * Retrieves an option that has been received from the Telegram server upon login or after an update
     * Clients should subscribe for the updateOption signal and then call this method with the appropriate name
     * Returns an empty QVariant if the option is not found.
     */
    QVariant getOption(const QString name);

signals:
    void authStateChanged(QTdAuthState *state);
    void connectionStateChanged(QTdConnectionState *state);
    void updateUser(QJsonObject user);
    void updateUserStatus(const QString &user_id, const QJsonObject &status);
    void updateFile(const QJsonObject &file);
    void updateNewChat(const QJsonObject &chat);
    void updateChatOrder(const QJsonObject &chat);
    void updateChatLastMessage(const QJsonObject &chat);
    void updateMessageSendSucceeded(const QJsonObject &chat);
    void updateMessageContent(const QJsonObject &chat);
    void updateBasicGroup(const QJsonObject &group);
    void updateBasicGroupFullInfo(const QJsonObject &group);
    void updateSecretChat(const QJsonObject &chat);
    void updateSuperGroup(const QJsonObject &group);
    void updateSupergroupFullInfo(const QJsonObject &group);
    void updateChatReadInbox(const QJsonObject &chat);
    void updateChatReadOutbox(const QJsonObject &chat);
    void updateChatIsPinned(const QJsonObject &chat);
    void updateChatPhoto(const QJsonObject &photo);
    void updateChatReplyMarkup(const QJsonObject &chat);
    void updateChatTitle(const QJsonObject &chat);
    void updateChatUnreadMentionCount(const QJsonObject &chat);
    void updateUserChatAction(const QJsonObject &chat);
    void updateChatNotificationSettings(const QJsonObject &chat);
    void updateDeleteMessages(const QJsonObject &messages);
    void updateOption(const QJsonObject &option);
    void updateNewMessage(const QJsonObject &message);

    // Response signals
    void error(QJsonObject error);
    void ok(QJsonObject message);
    void chats(QJsonObject chats);
    void basicGroup(QJsonObject group);
    void secretChat(QJsonObject chat);
    void superGroup(QJsonObject group);
    void supergroupFullInfo(QJsonObject group);
    void messages(QJsonObject messages);
    void message(QJsonObject message);
    void file(QJsonObject file);
    void user(QJsonObject user);

private slots:
    void handleRecv(const QJsonObject &data);
    void handleApplicationStateChanged(Qt::ApplicationState state);

private:
    Q_DISABLE_COPY(QTdClient)
    void init();
    void handleUpdateOption(const QJsonObject &json);
    QScopedPointer<QThread> m_worker;
    QPointer<QTdAuthState> m_authState;
    QPointer<QTdConnectionState> m_connectionState;
    QHash<QString, ReceiveCallback> m_events;
    QHash<QString, QVariant> m_options;

    QString getTag();
    int m_tagcounter;

};

#endif // QTDCLIENT_H
