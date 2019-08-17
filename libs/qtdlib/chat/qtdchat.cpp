#include "utils/i18n.h"
#include "qtdchat.h"
#include <QDebug>
#include <QJsonArray>
#include "qtdchattypefactory.h"
#include "qtdchatactionfactory.h"
#include "client/qtdclient.h"
#include "chat/requests/qtdopenchatrequest.h"
#include "chat/requests/qtdclosechatrequest.h"
#include "chat/requests/qtdsetchattitlerequest.h"
#include "chat/requests/qtdsendchatactionrequest.h"
#include "chat/requests/qtddeletechathistoryrequest.h"
#include "chat/requests/qtdleavechatrequest.h"
#include "user/qtdusers.h"
#include "common/qtdhelpers.h"
#include "messages/requests/qtdgetmessagerequest.h"
#include "utils/await.h"

QTdChat::QTdChat(QObject *parent) : QAbstractInt64Id(parent),
    m_chatType(0), m_chatPhoto(new QTdChatPhoto), m_lastMessage(new QTdMessage),
    m_order(0), m_isPinned(false), m_canBeReported(false),
    m_unreadCount(0), m_lastReadInboxMsg(0), m_lastReadOutboxMsg(0),
    m_unreadMentionCount(0), m_notifySettings(new QTdNotificationSettings),
    m_messages(0), m_chatOpen(false)
{
    setType(CHAT);
    m_my_id = QTdClient::instance()->getOption("my_id").toInt();
    m_messages = new QQmlObjectListModel<QTdMessage>(this, "", "id");
    connect(QTdClient::instance(), &QTdClient::updateUserChatAction, this, &QTdChat::handleUpdateChatAction);
    connect(m_lastMessage.data(), &QTdMessage::senderChanged, this, &QTdChat::summaryChanged);
    emit messagesChanged();
}

void QTdChat::unmarshalJson(const QJsonObject &json)
{
    updateChatTitle(json);

    if (m_chatType) {
        delete m_chatType;
        m_chatType = nullptr;
    }
    m_chatType = QTdChatFactory::createType(json["type"].toObject(), this);
    emit chatTypeChanged(m_chatType);

    updateLastMessage(json["last_message"].toObject());

    updateChatOrder(json);
    updateChatIsPinned(json);

    m_canBeReported = json["can_be_reported"].toBool();
    emit canBeReportedChanged();

    updateChatReadInbox(json);

    updateChatReadOutbox(json);

    updateChatUnreadMentionCount(json);

    updateChatReplyMarkup(json);

    m_notifySettings->unmarshalJson(json["notification_settings"].toObject());
    emit notificationSettingsChanged();

    QAbstractInt64Id::unmarshalJson(json);
    updateChatPhoto(json["photo"].toObject());
}

QString QTdChat::title() const
{
    if (isMyself())
        return gettext("Saved Messages");
    else
        return m_title;
}

QTdMessage *QTdChat::lastMessage() const
{
    return m_lastMessage.data();
}

QTdChatPhoto *QTdChat::chatPhoto() const
{
    return m_chatPhoto.data();
}

QString QTdChat::initials() const
{
    if(m_title != "") {
        QString initials = "";
        QStringList parts = m_title.trimmed().split(" ", QString::SkipEmptyParts);
        for (int i = 0; i < parts.size(); i++)
        {
            initials += parts[i][0].toUpper();
            if (initials.length() >= 2) {
                break;
            }
        }
        if (initials.length() < 2) {
            initials = m_title.trimmed().left(2).toUpper();
        }
        return initials;
    }
    return "N/A";
}

QString QTdChat::avatarColor(unsigned int userId)
{
    return isMyself() ? QTdHelpers::selfColor() : QTdHelpers::avatarColor(userId);
}

void QTdChat::sendChatAction(bool isTyping)
{
    //TODO: Make more actions available
    QScopedPointer<QTdSendChatActionRequest> req(new QTdSendChatActionRequest);
    req->setChatId(id());
    QTdClient::instance()->send(req.data());
}

QString QTdChat::qmlOrder() const
{
    return m_order.toQmlValue();
}

qint64 QTdChat::order() const
{
    return m_order.value();
}

bool QTdChat::isPinned() const
{
    return m_isPinned;
}

bool QTdChat::isMuted() const
{
    return m_notifySettings->muteFor() > 0;
}

