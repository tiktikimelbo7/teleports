#include "qtdchatlistmodel.h"
#include "client/qtdclient.h"
#include "chat/requests/qtdgetchatsrequest.h"
#include "chat/requests/qtdsetpinnedchatsrequest.h"
#include "chat/requests/qtddeletechathistoryrequest.h"
#include "chat/requests/qtdleavechatrequest.h"

#include "chat/qtdchattypefactory.h"

QTdChatListModel::QTdChatListModel(QObject *parent) : QObject(parent),
    m_model(Q_NULLPTR), m_currentChat(Q_NULLPTR), m_viewedInDetailGroup(Q_NULLPTR)
{
    m_model = new QQmlObjectListModel<QTdChat>(this, "", "id");
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

QTdChat *QTdChatListModel::viewedInDetailGroup() const
{
    return m_viewedInDetailGroup;
}

void QTdChatListModel::setCurrentChat(QTdChat *currentChat)
{
    if (m_currentChat == currentChat)
        return;
    m_currentChat = currentChat;
    emit currentChatChanged(m_currentChat);
}

void QTdChatListModel::setViewedInDetailGroup(QTdChat *group)
{
    if (m_viewedInDetailGroup == group)
        return;
    m_viewedInDetailGroup = group;
    emit viewedInDetailGroupChanged(m_viewedInDetailGroup);
}


void QTdChatListModel::clearCurrentChat()
{
    m_currentChat = Q_NULLPTR;
    emit currentChatChanged(m_currentChat);
}

void QTdChatListModel::leaveChat(qint64 chatId)
{
    QTdChat *tdchat = m_model->getByUid(QString::number(chatId));
    if (tdchat->isPrivate())
    {
        QTdDeleteChatHistoryRequest *req = new QTdDeleteChatHistoryRequest;
        req->setChatId(chatId);
        req->setRemoveFromChatList();
        QTdClient::instance()->send(req);
        req->deleteLater();
        emit contentsChanged();
    }
    else
    {
        QTdLeaveChatRequest *req = new QTdLeaveChatRequest;
        req->setChatId(chatId);
        QTdClient::instance()->send(req);
        req->deleteLater();
    }
}

void QTdChatListModel::deleteChatHistory(qint64 chatId)
{
    QTdDeleteChatHistoryRequest *req = new QTdDeleteChatHistoryRequest;
    req->setChatId(chatId);
    QTdClient::instance()->send(req);
    req->deleteLater();
    emit contentsChanged();
}

void QTdChatListModel::handleUpdateNewChat(const QJsonObject &chat)
{
    const qint64 id = qint64(chat["id"].toDouble());
    // Need to remember the model actually indexes on the qmlId variant which is a QString
    QTdChat *tdchat = m_model->getByUid(QString::number(id));
    if (!tdchat) {
        tdchat = QTdChatFactory::createChat(chat["type"].toObject());
        tdchat->unmarshalJson(chat);
        m_model->append(tdchat);
        connect(tdchat, &QTdChat::chatStatusChanged, this, &QTdChatListModel::chatStatusChanged);
        // We also need to update the internal pinned chats list now
        // otherwise any pinned chats will get removed when QTdChat::pinChat/unpinChat() is called
        connect(tdchat, &QTdChat::pinChatAction, this, &QTdChatListModel::handlePinChatAction);
        if (tdchat->isPinned()) {
            m_pinnedChats << tdchat->id();
        }
    }
    tdchat->unmarshalJson(chat);
    emit contentsChanged();
}

void QTdChatListModel::handleUpdateChatOrder(const QJsonObject &json)
{
    const qint64 id = qint64(json["chat_id"].toDouble());
    QTdChat *tdchat = m_model->getByUid(QString::number(id));
    if (tdchat) {
        tdchat->updateChatOrder(json);
        emit contentsChanged();
    }
}

void QTdChatListModel::handleUpdateChatLastMessage(const QJsonObject chat)
{
    const qint64 id = qint64(chat["chat_id"].toDouble());
    QTdChat *tdchat = m_model->getByUid(QString::number(id));
    if (tdchat) {
        tdchat->updateLastMessage(chat);
        tdchat->updateChatOrder(chat);
        emit contentsChanged();
    }
}

void QTdChatListModel::handleAuthStateChanges(const QTdAuthState *state)
{
    switch (state->type()) {
    case QTdAuthState::Type::AUTHORIZATION_STATE_READY:
    {
        QTdGetChatsRequest *req = new QTdGetChatsRequest;
        QTdClient::instance()->send(QJsonObject{{"@type", "clearRecentlyFoundChats"}});
        QTdClient::instance()->send(req);
        req->deleteLater();
        break;
    }
    case QTdAuthState::Type::AUTHORIZATION_STATE_CLOSED:
    {
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
    QTdChat *tdchat = m_model->getByUid(QString::number(id));
    if (tdchat) {
        tdchat->updateChatReadInbox(json);
        emit contentsChanged();
    }
}

void QTdChatListModel::updateChatReadOutbox(const QJsonObject &json)
{
    const qint64 id = qint64(json["chat_id"].toDouble());
    QTdChat *tdchat = m_model->getByUid(QString::number(id));
    if (tdchat) {
        tdchat->updateChatReadOutbox(json);
        emit contentsChanged();
    }
}

void QTdChatListModel::handleUpdateChatIsPinned(const QJsonObject &json)
{
    const qint64 id = qint64(json["chat_id"].toDouble());
    QTdChat *tdchat = m_model->getByUid(QString::number(id));
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
    QTdChat *tdchat = m_model->getByUid(QString::number(id));
    if (tdchat) {
//        qDebug() << "Updating chat photo";
        tdchat->updateChatPhoto(chat["photo"].toObject());
        emit contentsChanged();
    }
}

void QTdChatListModel::handleUpdateChatReplyMarkup(const QJsonObject &chat)
{
    const qint64 id = qint64(chat["chat_id"].toDouble());
    QTdChat *tdchat = m_model->getByUid(QString::number(id));
    if (tdchat) {
//        qDebug() << "Updating chat reply markup";
        tdchat->updateChatReplyMarkup(chat);
        emit contentsChanged();
    }
}

void QTdChatListModel::handleUpdateChatTitle(const QJsonObject &chat)
{
    const qint64 id = qint64(chat["chat_id"].toDouble());
    QTdChat *tdchat = m_model->getByUid(QString::number(id));
    if (tdchat) {
//        qDebug() << "Updating chat title";
        tdchat->updateChatTitle(chat);
        emit contentsChanged();
    }
}

void QTdChatListModel::handleUpdateChatUnreadMentionCount(const QJsonObject &chat)
{
    const qint64 id = qint64(chat["chat_id"].toDouble());
    QTdChat *tdchat = m_model->getByUid(QString::number(id));
    if (tdchat) {
        tdchat->updateChatUnreadMentionCount(chat);
        emit contentsChanged();
    }
}

void QTdChatListModel::handleUpdateChatNotificationSettings(const QJsonObject &chat)
{
    const qint64 id = qint64(chat["chat_id"].toDouble());
    QTdChat *tdchat = m_model->getByUid(QString::number(id));
    if (tdchat) {
        tdchat->updateChatNotificationSettings(chat);
        emit contentsChanged();
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
    auto *req = new QTdSetPinnedChatsRequest;
    req->setPinnedChats(chats);
    QTdClient::instance()->send(req);
    req->deleteLater();
}
