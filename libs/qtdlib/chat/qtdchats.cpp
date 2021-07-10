#include "qtdchats.h"
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

QTdChats::QTdChats(QObject *parent)
    : QObject(parent)
    , m_model(Q_NULLPTR)
    , m_sortedmodel(new QTdSortedChats)
    , m_currentChat(Q_NULLPTR)
    , m_forwardedFromChat(Q_NULLPTR)
    , m_forwardingMessages(QStringList())
    , m_listMode(ListMode::Idle)
    , m_positionWaitTimer(new QTimer(this))
    , m_chatToOpenOnUpdate(0)
{
    m_model = new QQmlObjectListModel<QTdChat>(this, "", "id");
    m_positionWaitTimer->setInterval(180000);
    m_positionWaitTimer->setSingleShot(true);
    connect(this->m_positionWaitTimer, &QTimer::timeout, this, &QTdChats::onPositionInfoTimeout);

    connect(QTdClient::instance(), &QTdClient::chats, this, &QTdChats::handleChats);
    connect(QTdClient::instance(), &QTdClient::chat, this, &QTdChats::handleChat);
    connect(QTdClient::instance(), &QTdClient::updateNewChat, this, &QTdChats::handleUpdateNewChat);
    connect(QTdClient::instance(), &QTdClient::authStateChanged, this, &QTdChats::handleAuthStateChanges);
    connect(QTdClient::instance(), &QTdClient::updateChatOrder, this, &QTdChats::handleUpdateChatOrder);
    connect(QTdClient::instance(), &QTdClient::updateChatLastMessage, this, &QTdChats::handleUpdateChatLastMessage);
    connect(QTdClient::instance(), &QTdClient::updateChatReadInbox, this, &QTdChats::updateChatReadInbox);
    connect(QTdClient::instance(), &QTdClient::updateChatReadOutbox, this, &QTdChats::updateChatReadOutbox);
    connect(QTdClient::instance(), &QTdClient::updateChatIsPinned, this, &QTdChats::handleUpdateChatIsPinned);
    connect(QTdClient::instance(), &QTdClient::updateChatPhoto, this, &QTdChats::handleUpdateChatPhoto);
    connect(QTdClient::instance(), &QTdClient::updateChatReplyMarkup, this, &QTdChats::handleUpdateChatReplyMarkup);
    connect(QTdClient::instance(), &QTdClient::updateChatDraftMessage, this, &QTdChats::handleUpdateChatDraftMessage);
    connect(QTdClient::instance(), &QTdClient::updateChatTitle, this, &QTdChats::handleUpdateChatTitle);
    connect(QTdClient::instance(), &QTdClient::updateChatUnreadMentionCount, this, &QTdChats::handleUpdateChatUnreadMentionCount);
    connect(QTdClient::instance(), &QTdClient::updateChatNotificationSettings, this, &QTdChats::handleUpdateChatNotificationSettings);
    connect(QTdClient::instance(), &QTdClient::updateChatOnlineMemberCount, this, &QTdChats::handleUpdateChatOnlineMemberCount);
    connect(QTdClient::instance(), &QTdClient::updateChatChatList, this, &QTdChats::handleUpdateChatChatList);
}

QTdChats::~QTdChats()
{
    m_model->clear();
}

static QPointer<QTdChats> s_chats;
QTdChats *QTdChats::instance()
{
    if (s_chats.isNull()) {
        s_chats = new QTdChats();
    }
    return s_chats;
}

QObject *QTdChats::model() const
{
    return m_model;
}

QTdChat *QTdChats::currentChat() const
{
    return m_currentChat;
}

QTdChat *QTdChats::chatById(const qint64 &chatId) const
{
    return m_model->getByUid(QString::number(chatId));
}

void QTdChats::createOrOpenSecretChat(const int &userId)
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

void QTdChats::createOrOpenPrivateChat(const int &userId)
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

void QTdChats::createOrOpenSavedMessages()
{
    createOrOpenPrivateChat(QTdClient::instance()->getOption("my_id").toInt());
}

