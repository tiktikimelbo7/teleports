#include "qtdmessage.h"
#include <QDebug>
#include "client/qtdclient.h"
#include "user/qtdusers.h"
#include "qtdmessagecontentfactory.h"
#include "content/qtdmessagetext.h"
#include "content/qtdmessagedate.h"
#include "content/qtdmessageunreadlabel.h"
#include "content/qtdmessagesticker.h"
#include "content/qtdmessagecontact.h"
#include "content/qtdmessagedocument.h"
#include "content/qtdmessagelocation.h"
#include "content/qtdmessagecustomserviceaction.h"
#include "common/qtdhelpers.h"
#include "requests/qtdgetmessagerequest.h"
#include "user/requests/qtdgetuserrequest.h"
#include "chat/requests/qtdgetchatrequest.h"
#include "chat/qtdchat.h"
#include "utils/await.h"

//#include "i18n.h"

QTdMessage::QTdMessage(QObject *parent)
    : QAbstractInt64Id(parent)
    , m_date(0)
    , m_sender_user_id(0)
    , m_chatId(0)
    , m_sender(Q_NULLPTR)
    , m_waitingForSender(false)
    , m_sendingState(Q_NULLPTR)
    , m_isOutgoing(false)
    , m_canBeEdited(false)
    , m_canBeForwarded(false)
    , m_canBeDeletedOnlyForSelf(false)
    , m_canBeDeletedForAllUsers(false)
    , m_isChannelPost(false)
    , m_containsUnreadMention(false)
    , m_content(Q_NULLPTR)
    , m_isValid(false)
    , m_previousSender(0)
    , m_nextSender(0)
    , m_replyMarkup(Q_NULLPTR)
    , m_forwardedFromDetails("")
    , m_messageRepliedTo(Q_NULLPTR)
    , m_replyToMessageId(0)
    , m_isCollapsed(false)
{
    setType(MESSAGE);
}

QDateTime QTdMessage::qmlDate() const
{
    return QDateTime::fromTime_t(m_date);
}

qint32 QTdMessage::date() const
{
    return m_date;
}

QString QTdMessage::qmlSenderUserId() const
{
    return m_sender_user_id.toQmlValue();
}

qint32 QTdMessage::senderUserId() const
{
    return m_sender_user_id.value();
}

QString QTdMessage::qmlChatId() const
{
    return m_chatId.toQmlValue();
}

qint64 QTdMessage::chatId() const
{
    return m_chatId.value();
}

QTdUser *QTdMessage::sender() const
{
    return m_sender;
}

