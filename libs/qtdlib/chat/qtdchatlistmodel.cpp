#include "qtdchatlistmodel.h"
#include <QScopedPointer>
#include "client/qtdclient.h"
#include "chat/requests/qtdgetchatsrequest.h"
#include "chat/requests/qtdgetchatrequest.h"
#include "chat/requests/qtdsetpinnedchatsrequest.h"
#include "chat/requests/qtdleavechatrequest.h"
#include "chat/requests/qtdforwardmessagesrequest.h"
#include "messages/requests/qtdsendmessagerequest.h"
#include "messages/requests/content/qtdinputmessagetext.h"
#include "common/qtdhelpers.h"

#include "chat/qtdchattypefactory.h"

QTdChatListModel::QTdChatListModel(QObject *parent)
    : QObject(parent)
    , m_model(Q_NULLPTR)
    , m_currentChat(Q_NULLPTR)
    , m_forwardedFromChat(Q_NULLPTR)
    , m_forwardingMessages(QStringList())
    , m_listMode(ListMode::Idle)
{
    m_model = new QQmlObjectListModel<QTdChat>(this, "", "id");
    connect(QTdClient::instance(), &QTdClient::chats, this, &QTdChatListModel::handleChats);
    connect(QTdClient::instance(), &QTdClient::chat, this, &QTdChatListModel::handleChat);
    connect(QTdClient::instance(), &QTdClient::updateNewChat, this, &QTdChatListModel::handleUpdateNewChat);
    connect(QTdClient::instance(), &QTdClient::authStateChanged, this, &QTdChatListModel::handleAuthStateChanges);
    connect(QTdClient::instance(), &QTdClient::updateChatOrder, this, &QTdChatListModel::handleUpdateChatOrder);
    connect(QTdClient::instance(), &QTdClient::updateChatLastMessage, this, &QTdChatListModel::handleUpdateChatLastMessage);
    connect(QTdClient::instance(), &QTdClient::updateChatReadInbox, this, &QTdChatListModel::updateChatReadInbox);
    connect(QTdClient::instance(), &QTdClient::updateChatReadOutbox, this, &QTdChatListModel::updateChatReadOutbox);
    connect(QTdClient::instance(), &QTdClient::updateChatIsPinned, this, &QTdChatListModel::handleUpdateChatIsPinned);
    connect(QTdClient::instance(), &QTdClient::updateChatPhoto, this, &QTdChatListModel::handleUpdateChatPhoto);
    connect(QTdClient::instance(), &QTdClient::updateChatReplyMarkup, this, &QTdChatListModel::handleUpdateChatReplyMarkup);
    connect(QTdClient::instance(), &QTdClient::updateChatTitle, this, &QTdChatListModel::handleUpdateChatTitle);
    connect(QTdClient::instance(), &QTdClient::updateChatUnreadMentionCount, this, &QTdChatListModel::handleUpdateChatUnreadMentionCount);
    connect(QTdClient::instance(), &QTdClient::updateChatNotificationSettings, this, &QTdChatListModel::handleUpdateChatNotificationSettings);
}

QObject *QTdChatListModel::model() const
{
    return m_model;
}

QTdChat *QTdChatListModel::currentChat() const
{
    return m_currentChat;
}

QTdChat *QTdChatListModel::chatById(const qint64 &chatId) const
{
    return m_model->getByUid(QString::number(chatId));
}

void QTdChatListModel::setCurrentChatById(const int &chatId)
{
    QTdChat *currentChat = chatById(chatId);
    setCurrentChat(currentChat);
}

qint32 QTdChatListModel::forwardingMessagesCount() const
{
    return m_forwardingMessages.length();
}

void QTdChatListModel::setCurrentChat(QTdChat *currentChat)
{
    if (m_currentChat == currentChat)
        return;
    m_currentChat = currentChat;
    emit currentChatChanged(m_currentChat);
}

QTdChat *QTdChatListModel::forwardedFromChat() const
{
    return m_forwardedFromChat;
}

void QTdChatListModel::setForwardedFromChat(QTdChat *forwardedFromChat)
{
    if (m_forwardedFromChat == forwardedFromChat)
        return;
    m_forwardedFromChat = forwardedFromChat;
    // emit forwardedFromChatChanged(m_forwardedFromChat);
}