void QTdChats::setCurrentChatById(const qint64 &chatId)
{
    QTdChat *currentChat = chatById(chatId);
    setCurrentChat(currentChat);
}

void QTdChats::setCurrentChatByUsername(const QString &username)
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

qint32 QTdChats::forwardingMessagesCount() const
{
    return m_forwardingMessages.length();
}

void QTdChats::setCurrentChat(QTdChat *currentChat)
{
    if (currentChat == nullptr) {
        return;
    }
    if (m_currentChat && m_currentChat->isOpen()) {
        if (m_currentChat == currentChat)
            return;
        else
            m_currentChat->closeChat();
    }

    m_currentChat = currentChat;
    emit currentChatChanged();
}

QTdChat *QTdChats::forwardedFromChat() const
{
    return m_forwardedFromChat;
}

void QTdChats::setForwardedFromChat(QTdChat *forwardedFromChat)
{
    if (m_forwardedFromChat == forwardedFromChat)
        return;
    m_forwardedFromChat = forwardedFromChat;
}

QStringList QTdChats::forwardingMessages() const
{
    return m_forwardingMessages;
}

void QTdChats::setForwardingMessages(QStringList forwardingMessages)
{
    if (m_forwardingMessages == forwardingMessages)
        return;
    m_forwardingMessages = forwardingMessages;
}

void QTdChats::handleChat(const QJsonObject &data)
{
    QScopedPointer<QTdChat> chat(new QTdChat);
    chat->unmarshalJson(data);
    handleUpdateNewChat(data);
}

void QTdChats::handleChats(const QJsonObject &data)
{
    QJsonArray chats = data["chat_ids"].toArray();
    if (chats.count() == 0) {
        qDebug() << "No more chats found, completing initial load.";
        m_receivedChatIds.clear();
        return;
    }
    qDebug() << "Received" << chats.count() << "chats";
    foreach (QJsonValue chat, chats) {
        auto chatId = chat.toInt();
        if (chatId == 0) {
            continue;
        }
        m_receivedChatIds.append(chatId);
        //Only request chats that we did not receive already
        if (!chatById(chatId)) {
            QScopedPointer<QTdGetChatRequest> chatReq(new QTdGetChatRequest);
            chatReq->setChatId(chatId);
            qDebug() << "Request chat id" << chatId << "to be added to chatmodel";
            chatReq->sendAsync();
        } else {
            qDebug() << "Chat" << chatId << "already received, ignoring";
        }
    }
    emit modelPopulatedCompleted();
}

void QTdChats::handleUpdateNewChat(const QJsonObject &data)
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
        connect(tdchat, &QTdChat::chatStatusChanged, this, &QTdChats::chatStatusChanged);
        // We also need to update the internal pinned chats list now
        // otherwise any pinned chats will get removed when QTdChat::pinChat/unpinChat() is called
        connect(tdchat, &QTdChat::pinChatAction, this, &QTdChats::handlePinChatAction);
        connect(tdchat, &QTdChat::forwardingMessagesAction, this, &QTdChats::handleForwardingMessagesAction);
        if (tdchat->isPinned()) {
            m_pinnedChats << tdchat->id();
        }
    }
    emit contentsChanged();
    if (m_chatToOpenOnUpdate == tdchat->id()) {
        qDebug() << "Auto-opening chat" << tdchat->id();
        setCurrentChat(tdchat);
        m_chatToOpenOnUpdate = 0;
    }
}

void QTdChats::handleUpdateChatOrder(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatOrder(data);
        emit contentsChanged();
    }
}

void QTdChats::handleUpdateChatLastMessage(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateLastMessage(data["last_message"].toObject());
        tdchat->updateChatOrder(data);
        emit contentsChanged();
    }
}

void QTdChats::handleUpdateChatChatList(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatChatList(data["chat_list"].toObject());
        emit contentsChanged();
    }
}