bool QTdChat::isPrivate() const
{
    return qobject_cast<QTdChatTypePrivate*>(m_chatType) != NULL;
}

bool QTdChat::isSecret() const
{
    return qobject_cast<QTdChatTypeSecret*>(m_chatType) != NULL;
}

bool QTdChat::isGroup() const
{
    if (qobject_cast<QTdChatTypeBasicGroup*>(m_chatType) != NULL)
        return true;
    auto result = qobject_cast<QTdChatTypeSuperGroup*>(m_chatType);
    return result != NULL && !result->isChannel();
}

bool QTdChat::isChannel() const
{
    auto result = qobject_cast<QTdChatTypeSuperGroup*>(m_chatType);
    return result != NULL && result->isChannel();
}

bool QTdChat::isMyself() const
{
    return id() == m_my_id;
}

bool QTdChat::canBeReported() const
{
    return m_canBeReported;
}

bool QTdChat::hasUnreadMessages() const
{
    return unreadCount() > 0;
}

QString QTdChat::qmlUnreadCount() const
{
    return m_unreadCount.toQmlValue();
}

qint32 QTdChat::unreadCount() const
{
    return m_unreadCount.value();
}

QString QTdChat::qmlLastReadInboxMessageId() const
{
    return m_lastReadInboxMsg.toQmlValue();
}

qint64 QTdChat::lastReadInboxMessageId() const
{
    return m_lastReadInboxMsg.value();
}

QString QTdChat::qmlLastReadOutboxMessageId() const
{
    return m_lastReadOutboxMsg.toQmlValue();
}

qint64 QTdChat::lastReadOutboxMessageId() const
{
    return m_lastReadOutboxMsg.value();
}

bool QTdChat::hasUnreadMentions() const
{
    return unreadMentionCount() > 0;
}

QString QTdChat::qmlUnreadMentionCount() const
{
    return m_unreadMentionCount.toQmlValue();
}

qint32 QTdChat::unreadMentionCount() const
{
    return m_unreadMentionCount.value();
}

QString QTdChat::qmlReplyMarkupMessageId() const
{
    return m_replyMarkupMessageId.toQmlValue();
}

qint64 QTdChat::replyMarkupMessageId() const
{
    return m_replyMarkupMessageId.value();
}

QTdMessage *QTdChat::replyMarkupMessage() const
{
    return m_replyMarkupMessage;
}

bool QTdChat::hasReplyMarkup() const
{
    return m_replyMarkupMessageId.value() != 0;
}

void QTdChat::loadReplyMarkupMessage() {
    if (!hasReplyMarkup()) {
        return;
    }
    QScopedPointer<QTdGetMessageRequest> req(new QTdGetMessageRequest);
    req->setChatId(id());
    req->setMessageId(m_replyMarkupMessageId.value());
    QFuture<QTdResponse> resp = req->sendAsync();
    await(resp, 2000);
    if (resp.result().isError()) {
        qWarning() << "Failed to get reply markup message with error: " << resp.result().errorString();
        return;
    }
    m_replyMarkupMessage = new QTdMessage(this);
    m_replyMarkupMessage->unmarshalJson(resp.result().json());
    emit replyMarkupMessageChanged();
}

QTdNotificationSettings *QTdChat::notificationSettings() const
{
    return m_notifySettings.data();
}

QString QTdChat::action() const {
    auto *users = QTdUsers::instance()->model();
    QString actionMessage;
    switch(m_chatActions.count())
    {
        case 0:
            return "";
        case 1:
        {
          auto *user =
              users->getByUid(QString::number(m_chatActions.first().userId.value()));
          if (user)
              actionMessage = QString("%1 %2 ").arg(
                  user->firstName(), m_chatActions.first().singular_description);
        }
        break;
        case 2:
        {
          auto *user1 = users->getByUid(
              QString::number(m_chatActions.first().userId.value()));
          if (user1)
            actionMessage = QString("%1, ").arg(user1->firstName());
          auto *user2 = users->getByUid(
              QString::number(m_chatActions.last().userId.value()));
          if (user2)
            actionMessage += QString("%1 %2").arg(
                user2->firstName(), m_chatActions.last().plural_description);
        }
        break;
        default: {
            actionMessage = QString("%1 %2").arg(
                m_chatActions.count()).arg(m_chatActions.first().plural_description);
        }
    }
    return actionMessage;
}

