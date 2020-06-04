#include "qtdchatlistmodel.h"
#include <QScopedPointer>
#include "client/qtdclient.h"
#include "chat/requests/qtdgetchatsrequest.h"
#include "chat/requests/qtdcreatenewsecretchatrequest.h"
#include "chat/requests/qtdcreateprivatechatrequest.h"
#include "chat/requests/qtdgetchatrequest.h"
#include "chat/requests/qtdsetpinnedchatsrequest.h"
#include "chat/requests/qtdleavechatrequest.h"
#include "chat/requests/qtdforwardmessagesrequest.h"
#include "chat/requests/qtdsetchatdraftrequest.h"
#include "chat/requests/qtdsearchpublicchatrequest.h"
#include "chat/requests/qtdjoinchatrequest.h"
#include "chat/requests/qtdcheckchatinvitelinkrequest.h"
#include "chat/requests/qtdjoinchatbyinvitelinkrequest.h"
#include "messages/requests/qtdsendmessagerequest.h"
#include "messages/requests/content/qtdinputmessagetext.h"
#include "common/qtdhelpers.h"

#include "utils/await.h"
#include "chat/qtdchattypefactory.h"
#include "qtdsecretchat.h"

QTdChatListModel::QTdChatListModel(QObject *parent)
    : QObject(parent)
    , m_model(Q_NULLPTR)
    , m_currentChat(Q_NULLPTR)
    , m_currentChatValid(false)
    , m_forwardedFromChat(Q_NULLPTR)
    , m_forwardingMessages(QStringList())
    , m_listMode(ListMode::Idle)
    , m_positionWaitTimer(new QTimer(this))
    , m_chatToOpenOnUpdate(0)
{
    m_model = new QQmlObjectListModel<QTdChat>(this, "", "id");
    m_positionWaitTimer->setInterval(180000);
    m_positionWaitTimer->setSingleShot(true);
    connect(this->m_positionWaitTimer, &QTimer::timeout, this, &QTdChatListModel::onPositionInfoTimeout);

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
    connect(QTdClient::instance(), &QTdClient::updateChatDraftMessage, this, &QTdChatListModel::handleUpdateChatDraftMessage);
    connect(QTdClient::instance(), &QTdClient::updateChatTitle, this, &QTdChatListModel::handleUpdateChatTitle);
    connect(QTdClient::instance(), &QTdClient::updateChatUnreadMentionCount, this, &QTdChatListModel::handleUpdateChatUnreadMentionCount);
    connect(QTdClient::instance(), &QTdClient::updateChatNotificationSettings, this, &QTdChatListModel::handleUpdateChatNotificationSettings);
    connect(QTdClient::instance(), &QTdClient::updateChatOnlineMemberCount, this, &QTdChatListModel::handleUpdateChatOnlineMemberCount);
    connect(QTdClient::instance(), &QTdClient::updateChatChatList, this, &QTdChatListModel::handleUpdateChatChatList);
}

QObject *QTdChatListModel::model() const
{
    return m_model;
}

QTdChat *QTdChatListModel::currentChat() const
{
    return m_currentChat;
}

bool QTdChatListModel::currentChatValid() const
{
    return m_currentChatValid;
}

QTdChat *QTdChatListModel::chatById(const qint64 &chatId) const
{
    return m_model->getByUid(QString::number(chatId));
}

void QTdChatListModel::createOrOpenSecretChat(const int &userId)
{
    qint64 chatId = 0;
    foreach (QTdChat *chat, m_model->toList()) {
        if (chat->isSecret()) {
            auto c = static_cast<QTdSecretChat *>(chat);
            if (!c->isClosed() && c->userId() == userId) {
                chatId = c->id();
                break;
            }
        }
    }
    if (chatId == 0) {
        QScopedPointer<QTdCreateNewSecretChatRequest> req(new QTdCreateNewSecretChatRequest);
        req->setUserId(userId);
        QFuture<QTdResponse> resp = req->sendAsync();
        await(resp, 2000);
        if (resp.result().isError()) {
            qWarning() << "Error during secret chat creation:" << resp.result().errorString();
            return;
        }
        chatId = (qint64)resp.result().json()["id"].toDouble();
        if (chatId == 0) {
            return;
        }
    }
    setCurrentChatById(chatId);
}

void QTdChatListModel::createOrOpenPrivateChat(const int &userId)
{
    QScopedPointer<QTdCreatePrivateChatRequest> req(new QTdCreatePrivateChatRequest);
    req->setUserId(userId);
    QFuture<QTdResponse> resp = req->sendAsync();
    await(resp, 2000);
    if (resp.result().isError()) {
        qWarning() << "Error during private chat creation:" << resp.result().errorString();
        return;
    }
    qint64 chatId = (qint64)resp.result().json()["id"].toDouble();
    setCurrentChatById(chatId);
}

void QTdChatListModel::createOrOpenSavedMessages() {
    createOrOpenPrivateChat(QTdClient::instance()->getOption("my_id").toInt());
}