void QTdMessage::unmarshalJson(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }
    if (m_content) {
        delete m_content;
        m_content = nullptr;
    }

    // This is a special case to insert a date and unread labels into the
    // model so messages can be grouped by day and by read-state. No data
    // beyond so we just return early.
    if (json.keys().contains("dateLabel")) {
        auto *md = new QTdMessageDate(this);
        md->setDate(json["dateLabel"].toInt());
        m_content = md;
        return;
    }
    if (json.keys().contains("unreadLabel")) {
        auto *md = new QTdMessageUnreadLabel(this);
        md->setLabel(json["unreadLabel"].toString());
        m_content = md;
        return;
    }

    m_isValid = false;
    m_date = qint32(json["date"].toInt());
    m_sender_user_id = json["sender_user_id"];
    if (m_sender_user_id.isValid()) {
        updateSender(m_sender_user_id.value());
    }
    m_chatId = json["chat_id"];

    updateSendingState(json);

    m_isOutgoing = json["is_outgoing"].toBool();
    m_isEdited = qint32(json["edit_date"].toInt()) > 0;
    m_canBeEdited = json["can_be_edited"].toBool();
    m_canBeForwarded = json["can_be_forwarded"].toBool();
    m_canBeDeletedOnlyForSelf = json["can_be_deleted_only_for_self"].toBool();
    m_canBeDeletedForAllUsers = json["can_be_deleted_for_all_users"].toBool();
    m_isChannelPost = json["is_channel_post"].toBool();
    m_views = json["views"].toInt();
    m_containsUnreadMention = json["contains_unread_mention"].toBool();
    m_replyToMessageId = json["reply_to_message_id"];
    if (isReply() && !isCollapsed()) {
        if (m_messageRepliedTo == Q_NULLPTR) {
            connect(QTdClient::instance(), &QTdClient::message, this, &QTdMessage::handleMessage);
        }
        QScopedPointer<QTdGetMessageRequest> request(new QTdGetMessageRequest);
        request->setChatId(chatId());
        request->setMessageId(replyToMessageId());
        QTdClient::instance()->send(request.data());
    }

    const QJsonObject content = json["content"].toObject();
    m_content = QTdMessageContentFactory::create(content, this);
    m_content->unmarshalJson(content);

    if (m_replyMarkup) {
        delete m_replyMarkup;
        m_replyMarkup = nullptr;
    }
    const QJsonObject replyMarkup = json["reply_markup"].toObject();
    if (!replyMarkup.isEmpty()) {
        const QString replyMarkupType = replyMarkup["@type"].toString();
        if (replyMarkupType == "replyMarkupForceReply") {
            m_replyMarkup = new QTdReplyMarkupForceReply(this);
        } else if (replyMarkupType == "replyMarkupInlineKeyboard") {
            m_replyMarkup = new QTdReplyMarkupInlineKeyboard(this);
        } else if (replyMarkupType == "replyMarkupRemoveKeyboard") {
            m_replyMarkup = new QTdReplyMarkupRemoveKeyboard(this);
        } else if (replyMarkupType == "replyMarkupShowKeyboard") {
            m_replyMarkup = new QTdReplyMarkupShowKeyboard(this);
        }
        if (m_replyMarkup) {
            m_replyMarkup->unmarshalJson(replyMarkup);
        }
    }

    if (m_forwardInfo) {
        delete m_forwardInfo;
        m_forwardInfo = nullptr;
    }
    qint64 forwardedFromId = 0;
    const QJsonObject forwardInfo = json["forward_info"].toObject();
    if (!forwardInfo.isEmpty()) {
        const QString forwardInfoType = forwardInfo["@type"].toString();
        if (forwardInfoType == "messageForwardedFromUser") {
            auto tempForwardInfo = new QTdMessageForwardedFromUser(this);
            tempForwardInfo->unmarshalJson(forwardInfo);
            m_forwardInfo = tempForwardInfo;
            forwardedFromId = tempForwardInfo->senderUserId();
            QScopedPointer<QTdGetUserRequest> req(new QTdGetUserRequest);
            req->setUserId(forwardedFromId);
            QFuture<QTdResponse> resp = req->sendAsync();
            await(resp, 2000);
            if (resp.result().isError()) {
                qWarning() << "Failed to get forward info message with error: " << resp.result().errorString();
                return;
            }
            auto tempUser = new QTdUser(this);
            tempUser->unmarshalJson(resp.result().json());
            m_forwardedFromDetails = tempUser->firstName() + " " + tempUser->lastName();
        } else if (forwardInfoType == "messageForwardedPost") {
            auto tempForwardInfo = new QTdMessageForwardedPost(this);
            tempForwardInfo->unmarshalJson(forwardInfo);
            m_forwardInfo = tempForwardInfo;
            forwardedFromId = tempForwardInfo->chatId();
            QScopedPointer<QTdGetChatRequest> req(new QTdGetChatRequest);
            req->setChatId(forwardedFromId);
            QFuture<QTdResponse> resp = req->sendAsync();
            await(resp, 2000);
            if (resp.result().isError()) {
                qWarning() << "Failed to get forward info message with error: " << resp.result().errorString();
                return;
            }
            auto tempChat = new QTdChat(this);
            tempChat->unmarshalJson(resp.result().json());
            m_forwardedFromDetails = tempChat->title();
        }
        emit messageChanged();
    }
    emit messageChanged();
    QAbstractInt64Id::unmarshalJson(json);
    m_isValid = true;
}

void QTdMessage::unmarshalUpdateContent(const QJsonObject &content)
{
    if (content.isEmpty()) {
        return;
    }
    if (m_content) {
        delete m_content;
        m_content = nullptr;
    }

    m_isValid = false;
    m_content = QTdMessageContentFactory::create(content, this);
    m_content->unmarshalJson(content);

    emit messageChanged();
    m_isValid = true;
}

QTdMessageSendingState *QTdMessage::sendingState() const
{
    return m_sendingState;
}