QString QTdChat::summary() const {
    if (action() != "") {
        return action();
    }
    return m_lastMessage->isValid() ? m_lastMessage->summary() : QString();
}

QObject *QTdChat::messages() const
{
    return m_messages;
}

void QTdChat::openChat()
{
    m_chatOpen = true;
    QScopedPointer<QTdOpenChatRequest> req(new QTdOpenChatRequest);
    req->setChatId(id());
    QTdClient::instance()->send(req.data());
    onChatOpened();
    QTdChat::loadReplyMarkupMessage();
}

void QTdChat::closeChat()
{
    m_chatOpen = false;
    QScopedPointer<QTdCloseChatRequest> req(new QTdCloseChatRequest);
    req->setChatId(id());
    QTdClient::instance()->send(req.data());
    m_currentMessageIndex= -1;
    emit closed();
}

void QTdChat::pinChat()
{
    if (!m_isPinned) {
        emit pinChatAction(id(), true);
    }
}

void QTdChat::unpinChat()
{
    if (m_isPinned) {
        emit pinChatAction(id(), false);
    }
}

void QTdChat::setTitle(const QString &title)
{
    if (m_title != title) {
        QScopedPointer<QTdSetChatTitleRequest> req(new QTdSetChatTitleRequest);
        req->setTitle(id(), title);
        QTdClient::instance()->send(req.data());
    }
}

void QTdChat::deleteChatHistory(const bool &removeFromChatlist)
{
    if (m_chatType->type() == QTdChat::CHAT_TYPE_SUPERGROUP || isChannel()) {
        qWarning() << "Cannot delete chat history for supergroups or channels";
        return;
    }
    QScopedPointer<QTdDeleteChatHistoryRequest> req(new QTdDeleteChatHistoryRequest);
    req->setChatId(id());
    req->setRemoveFromChatList(removeFromChatlist);
    QTdClient::instance()->send(req.data());
}

void QTdChat::leaveChat()
{
    /**
     * Because telegram likes to have all different types of
     * groups and it is probably going to change again. SO let's
     * for the sake of simplicity use a switch and handle each type
     * appropriately.
     *
     * Currently private & secret chats can only delete history and be removed
     * from chatlist.
     *
     * Supergroups and basicgroups can be left using the id()
     * Strangely it takes the id and not superGroupId and basicGroupId
     */
    QScopedPointer<QTdLeaveChatRequest> req(new QTdLeaveChatRequest);
    switch(m_chatType->type()) {
    case QTdChat::CHAT_TYPE_PRIVATE:
    case QTdChat::CHAT_TYPE_SECRET:
        return deleteChatHistory(true);
    case QTdChat::CHAT_TYPE_SUPERGROUP:
    case QTdChat::CHAT_TYPE_BASIC_GROUP:
    {
        req->setChatId(id());
        break;
    }
    default:
        break;
    }
    QTdClient::instance()->send(req.data());
}

void QTdChat::updateChatOrder(const QJsonObject &json)
{
    m_order = json["order"];
    emit orderChanged();
}

void QTdChat::updateChatReadInbox(const QJsonObject &json)
{
    m_unreadCount = json["unread_count"];
    emit unreadCountChanged();

    m_lastReadInboxMsg = json["last_read_inbox_message_id"];
    emit lastReadInboxMessageIdChanged();
}

void QTdChat::updateChatReadOutbox(const QJsonObject &json)
{
    m_lastReadOutboxMsg = json["last_read_outbox_message_id"];
    emit lastReadOutboxMessageIdChanged();
}

void QTdChat::updateChatIsPinned(const QJsonObject &json)
{
    m_isPinned = json["is_pinned"].toBool();
    emit isPinnedChanged();
    updateChatOrder(json);
}

void QTdChat::updateChatPhoto(const QJsonObject &photo)
{
    m_chatPhoto->unmarshalJson(photo);
    emit chatPhotoChanged(m_chatPhoto.data());

    if (m_chatPhoto->small()->local()->path().isEmpty()) {
        connect(m_chatPhoto->small()->local(), &QTdLocalFile::pathChanged, this, &QTdChat::handleChatPhotoDownloaded);
        m_chatPhoto->small()->downloadFile();
    } else
    {
        QTdClient::instance()->setAvatarMapEntry(id(), m_chatPhoto->small()->local()->path());
    }
}