void QTdChats::handleAuthStateChanges(const QTdAuthState *state)
{
    switch (state->type()) {
    case QTdAuthState::Type::AUTHORIZATION_STATE_READY: {
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

void QTdChats::updateChatReadInbox(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatReadInbox(data);
        emit contentsChanged();
    }
}

void QTdChats::updateChatReadOutbox(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatReadOutbox(data);
        emit contentsChanged();
    }
}

void QTdChats::handleUpdateChatIsPinned(const QJsonObject &data)
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

void QTdChats::handleUpdateChatPhoto(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatPhoto(data["photo"].toObject());
        emit contentsChanged();
    }
}

void QTdChats::handleUpdateChatReplyMarkup(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatReplyMarkup(data);
        emit contentsChanged();
    }
}

void QTdChats::handleUpdateChatDraftMessage(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatDraftMessage(data["draft_message"].toObject());
        tdchat->updateChatOrder(data);
        emit contentsChanged();
    }
}

void QTdChats::handleUpdateChatTitle(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatTitle(data);
        emit contentsChanged();
    }
}

void QTdChats::handleUpdateChatUnreadMentionCount(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatUnreadMentionCount(data);
        emit contentsChanged();
    }
}

void QTdChats::handleUpdateChatNotificationSettings(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatNotificationSettings(data);
        emit contentsChanged();
    }
}

void QTdChats::handleUpdateChatOnlineMemberCount(const QJsonObject &data)
{
    const qint64 id = qint64(data["chat_id"].toDouble());
    QTdChat *tdchat = chatById(id);
    if (tdchat) {
        tdchat->updateChatOnlineMemberCount(data);
        emit contentsChanged();
    }
}

void QTdChats::sendForwardMessage(const QStringList &forwardMessageIds,
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

void QTdChats::handlePinChatAction(const qint64 &chatId, const bool &pinned)
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

void QTdChats::handleForwardingMessagesAction()
{
    setListMode(ListMode::ForwardingMessages);
}

QTdChats::ListMode QTdChats::listMode() const
{
    return m_listMode;
}

void QTdChats::setListMode(ListMode listMode)
{
    m_listMode = listMode;
    emit listModeChanged();
}

void QTdChats::requestPositionInfo()
{
    if (!m_positionInfoSource) {
        m_positionInfoSource = QGeoPositionInfoSource::createDefaultSource(this);
        if (!m_positionInfoSource) {
            qWarning() << "Could not initialize position info source!";
            return;
        }
    }
    connect(m_positionInfoSource, &QGeoPositionInfoSource::positionUpdated,
            this, &QTdChats::positionUpdated);
    m_positionInfoSource->requestUpdate(180000);
    m_positionWaitTimer->start();
}

void QTdChats::cancelPositionInfo()
{
    disconnect(m_positionInfoSource, &QGeoPositionInfoSource::positionUpdated,
               this, &QTdChats::positionUpdated);
    m_positionWaitTimer->stop();
}

void QTdChats::onPositionInfoTimeout()
{
    cancelPositionInfo();
    emit positionInfoTimeout();
}

void QTdChats::positionUpdated(const QGeoPositionInfo &positionInfo)
{
    cancelPositionInfo();
    emit positionInfoReceived(positionInfo.coordinate().latitude(), positionInfo.coordinate().longitude());
}

void QTdChats::setChatDraftMessage(const QString &draftText,
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

void QTdChats::joinChat(const qint64 &chatId) const
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

void QTdChats::setChatToOpenOnUpdate(const qint64 &chatId)
{
    m_chatToOpenOnUpdate = chatId;
}

void QTdChats::loadMoreChats(const QString &chatList)
{
    QScopedPointer<QTdGetChatsRequest> req(new QTdGetChatsRequest);

    if (m_model->isEmpty()) {
        req->setOffsetChatId(0);
        req->setOffsetOrder(9223372036854775807);
        req->setChatList(chatList);
    } else {
        auto lastChat = m_model->first();
        req->setOffsetChatId(lastChat->id());
        req->setOffsetOrder(lastChat->order());
    }
    req->setLimit(10);
    req->sendAsync();
}

void QTdChats::checkChatInviteLink(const QString &inviteLink)
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

void QTdChats::joinChatByInviteLink(const QString &inviteLink)
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
