#include "qtdmessage.h"
#include <QDebug>
#include "client/qtdclient.h"
#include "user/qtdusers.h"
#include "qtdmessagecontentfactory.h"
#include "content/qtdmessagecall.h"
#include "content/qtdmessagetext.h"
#include "content/qtdmessagedate.h"
#include "content/qtdmessageunreadlabel.h"
#include "content/qtdmessagesticker.h"
#include "content/qtdmessagedocument.h"
#include "content/qtdmessagelocation.h"
#include "content/qtdmessagecustomserviceaction.h"
#include "content/qtdmessagechatdeletemember.h"
#include "content/qtdmessagechataddmembers.h"
#include "common/qtdhelpers.h"
#include "requests/qtdgetmessagerequest.h"
#include "user/requests/qtdgetuserrequest.h"
#include "chat/requests/qtdgetchatrequest.h"
#include "chat/qtdchat.h"
#include "utils/await.h"
#include "utils/i18n.h"

QTdMessage::QTdMessage(QObject *parent)
    : QAbstractInt64Id(parent)
    , m_date(0)
    , m_chatId(0)
    , m_sender(Q_NULLPTR)
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
    , m_previousSender(Q_NULLPTR)
    , m_nextSender(Q_NULLPTR)
    , m_replyMarkup(Q_NULLPTR)
    , m_forwardInfo(Q_NULLPTR)
    , m_interactionInfo(Q_NULLPTR)
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

QString QTdMessage::qmlChatId() const
{
    return m_chatId.toQmlValue();
}

qint64 QTdMessage::chatId() const
{
    return m_chatId.value();
}

QTdMessageSender *QTdMessage::sender() const
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

    const QJsonObject sender = json["sender"].toObject();
    m_sender = QTdMessageSender::create(sender, this);
    m_sender->unmarshalJson(sender);

    m_chatId = json["chat_id"].toVariant().toLongLong();

    updateSendingState(json);

    m_isOutgoing = json["is_outgoing"].toBool();
    m_isEdited = qint32(json["edit_date"].toInt()) > 0;
    m_canBeEdited = json["can_be_edited"].toBool();
    m_canBeForwarded = json["can_be_forwarded"].toBool();
    m_canBeDeletedOnlyForSelf = json["can_be_deleted_only_for_self"].toBool();
    m_canBeDeletedForAllUsers = json["can_be_deleted_for_all_users"].toBool();
    m_isChannelPost = json["is_channel_post"].toBool();
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
    auto temp_content = QTdMessageContentFactory::create(content, this);
    temp_content->unmarshalJson(content);
    switch (temp_content->type()) {
    case QTdObject::MESSAGE_CALL: {
        auto *c = qobject_cast<QTdMessageCall *>(temp_content);
        c->setOutgoing(m_isOutgoing);
        temp_content = qobject_cast<QTdMessageContent *>(c);
        break;
    }
    case QTdObject::MESSAGE_CHAT_ADD_MEMBERS: {
        auto *c = qobject_cast<QTdMessageChatAddMembers *>(temp_content);
        c->setSenderUserId(m_sender->id());
        temp_content = qobject_cast<QTdMessageContent *>(c);
        break;
    }
    case QTdObject::MESSAGE_CHAT_DELETE_MEMBER: {
        auto *c = qobject_cast<QTdMessageChatDeleteMember *>(temp_content);
        c->setSenderUserId(m_sender->id());
        temp_content = qobject_cast<QTdMessageContent *>(c);
        break;
    }
    }
    m_content = temp_content;

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

    if (m_interactionInfo) {
        delete m_interactionInfo;
        m_interactionInfo = nullptr;
    }
    const QJsonObject interactionInfo = json["interaction_info"].toObject();
    if (!interactionInfo.isEmpty()) {
        m_interactionInfo = new QTdMessageInteractionInfo(this);
        m_interactionInfo->unmarshalJson(interactionInfo);
    }

    if (m_forwardInfo) {
        delete m_forwardInfo;
        m_forwardInfo = nullptr;
    }
    qint64 forwardedFromId = 0;
    const QJsonObject forwardInfo = json["forward_info"].toObject();
    if (!forwardInfo.isEmpty()) {
        m_forwardInfo = new QTdMessageForwardInfo(this);
        m_forwardInfo->unmarshalJson(forwardInfo);
    }
    m_ttl = json["ttl"].toInt();
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
    return m_isOutgoing && !m_isChannelPost;
}

bool QTdMessage::isEdited() const
{
    return m_isEdited;
}

void QTdMessage::setIsEdited(const bool value)
{
    m_isEdited = value;
    emit messageChanged();
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

qint32 QTdMessage::ttl() const
{
    return m_ttl;
}

QString QTdMessage::views() const
{
    if (!m_interactionInfo) {
        return QString("0");
    }

    qint32 viewCount = m_interactionInfo->viewCount();

    if (viewCount > 9999 && viewCount <= 999999) {
        return QString("%1K").arg(((double)(viewCount / 100)) / 10, 0, 'd', 1);
    } else if (viewCount > 999999) {
        return QString("%1M").arg(((double)(viewCount / 100000)) / 10, 0, 'd', 1);
    } else {
        return QString("%1").arg(viewCount);
    }
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

QTdMessageForwardInfo *QTdMessage::forwardInfo() const
{
    return m_forwardInfo;
}

QTdMessageInteractionInfo *QTdMessage::interactionInfo() const
{
    return m_interactionInfo;
}

bool QTdMessage::isForwarded() const
{
    return m_forwardInfo != nullptr;
}

QString QTdMessage::summary() const
{
    QString content;
    if (!m_content.isNull())
    {
        if (m_content->typeText() != "") {
            content = QString("%1 %2").arg(m_content->typeText(), m_content->infoText());
        } else {
            content = m_content->infoText();
        }
    }
    return content;
}

QString QTdMessage::formatDate(const QDateTime &dt)
{
    return QTdHelpers::formatDate(dt);
}

bool QTdMessage::isValid() const
{
    return m_isValid;
}

bool QTdMessage::sameSenderAsPreviousMessage() const
{
    return !m_previousSender.isNull() && m_sender->type() == m_previousSender->type() && m_sender->id() == m_previousSender->id();
}

void QTdMessage::setPreviousSender(QTdMessageSender *sender)
{
    m_previousSender = sender;
    emit previousSenderChanged();
}

bool QTdMessage::sameSenderAsNextMessage() const
{
    return !m_nextSender.isNull() && m_sender->type() == m_nextSender->type() && m_sender->id() == m_nextSender->id();
}

void QTdMessage::setNextSender(QTdMessageSender *sender)
{
    m_nextSender = sender;
    emit nextSenderChanged();
}

bool QTdMessage::isLatest() const
{
    // Only the latest message should not have a nextSender
    return m_nextSender.isNull();
}

qint64 QTdMessage::replyToMessageId() const
{
    return m_replyToMessageId.value();
}

QString QTdMessage::qmlReplyToMessageId() const
{
    return m_replyToMessageId.toQmlValue();
}

void QTdMessage::updateSendingState(const QJsonObject &json)
{
    if (json.isEmpty() || !json.contains("sending_state")) {
        return;
    }
    const QJsonObject jsonSendingState = json["sending_state"].toObject();
    const QString type = jsonSendingState["@type"].toString();
    QTdMessageSendingState *obj = Q_NULLPTR;
    if (type == "messageSendingStatePending") {
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

QTdMessage *QTdMessage::messageRepliedTo()
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

    auto *msgRepliedTo = messageRepliedTo();
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