QStringList QTdChatListModel::forwardingMessages() const
{
    return m_forwardingMessages;
}

void QTdChatListModel::setForwardingMessages(QStringList forwardingMessages)
{
    if (m_forwardingMessages == forwardingMessages)
        return;
    m_forwardingMessages = forwardingMessages;
}

void QTdChatListModel::clearCurrentChat()
{
    m_currentChat = Q_NULLPTR;
    emit currentChatChanged(m_currentChat);
}

void QTdChatListModel::handleChat(const QJsonObject &json) {
    QScopedPointer<QTdChat> chat(new QTdChat);
    chat->unmarshalJson(json);
    if (chat->isMyself()) {
        qWarning() << "Chat with myself. Id:" << chat->id() << "Order:" << chat->order();
    }
    handleUpdateNewChat(json);
}

void QTdChatListModel::handleChats(const QJsonObject &json)
{
    QJsonArray chats = json["chat_ids"].toArray();
    if (chats.count() == 0) {
        if (m_receivedChatIds.count()>0) {
            QScopedPointer<QTdGetChatRequest> chatReq(new QTdGetChatRequest);
            foreach (qint64 chatToRequest, m_receivedChatIds) {
                qWarning() << "Requesting chat" << chatToRequest;
                chatReq->setChatId(chatToRequest);
                chatReq->sendAsync();
            }
        }
        return;
    }
    qWarning() << "Handling getChats result:";
    qWarning() << "Received" << chats.count() << "chats.";
    foreach (QJsonValue chatToRequest, chats) {
        m_receivedChatIds.append((qint64) chatToRequest.toDouble());
    }
    auto lastChat = m_model->getByUid(QString::number(m_receivedChatIds.last()));
    if (!lastChat) {
        qWarning() << "Last chat not yet loaded, cannot fetch more chats";
        return;
    }
    QScopedPointer<QTdGetChatsRequest> req(new QTdGetChatsRequest);
    req->setOffsetChatId(lastChat->id());
    req->setOffsetOrder(lastChat->order());
    req->sendAsync();
}
void QTdChatListModel::handleUpdateNewChat(const QJsonObject &chat)
{
    const qint64 id = qint64(chat["id"].toDouble());
    // Need to remember the model actually indexes on the qmlId variant which is a QString
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->unmarshalJson(chat);
    } else {
        tdchat = QTdChatFactory::createChat(chat["type"].toObject());
        tdchat->unmarshalJson(chat);
        m_model->append(tdchat);
        connect(tdchat, &QTdChat::chatStatusChanged, this, &QTdChatListModel::chatStatusChanged);
        // We also need to update the internal pinned chats list now
        // otherwise any pinned chats will get removed when QTdChat::pinChat/unpinChat() is called
        connect(tdchat, &QTdChat::pinChatAction, this, &QTdChatListModel::handlePinChatAction);
        connect(tdchat, &QTdChat::forwardingMessagesAction, this, &QTdChatListModel::handleForwardingMessagesAction);
        if (tdchat->isPinned()) {
            m_pinnedChats << tdchat->id();
        }
    }
    emit contentsChanged();
}