bool QTdMessage::isOutgoing() const
{
    return m_isOutgoing;
}

bool QTdMessage::isEdited() const
{
    return m_isEdited;
}

bool QTdMessage::canBeEdited() const
{
    return m_canBeEdited;
}

bool QTdMessage::canBeForwarded() const
{
    return m_canBeForwarded;
}

bool QTdMessage::canBeDeletedOnlyForSelf() const
{
    return m_canBeDeletedOnlyForSelf;
}

bool QTdMessage::canBeDeletedForAllUsers() const
{
    return m_canBeDeletedForAllUsers;
}

bool QTdMessage::isChannelPost() const
{
    return m_isChannelPost;
}

QString QTdMessage::views() const
{

    if (m_views > 9999 && m_views <= 999999)
        return  QString("%1K").arg(((double)(m_views/ 100)) / 10, 0, 'd', 1);
    else if (m_views > 999999)
        return QString("%1M").arg(((double)(m_views / 100000)) / 10, 0, 'd', 1);
    else
        return QString("%1").arg(m_views);
}

bool QTdMessage::containsUnreadMention() const
{
    return m_containsUnreadMention;
}

QTdMessageContent *QTdMessage::content() const
{
    return m_content;
}

QTdReplyMarkup *QTdMessage::replyMarkup() const
{
    return m_replyMarkup;
}

QString QTdMessage::forwardedFromDetails() const
{
    return m_forwardedFromDetails;
}

bool QTdMessage::isForwarded() const
{
    return m_forwardInfo != nullptr;
}

QString QTdMessage::summary() const
{
    QString content;

    switch (m_content->type()) {
    case QTdObject::MESSAGE_TEXT:
    {
        auto *c = qobject_cast<QTdMessageText*>(m_content);
        content = c->text()->text();
        break;
    }
    case QTdObject::MESSAGE_STICKER:
    {
        auto *c = qobject_cast<QTdMessageSticker*>(m_content);
        content = c->sticker()->emoji() + " " + tr("Sticker");
        break;
    }
    case QTdObject::MESSAGE_CALL: {
        content = tr("call has been ended");
        break;
    }
    case QTdObject::MESSAGE_AUDIO: {
        content = tr("sent an audio message");
        break;
    }
    case QTdObject::MESSAGE_PHOTO: {
        content = tr("sent a photo");
        break;
    }
    case QTdObject::MESSAGE_CONTACT: {
        content = tr("Contact");
        break;
    }
    case QTdObject::MESSAGE_DOCUMENT: {
        auto *c = qobject_cast<QTdMessageDocument*>(m_content);
        content = c->document()->fileName();
        break;
    }
    case QTdObject::MESSAGE_LOCATION: {
        content = tr("Location");
        break;
    }
    case QTdObject::MESSAGE_VIDEO: {
        content = tr("sent a video");
        break;
    }
    case QTdObject::MESSAGE_VIDEO_NOTE: {
        content = tr("sent a video note");
        break;
    }
    case QTdObject::MESSAGE_VOICE_NOTE: {
        content = tr("sent a voice note");
        break;
    }
    case QTdObject::MESSAGE_CHAT_ADD_MEMBERS: {
        content = tr("joined the group");
        break;
    }
    case QTdObject::MESSAGE_CHAT_CHANGE_PHOTO: {
        content = tr("changed the chat photo");
        break;
    }
    case QTdObject::MESSAGE_CHAT_CHANGE_TITLE: {
        content = tr("changed the chat title");
        break;
    }
    case QTdObject::MESSAGE_CHAT_JOIN_BY_LINK: {
        content = tr("joined by invite link");
        break;
    }
    case QTdObject::MESSAGE_CHAT_DELETE_MEMBER: {
        content = tr("removed a member");
        break;
    }
    case QTdObject::MESSAGE_CHAT_DELETE_PHOTO: {
        content = tr("deleted the chat photo");
        break;
    }
    case QTdObject::MESSAGE_CHAT_UPGRADE_FROM: {
        content = tr("upgraded to supergroup");
        break;
    }
    case QTdObject::MESSAGE_CHAT_SET_TTL: {
        content = tr("message TTL has been changed");
        break;
    }
    case QTdObject::MESSAGE_BASIC_GROUP_CHAT_CREATE: {
        content = tr("created this group");
        break;
    }
    case QTdObject::MESSAGE_CUSTOM_SERVICE_ACTION: {
        auto *c = qobject_cast<QTdMessageCustomServiceAction*>(m_content);
        content = c->text();
        break;
    }
    default : content = tr("sent an unknown message: %1").arg(m_content->typeString());
        break;
    }

    if (isOutgoing())
        return QString("%1: %2").arg(tr("Me"), content);
    QString name;
    if (m_sender) {
        name = m_sender->firstName();
        if (name.isEmpty()) {
            name = m_sender->username();
        }
    }
    return name.isEmpty() ? content : QString("%1: %2").arg(name, content);
}