void QTdChatListModel::setCurrentChatById(const qint64 &chatId)
{
    QTdChat *currentChat = chatById(chatId);
    setCurrentChat(currentChat);
}

void QTdChatListModel::setCurrentChatByUsername(const QString &username)
{
    qDebug() << "OPENING CHAT" << username;
    QScopedPointer<QTdSearchPublicChatRequest> req(new QTdSearchPublicChatRequest);
    req->setChatUsername(username);
    QFuture<QTdResponse> resp = req->sendAsync();
    await(resp, 2000);
    if (resp.result().isError()) {
        qWarning() << "Error during public chat search:" << resp.result().errorString();
        if (resp.result().errorCode() == 400)
            emit invalidChatUsername(username);
        return;
    }
    qint64 chatId = (qint64)resp.result().json()["id"].toDouble();
    setCurrentChatById(chatId);
}

qint32 QTdChatListModel::forwardingMessagesCount() const
{
    return m_forwardingMessages.length();
}

void QTdChatListModel::setCurrentChat(QTdChat *currentChat)
{
    if (currentChat == nullptr) {
        return;
    }
    if (currentChatValid()) {
        if (m_currentChat == currentChat)
            return;
        else
            m_currentChat->closeChat();
    }

    m_currentChat = currentChat;
    m_currentChatValid = true;
    emit currentChatChanged();
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
    m_currentChatValid = false;
    emit currentChatChanged();
}

void QTdChatListModel::handleChat(const QJsonObject &data)
{
    QScopedPointer<QTdChat> chat(new QTdChat);
    chat->unmarshalJson(data);
    handleUpdateNewChat(data);
}

void QTdChatListModel::handleChats(const QJsonObject &data)
{
    QJsonArray chats = data["chat_ids"].toArray();
    if (chats.count() == 0) {
        if (m_receivedChatIds.count() > 0) {
            QScopedPointer<QTdGetChatRequest> chatReq(new QTdGetChatRequest);
            foreach (qint64 chatToRequest, m_receivedChatIds) {
                chatReq->setChatId(chatToRequest);
                chatReq->sendAsync();
            }
        }
        return;
    }
    foreach (QJsonValue chatToRequest, chats) {
        m_receivedChatIds.append((qint64)chatToRequest.toDouble());
    }
    emit modelPopulatedCompleted();
    auto lastChat = m_model->getByUid(QString::number(m_receivedChatIds.last()));
    if (!lastChat) {
        return;
    }
    //TODO: Make loading of chats really asyncrhonous and load only visible chats at a time
    QTime sleepTime = QTime::currentTime().addMSecs(500);
    while (QTime::currentTime() < sleepTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    QScopedPointer<QTdGetChatsRequest> req(new QTdGetChatsRequest);
    req->setOffsetChatId(lastChat->id());
    req->setOffsetOrder(lastChat->order());
    req->sendAsync();
}


void QTdChatListModel::handleUpdateNewChat(const QJsonObject &data)
{
    const qint64 id = qint64(data["id"].toDouble());
    // Need to remember the model actually indexes on the qmlId variant which is a QString
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->unmarshalJson(data);
    } else {
        tdchat = QTdChatFactory::createChat(data["type"].toObject());
        tdchat->unmarshalJson(data);
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
    if (m_chatToOpenOnUpdate == tdchat->id()) {
        qWarning() << "Auto-opening chat" << tdchat->id();
        setCurrentChat(tdchat);
        m_chatToOpenOnUpdate = 0;
    }
}

void QTdChatListModel::handleUpdateChatOrder(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatOrder(data);
        emit contentsChanged();
    }
}

void QTdChatListModel::handleUpdateChatLastMessage(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateLastMessage(data["last_message"].toObject());
        tdchat->updateChatOrder(data);
        emit contentsChanged();
    }
}

void QTdChatListModel::handleAuthStateChanges(const QTdAuthState *state)
{
    switch (state->type()) {
    case QTdAuthState::Type::AUTHORIZATION_STATE_READY: {
        QTdClient::instance()->send(QJsonObject{ { "@type", "clearRecentlyFoundChats" } });
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

void QTdChatListModel::updateChatReadInbox(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatReadInbox(data);
        emit contentsChanged();
    }
}

void QTdChatListModel::updateChatReadOutbox(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatReadOutbox(data);
        emit contentsChanged();
    }
}

void QTdChatListModel::handleUpdateChatIsPinned(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatIsPinned(data);
        emit contentsChanged();
        // Update our internal PinnedChats list
        if (tdchat->isPinned() && !m_pinnedChats.contains(tdchat->id())) {
            m_pinnedChats << tdchat->id();
        } else if (!tdchat->isPinned() && m_pinnedChats.contains(tdchat->id())) {
            m_pinnedChats.removeAll(tdchat->id());
        }
    }
}

void QTdChatListModel::handleUpdateChatPhoto(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatPhoto(data["photo"].toObject());
        emit contentsChanged();
    }
}

void QTdChatListModel::handleUpdateChatReplyMarkup(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatReplyMarkup(data);
        emit contentsChanged();
    }
}