void QTdChat::updateChatReplyMarkup(const QJsonObject &json)
{
    m_replyMarkupMessageId = json["reply_markup_message_id"];
    if (hasReplyMarkup() && m_chatOpen) {
        loadReplyMarkupMessage();
    }
}

void QTdChat::updateChatTitle(const QJsonObject &json)
{
    m_title = json["title"].toString();
    emit titleChanged(m_title);
}

void QTdChat::updateChatUnreadMentionCount(const QJsonObject &json)
{
    m_unreadMentionCount = json["unread_mention_count"];
    emit unreadMentionCountChanged();
}

void QTdChat::updateChatNotificationSettings(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }
    m_notifySettings->unmarshalJson(json["notification_settings"].toObject());
    emit notificationSettingsChanged();
}


void QTdChat::updateLastMessage(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }

    m_lastMsgJson = json["last_message"].toObject();
    m_lastMessage->unmarshalJson(m_lastMsgJson);
    emit lastMessageChanged(m_lastMessage.data());
    emit summaryChanged();
}

void QTdChat::handleUpdateChatAction(const QJsonObject &json)
{
    const qint64 cid = qint64(json["chat_id"].toDouble());
    if (cid != id()) {
        return;
    }
    updateChatAction(json);
}

void QTdChat::handleChatPhotoDownloaded() {
    QTdClient::instance()->setAvatarMapEntry(id(), m_chatPhoto->small()->local()->path());
}

void QTdChat::onChatOpened()
{
}

void QTdChat::onChatClosed()
{
}

void QTdChat::forwardMessage(const QString &messageId) {
    QStringList forwardingMessages = QStringList(messageId);
    emit forwardingMessagesAction(forwardingMessages, this);
}

void QTdChat::updateChatAction(const QJsonObject &json)
{
    const qint32 user_id = qint32(json["user_id"].toInt());
    const QJsonObject data = json["action"].toObject();
    QTdChatAction *action = QTdChatActionFactory::create(data, this);
    if (action->type() == QTdChatAction::Type::CHAT_ACTION_CANCEL && m_chatActions.contains(user_id)) {
        m_chatActions.remove(user_id);
    } else if (action->type() != QTdChatAction::Type::CHAT_ACTION_CANCEL && !m_chatActions.contains(user_id)) {
        QString singular_description;
        QString plural_description;
        // TODO: i18n these strings
        switch (action->type()) {
        case QTdChatAction::Type::CHAT_ACTION_CANCEL:
            return;
        case QTdChatAction::Type::CHAT_ACTION_CHOOSING_CONTACT:
          singular_description = QStringLiteral("is choosing contact...");
          plural_description = QStringLiteral("are choosing contact...");
          break;
        case QTdChatAction::Type::CHAT_ACTION_CHOOSING_LOCATION:
          singular_description = QStringLiteral("is choosing location...");
          plural_description = QStringLiteral("are choosing location...");
          break;
        case QTdChatAction::Type::CHAT_ACTION_RECORDING_VIDEO:
        case QTdChatAction::Type::CHAT_ACTION_RECORDING_VIDEO_NOTE:
        case QTdChatAction::Type::CHAT_ACTION_RECORDING_VOICE_NOTE:
          singular_description = QStringLiteral("is recording...");
          plural_description = QStringLiteral("are recording...");
          break;
        case QTdChatAction::Type::CHAT_ACTION_TYPING:
          singular_description = QStringLiteral("is typing...");
          plural_description = QStringLiteral("are typing...");
          break;
        default:
          singular_description = QStringLiteral("is doing something");
          plural_description = QStringLiteral("are doing something");
          break;
        }
        m_chatActions.insert(user_id, useraction(user_id, singular_description,
                                                 plural_description));
    }
    action->deleteLater();
    emit summaryChanged();
}

QTdChatType *QTdChat::chatType() const
{
    return m_chatType;
}

QTdChatPhoto::QTdChatPhoto(QObject *parent) : QTdPhoto(parent)
{
    setType(CHAT_PHOTO);
}

QString QTdChat::formatDate(const QDateTime &dt)
{
    return QTdHelpers::formatDate(dt);
}

int QTdChat::currentMessageIndex() const
{
    return m_currentMessageIndex;
}

void QTdChat::positionMessageListViewAtIndex(int index)
{
    m_currentMessageIndex = index;
    currentMessageIndexChanged();
}