QString QTdMessage::formatDate(const QDateTime &dt)
{
    return QTdHelpers::formatDate(dt);
}

bool QTdMessage::isValid() const
{
    return m_isValid;
}

bool QTdMessage::sameUserAsPreviousMessage() const
{
    return m_sender_user_id.value() == m_previousSender.value();
}

void QTdMessage::setPreviousSenderId(const qint32 &id)
{
    m_previousSender = id;
    emit previousSenderChanged();

}

bool QTdMessage::sameUserAsNextMessage() const
{
    return m_sender_user_id.value() == m_nextSender.value();
}

void QTdMessage::setNextSenderId(const qint32 &id)
{
    m_nextSender = id;
    emit nextSenderChanged();
}

bool QTdMessage::isLatest() const
{
    // Only the latest message should not have a nextSenderId
    return !m_nextSender.isValid();
}

qint64 QTdMessage::replyToMessageId() const
{
    return m_replyToMessageId.value();
}

QString QTdMessage::qmlReplyToMessageId() const
{
    return m_replyToMessageId.toQmlValue();
}

void QTdMessage::updateSender(const qint32 &senderId)
{
    if (senderId != m_sender_user_id.value()) {
        return;
    }
    if (m_sender) {
        m_sender = Q_NULLPTR;
    }

    auto *users = QTdUsers::instance()->model();
    m_sender = users->getByUid(QString::number(senderId));
    if (m_sender) {
        emit senderChanged();
        if (m_waitingForSender) {
            disconnect(QTdUsers::instance(), &QTdUsers::userCreated, this, &QTdMessage::updateSender);
            m_waitingForSender = false;
        }
        return;
    }
    connect(QTdUsers::instance(), &QTdUsers::userCreated, this, &QTdMessage::updateSender);
    QTdClient::instance()->send(QJsonObject{
                                    {"@type", "getUser"},
                                    {"user_id", m_sender_user_id.value()}
                                });
    m_waitingForSender = true;
}

void QTdMessage::updateSendingState(const QJsonObject &json)
{
    if (json.isEmpty() || !json.contains("sending_state")) {
        return;
    }
    const QJsonObject jsonSendingState = json["sending_state"].toObject();
    const QString type = jsonSendingState["@type"].toString();
    QTdMessageSendingState *obj = Q_NULLPTR;
    if (type == "messageSendingStatePending"){
        obj = new QTdMessageSendingStatePending(this);
    } else {
        qWarning() << "Unknown user status type: " << type;
    }
    m_sendingState = obj;
}

bool QTdMessage::isReply() const
{
    return (m_replyToMessageId.value() > 0);
}

QTdMessage * QTdMessage::messageRepliedTo()
{
    if (replyToMessageId() <= 0) {
        return Q_NULLPTR;
    }

    if (!m_messageRepliedTo) {
        m_messageRepliedTo = new QTdMessage();
    }

    return m_messageRepliedTo;
}

void QTdMessage::handleMessage(const QJsonObject &json)
{
    if (json.isEmpty() || json["id"] != replyToMessageId()) {
        return;
    }

    auto * msgRepliedTo = messageRepliedTo();
    msgRepliedTo->collapse();

    if (!msgRepliedTo) {
        return;
    }

    msgRepliedTo->unmarshalJson(json);
    emit messageRepliedToChanged();
}

bool QTdMessage::isCollapsed() const
{
    return m_isCollapsed;
}

void QTdMessage::collapse()
{
    m_isCollapsed = true;
}