void QTdChatListModel::handleUpdateChatDraftMessage(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatDraftMessage(data["draft_message"].toObject());
        tdchat->updateChatOrder(data);
        emit contentsChanged();
    }
}

void QTdChatListModel::handleUpdateChatTitle(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatTitle(data);
        emit contentsChanged();
    }
}

void QTdChatListModel::handleUpdateChatUnreadMentionCount(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatUnreadMentionCount(data);
        emit contentsChanged();
    }
}

void QTdChatListModel::handleUpdateChatNotificationSettings(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatNotificationSettings(data);
        emit contentsChanged();
    }
}

void QTdChatListModel::handleUpdateChatOnlineMemberCount(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatOnlineMemberCount(data);
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

void QTdChatListModel::requestPositionInfo()
{
    if (!m_positionInfoSource) {
        m_positionInfoSource = QGeoPositionInfoSource::createDefaultSource(this);
        if (!m_positionInfoSource) {
            qWarning() << "Could not initialize position info source!";
            return;
        }
    }
    connect(m_positionInfoSource, &QGeoPositionInfoSource::positionUpdated,
            this, &QTdChatListModel::positionUpdated);
    m_positionInfoSource->requestUpdate(180000);
    m_positionWaitTimer->start();
}

void QTdChatListModel::cancelPositionInfo()
{
    disconnect(m_positionInfoSource, &QGeoPositionInfoSource::positionUpdated,
               this, &QTdChatListModel::positionUpdated);
    m_positionWaitTimer->stop();
}

void QTdChatListModel::onPositionInfoTimeout()
{
    cancelPositionInfo();
    emit positionInfoTimeout();
}

void QTdChatListModel::positionUpdated(const QGeoPositionInfo &positionInfo)
{
    cancelPositionInfo();
    emit positionInfoReceived(positionInfo.coordinate().latitude(), positionInfo.coordinate().longitude());
}

void QTdChatListModel::setChatDraftMessage(const QString &draftText,
                                           const qint64 &replyToMessageId,
                                           const qint64 &chatId)
{
    QScopedPointer<QTdInputMessageText> messageText(new QTdInputMessageText);
    messageText->setText(draftText);
    messageText->setClearDraft(false);
    QScopedPointer<QTdDraftMessage> draftMessage(new QTdDraftMessage);
    draftMessage->setInputMessageText(messageText.take());
    draftMessage->setReplyToMessageId(replyToMessageId);
    QScopedPointer<QTdSetChatDraftRequest> request(new QTdSetChatDraftRequest);
    request->setChatId(chatId);
    request->setDraftMessage(draftMessage.take());
    QTdClient::instance()->send(request.data());
}

void QTdChatListModel::joinChat(const qint64 &chatId) const
{
    QScopedPointer<QTdJoinChatRequest> req(new QTdJoinChatRequest);
    req->setChatId(chatId);
    QFuture<QTdResponse> resp = req->sendAsync();
    await(resp, 2000);
    qDebug() << resp.result().json();
    if (resp.result().isError()) {
        qWarning() << "Error during chat joining:" << resp.result().errorString();
    }
}

void QTdChatListModel::setChatToOpenOnUpdate(const qint64 &chatId)
{
    m_chatToOpenOnUpdate = chatId;
}

void QTdChatListModel::checkChatInviteLink(const QString &inviteLink)
{
    QScopedPointer<QTdCheckChatInviteLinkRequest> req(new QTdCheckChatInviteLinkRequest);
    req->setInviteLink(inviteLink);
    QFuture<QTdResponse> resp = req->sendAsync();
    await(resp, 2000);
    if (resp.result().isError()) {
        qWarning() << "Error during checking invite link:" << resp.result().errorString();
    }
    QPointer<QTdChatInviteLinkInfo> info(new QTdChatInviteLinkInfo);
    QJsonObject json = resp.result().json();
    info->unmarshalJson(json);
    if (info->chatId() != 0) {
        setCurrentChatById(info->chatId());
    } else {
        emit showChatInviteLinkInfo(info, inviteLink);
    }
}

void QTdChatListModel::joinChatByInviteLink(const QString &inviteLink)
{
    qDebug() << inviteLink;
    QScopedPointer<QTdJoinChatByInviteLinkRequest> req(new QTdJoinChatByInviteLinkRequest);
    req->setInviteLink(inviteLink);
    QFuture<QTdResponse> resp = req->sendAsync();
    await(resp, 2000);
    qDebug() << resp.result().json();
    if (resp.result().isError()) {
        qWarning() << "Error during joining chat by invite link:" << resp.result().errorString();
    }
    QScopedPointer<QTdChat> chat(new QTdChat);
    QJsonObject json = resp.result().json();
    chat->unmarshalJson(json);
    setChatToOpenOnUpdate(chat->id());
    setCurrentChatById(chat->id());
}

void QTdChatListModel::handleUpdateChatChatList(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatChatList(data["chat_list"].toObject());
        emit contentsChanged();
    }
}