void QTdChatListModel::handleUpdateChatOrder(const QJsonObject &json)
{
    const qint64 id = qint64(json["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatOrder(json);
        emit contentsChanged();
    }
}

void QTdChatListModel::handleUpdateChatLastMessage(const QJsonObject chat)
{
    const qint64 id = qint64(chat["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateLastMessage(chat);
        tdchat->updateChatOrder(chat);
        emit contentsChanged();
    }
}

void QTdChatListModel::handleAuthStateChanges(const QTdAuthState *state)
{
    switch (state->type()) {
    case QTdAuthState::Type::AUTHORIZATION_STATE_READY: {
        QTdClient::instance()->send(QJsonObject{ { "@type", "clearRecentlyFoundChats" } });
        qWarning() << "Requesting initially 50 chats";
        m_receivedChatIds.clear();
        QScopedPointer<QTdGetChatsRequest> req(new QTdGetChatsRequest);
        req->sendAsync();
        break;
    }
    case QTdAuthState::Type::AUTHORIZATION_STATE_CLOSED: {
        m_model->clear();
        break;
    }
    default:
        return;
    }
}

void QTdChatListModel::updateChatReadInbox(const QJsonObject &json)
{
    const qint64 id = qint64(json["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatReadInbox(json);
        emit contentsChanged();
    }
}

void QTdChatListModel::updateChatReadOutbox(const QJsonObject &json)
{
    const qint64 id = qint64(json["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatReadOutbox(json);
        emit contentsChanged();
    }
}

void QTdChatListModel::handleUpdateChatIsPinned(const QJsonObject &json)
{
    const qint64 id = qint64(json["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatIsPinned(json);
        emit contentsChanged();
        // Update our internal PinnedChats list
        if (tdchat->isPinned() && !m_pinnedChats.contains(tdchat->id())) {
            m_pinnedChats << tdchat->id();
        } else if (!tdchat->isPinned() && m_pinnedChats.contains(tdchat->id())) {
            m_pinnedChats.removeAll(tdchat->id());
        }
    }
}

void QTdChatListModel::handleUpdateChatPhoto(const QJsonObject &chat)
{
    const qint64 id = qint64(chat["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatPhoto(chat["photo"].toObject());
        emit contentsChanged();
    }
}

void QTdChatListModel::handleUpdateChatReplyMarkup(const QJsonObject &chat)
{
    const qint64 id = qint64(chat["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatReplyMarkup(chat);
        emit contentsChanged();
    }
}

void QTdChatListModel::handleUpdateChatTitle(const QJsonObject &chat)
{
    const qint64 id = qint64(chat["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatTitle(chat);
        emit contentsChanged();
    }
}

void QTdChatListModel::handleUpdateChatUnreadMentionCount(const QJsonObject &chat)
{
    const qint64 id = qint64(chat["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatUnreadMentionCount(chat);
        emit contentsChanged();
    }
}

void QTdChatListModel::handleUpdateChatNotificationSettings(const QJsonObject &chat)
{
    const qint64 id = qint64(chat["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatNotificationSettings(chat);
        emit contentsChanged();
    }
}

void QTdChatListModel::sendForwardMessage(const QStringList &forwardMessageIds,
                                          const qint64 &recievingChatId,
                                          const qint64 &fromChatId,
                                          const QString &message)
{

    QString plainText;
    QJsonArray formatEntities = QTdHelpers::formatPlainTextMessage(message, plainText);
    QTdInputMessageText *messageText = new QTdInputMessageText();
    messageText->setText(message);
    messageText->setEntities(formatEntities);
    QScopedPointer<QTdForwardMessagesRequest> request(new QTdForwardMessagesRequest);
    request->setChatId(recievingChatId);
    request->setFromChatId(fromChatId);
    QScopedPointer<QTdSendMessageRequest> additionalTextMessagerequest(new QTdSendMessageRequest);
    additionalTextMessagerequest->setChatId(recievingChatId);
    additionalTextMessagerequest->setContent(messageText);
    QList<qint64> forwardingMessageIntIds;
    foreach (QString msgId, forwardMessageIds) {
        forwardingMessageIntIds.append(msgId.toLongLong());
    }
    request->setMessageIds(forwardingMessageIntIds);
    QTdClient::instance()->send(request.data());
    if (message != "") {
        QTdClient::instance()->send(additionalTextMessagerequest.data());
    }
}

void QTdChatListModel::handlePinChatAction(const qint64 &chatId, const bool &pinned)
{
    // Copy the internal list as we will wait for the updateChatIsPinned events
    // to update m_pinnedChats. This prevents us from prematurely updating and
    // having to handle errors when updating the pinned chats list.
    PinnedChats chats = m_pinnedChats;
    if (pinned && !m_pinnedChats.contains(chatId)) {
        chats << chatId;
    } else if (!pinned && m_pinnedChats.contains(chatId)) {
        chats.removeAll(chatId);
    }
    QScopedPointer<QTdSetPinnedChatsRequest> req(new QTdSetPinnedChatsRequest);
    req->setPinnedChats(chats);
    QTdClient::instance()->send(req.data());
}

void QTdChatListModel::handleForwardingMessagesAction()
{
    setListMode(ListMode::ForwardingMessages);
}

QTdChatListModel::ListMode QTdChatListModel::listMode() const
{
    return m_listMode;
}

void QTdChatListModel::setListMode(ListMode listMode)
{
    m_listMode = listMode;
    emit listModeChanged();
}
