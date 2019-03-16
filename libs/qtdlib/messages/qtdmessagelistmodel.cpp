#include "qtdmessagelistmodel.h"
#include <QDebug>
#include <QJsonArray>
#include <QScopedPointer>
#include <QtPositioning/QGeoCoordinate>
#include <QTimer>
#include "client/qtdclient.h"
#include "requests/qtdsendmessagerequest.h"
#include "requests/qtdeditmessagetextrequest.h"
#include "requests/qtdeditmessagecaptionrequest.h"
#include "requests/content/qtdinputmessagetext.h"
#include "requests/content/qtdinputmessagephoto.h"
#include "requests/content/qtdinputmessagevideo.h"
#include "requests/content/qtdinputmessageaudio.h"
#include "requests/content/qtdinputmessagedocument.h"
#include "requests/content/qtdinputmessageaudio.h"
#include "requests/content/qtdinputmessagevideo.h"
#include "requests/content/qtdinputmessagelocation.h"
#include "qtdmessagecontentfactory.h"
#include "qtdmessagecontent.h"
#include "messages/requests/qtdviewmessagesrequest.h"
#include "messages/requests/qtddeletemessagesrequest.h"
#include "utils/i18n.h"
#include "messages/requests/qtdpinmessagerequest.h"
#include "common/qtdhelpers.h"
#include "utils/await.h"
#include "requests/content/imessageattachmentcontent.h"
#include "requests/content/imessagecaptioncontent.h"

QTdMessageListModel::QTdMessageListModel(QObject *parent)
    : QObject(parent)
    , m_model(Q_NULLPTR)
    , m_chat(Q_NULLPTR)
    , m_messageHandler(Q_NULLPTR)
    , m_jumpToMessageId("") 
    , m_isHandleUpdateLastChatMessageConnected(false)
{
    m_model = new QQmlObjectListModel<QTdMessage>(this, "", "id");
    connect(QTdClient::instance(), &QTdClient::messages, this, &QTdMessageListModel::handleMessages);
    connect(QTdClient::instance(), &QTdClient::updateMessageSendSucceeded, this, &QTdMessageListModel::handleUpdateMessageSendSucceeded);
    connect(QTdClient::instance(), &QTdClient::updateMessageContent, this, &QTdMessageListModel::handleUpdateMessageContent);
    connect(QTdClient::instance(), &QTdClient::updateDeleteMessages, this, &QTdMessageListModel::handleUpdateDeleteMessages);
    connect(QTdClient::instance(), &QTdClient::updateMessageEdited, this, &QTdMessageListModel::handleUpdateMessageEdited);
    connect(QTdClient::instance(), &QTdClient::updateMessageViews, this, &QTdMessageListModel::handleUpdateMessageViews);
}

QTdChat *QTdMessageListModel::chat() const
{
    return m_chat;
}

QObject *QTdMessageListModel::qmlModel() const
{
    return m_model;
}

bool QTdMessageListModel::isUpToDateAndFollowing() const
{
    return m_isHandleUpdateLastChatMessageConnected;
}

bool QTdMessageListModel::hasNewer() const
{
    // when setting messages read, model is already up to date, but hasUnreadMessages still true
    if (!m_chat->hasUnreadMessages())
    {
        return false;
    }

    if (m_model->isEmpty())
    {
        return false;
    }

    // this fails if the first message is a date label, which achtually should never be the case
    return m_chat->lastMessage()->id() != m_model->first()->id();
}

void QTdMessageListModel::setChat(QTdChat *chat)
{
    if (m_chat == chat)
        return;
    if (m_chat)
    {
        disconnect(m_chat, &QTdChat::closed, this, &QTdMessageListModel::cleanUp);
    }

    if (!m_model->isEmpty())
    {
        cleanUp();
    }

    m_chat = chat;

    if (!m_chat) {
        return;
    }

    if (m_chat) {
        connect(m_chat, &QTdChat::closed, this, &QTdMessageListModel::cleanUp);
        if (m_chat->hasUnreadMessages()) {

            m_jumpToMessageId = m_chat->qmlLastReadInboxMessageId();
            m_messageHandler = &jumpToWindowMessageHandler;
            loadMessages(m_chat->qmlLastReadInboxMessageId(), MESSAGE_LOAD_WINDOW / 2, MESSAGE_LOAD_WINDOW / 2);
        } else {
            m_messageHandler = &olderMessagesHandler;
            QScopedPointer<QTdMessage> lastMessage(new QTdMessage());
            lastMessage->unmarshalJson(m_chat->lastMessageJson());
            loadMessages(lastMessage->jsonId(), MESSAGE_LOAD_WINDOW, 1);
            connect(QTdClient::instance(), &QTdClient::updateChatLastMessage, this, &QTdMessageListModel::handleUpdateChatLastMessage);
        }
    }
    m_chat->openChat();
    emit chatChanged(m_chat);
}

void QTdMessageListModel::loadNewer()
{
    if (!m_chat) {
        return;
    }
    if (isUpToDateAndFollowing()) {
        return;
    }
    if (m_messageHandler) {
        return;
    }
    m_messageHandler = &newerMessagesHandler;
    loadMessages(m_model->first()->jsonId(), 0, MESSAGE_LOAD_WINDOW);
}

void QTdMessageListModel::loadOlder()
{
    if (!m_chat) {
        return;
    }
    if (m_messageHandler) {
        return;
    }
    m_messageHandler = &olderMessagesHandler;
    loadMessages(m_model->last()->jsonId(), MESSAGE_LOAD_WINDOW, 0);
}

void QTdMessageListModel::cleanUp()
{
    m_isHandleUpdateLastChatMessageConnected = false;
    disconnect(QTdClient::instance(), &QTdClient::updateChatLastMessage, this, &QTdMessageListModel::handleUpdateChatLastMessage);
    if (m_model->isEmpty()) {
        return;
    }
    m_model->clear();
}

void QTdMessageListModel::handleMessages(const QJsonObject &json)
{
    QJsonArray messages = json["messages"].toArray();
    if (messages.count() == 0) {
        if (m_model->count() > 0 && m_model->last()->id() != 0) {
            auto dateMessage = getDateLabelIfNeeded(m_model->last(), Q_NULLPTR);
            if (dateMessage) {
                m_model->append(dateMessage);
            }
        }
        m_messageHandler = Q_NULLPTR;
        return;
    }

    if (!m_messageHandler) {
        return;
    }

    m_messageHandler->handle(*this, messages);

    emit modelChanged();

    if (m_model->count() < MESSAGE_LOAD_WINDOW) {
        m_messageHandler = &olderMessagesHandler;
        qint64 lastValidMessageId;
        QJsonValue(jsonMessageId);
        auto i = m_model->count() - 1;
        do {
            lastValidMessageId = m_model->at(i)->id();
            jsonMessageId = m_model->at(i)->jsonId();
        } while (--i > 0 && lastValidMessageId == 0);
        loadMessages(jsonMessageId, MESSAGE_LOAD_WINDOW, 0);
    } else {
        m_messageHandler = Q_NULLPTR;
    }

    if (!hasNewer() && !m_isHandleUpdateLastChatMessageConnected) {
        connect(QTdClient::instance(), &QTdClient::updateChatLastMessage, this, &QTdMessageListModel::handleUpdateChatLastMessage);
        m_isHandleUpdateLastChatMessageConnected = true;
        m_chat->positionMessageListViewAtIndex(-1);
    }
}

void QTdMessageListModel::QTdOlderMessagesHandler::handle(QTdMessageListModel &messageListModel, const QJsonArray &messages) const
{
    for (unsigned int index = 0; index < messages.count(); index++) {
        auto *message = messageFromJson(messages[index]);
        messageListModel.appendMessage(message);
    }
}

void QTdMessageListModel::QTdNewerMessagesHandler::handle(QTdMessageListModel &messageListModel, const QJsonArray &messages) const
{
    QList<qint64> unreadMessages;

    for (int index = messages.count() - 1; index >= 0; index--) {
        auto *message = messageFromJson(messages[index]);
        messageListModel.prependMessage(message);
        unreadMessages << message->id();
    }

    messageListModel.setMessagesRead(unreadMessages);
}

void QTdMessageListModel::QTdJumpToWindowMessageHandler::handle(QTdMessageListModel &messageListModel, const QJsonArray &messages) const
{
    QList<qint64> unreadMessages;

    for (unsigned int index = 0; index < messages.count(); index++) {
        auto *message = messageFromJson(messages[index]);
        if (message->id() == messageListModel.m_chat->lastReadInboxMessageId() &&
            message->id() != messageListModel.m_chat->lastMessage()->id()) {
            auto *unreadLabel = new QTdMessage;
            unreadLabel->unmarshalJson(QJsonObject{ { "unreadLabel", gettext("Unread Messages") } });
            messageListModel.m_model->append(unreadLabel);
        }

        messageListModel.appendMessage(message);
        unreadMessages << message->id();
    }

    int jumpToMessageIndex = messageListModel.m_model->indexOf(messageListModel.m_jumpToMessageId); 
    messageListModel.m_chat->positionMessageListViewAtIndex(jumpToMessageIndex+1);
    messageListModel.setMessagesRead(unreadMessages);
}

QTdMessage *QTdMessageListModel::getDateLabelIfNeeded(QTdMessage *firstMessage, QTdMessage *secondMessage)
{
    const QDate firstDate = firstMessage->qmlDate().date();
    const QDate secondDate = secondMessage
            ? secondMessage->qmlDate().date()
            : QDate();
    if (firstDate.year() > secondDate.year()
        || firstDate.month() > secondDate.month()
        || firstDate.day() > secondDate.day()) {
        auto *dateMessage = new QTdMessage;
        dateMessage->unmarshalJson(QJsonObject{ { "dateLabel", firstMessage->date() } });
        return dateMessage;
    } else {
        return Q_NULLPTR;
    }
}

void QTdMessageListModel::appendMessage(QTdMessage *message)
{
    if (m_model->isEmpty()) {
        m_model->append(message);
        return;
    }
    if (m_model->getByUid(message->qmlId())) {
        return;
    }
    auto *last = m_model->last();
    message->setPreviousSenderId(last->senderUserId());
    last->setNextSenderId(message->senderUserId());

    if (!message->sendingState()) {
        auto dateMessage = getDateLabelIfNeeded(last, message);
        if (dateMessage) {
            m_model->append(dateMessage);
        }
    }

    m_model->append(message);
}

void QTdMessageListModel::prependMessage(QTdMessage *message)
{
    if (m_model->isEmpty()) {
        m_model->prepend(message);
        return;
    }
    if (m_model->getByUid(message->qmlId())) {
        return;
    }
    auto *first = m_model->first();
    message->setNextSenderId(first->senderUserId());
    first->setPreviousSenderId(message->senderUserId());

    if (!message->sendingState()) {
        auto dateMessage = getDateLabelIfNeeded(message, first);
        if (dateMessage) {
            m_model->prepend(dateMessage);
        }
    }

    m_model->prepend(message);
}

void QTdMessageListModel::loadMessages(const QJsonValue &fromMsgId, unsigned int amountOlder, unsigned int amountNewer)
{
    QTdClient::instance()->send(QJsonObject{
            { "@type", "getChatHistory" },
            { "chat_id", m_chat->jsonId() },
            { "from_message_id", fromMsgId },
            { "offset", static_cast<int>(-amountNewer) },
            { "limit", static_cast<int>(amountOlder + amountNewer + 1) },
            { "only_local", false },
    });
}

void QTdMessageListModel::handleUpdateChatLastMessage(const QJsonObject &json)
{
    if (!m_chat || json.isEmpty()) {
        return;
    }

    const qint64 id = qint64(json["chat_id"].toDouble());
    if (id != m_chat->id()) {
        return;
    }

    const QJsonObject messageJson = json["last_message"].toObject();

    const qint64 messageId = qint64(messageJson["id"].toDouble());
    if (!m_model->isEmpty() && m_model->getByUid(QString::number(messageId))) {
        return;
    }

    auto *message = new QTdMessage();
    message->unmarshalJson(messageJson);
    prependMessage(message);

    QList<qint64> unreadMessages;
    unreadMessages << messageId;
    setMessagesRead(unreadMessages);
    unreadMessages.clear();

    emit modelChanged();
}

void QTdMessageListModel::handleUpdateMessageSendSucceeded(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }
    const qint64 oldMid = qint64(json["old_message_id"].toDouble());
    auto *msgSent = m_model->getByUid(QString::number(oldMid));
    const QJsonObject message = json["message"].toObject();
    if (msgSent) {
        m_model->remove(msgSent);
        return;
    }
}

void QTdMessageListModel::handleUpdateMessageEdited(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }
    auto messageId = QString::number(json["message_id"].toDouble(), 'f', 0);
    QTdMessage *message = m_model->getByUid(messageId);
    if (message == nullptr) {
        return;
    }
    message->setIsEdited(true);
}

void QTdMessageListModel::handleUpdateMessageViews(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }
    auto messageId = QString::number(json["message_id"].toDouble(), 'f', 0);
    QTdMessage *message = m_model->getByUid(messageId);
    if (message == nullptr) {
        return;
    }
    auto views = (qint32)json["views"].toDouble();
    message->setViews(views);
}

void QTdMessageListModel::handleUpdateDeleteMessages(const QJsonObject &json)
{
    if (json.isEmpty() || json["from_cache"].toBool()) {
        return;
    }
    const QJsonArray messagesToDelete = json["message_ids"].toArray();
    foreach (QJsonValue messageToDelete, messagesToDelete) {
        auto messageId = QString::number(messageToDelete.toDouble(), 'f', 0);
        QTdMessage *message = m_model->getByUid(messageId);
        if (message == nullptr) {
            continue;
        }
        m_model->remove(message);
    }
}

void QTdMessageListModel::handleUpdateMessageContent(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }

    const qint64 messageId = qint64(json["message_id"].toDouble());
    const QJsonObject newContent = json["new_content"].toObject();
    QTdMessage *message = m_model->getByUid(QString::number(messageId));
    if (message == nullptr) {
        return;
    }
    message->unmarshalUpdateContent(newContent);
}

void QTdMessageListModel::sendMessage(const QString &fullmessage, const qint64 &replyToMessageId)
{
    if (!m_chat) {
        return;
    }
    QString plainText;
    QJsonArray formatEntities = QTdHelpers::formatPlainTextMessage(fullmessage, plainText);
    bool isFirstMessage = true;
    qint32 currentMessagePos = 0;
    qint32 currentMessageLength = 0;
    qint32 maxMessageLength = QTdClient::instance()->getOption("message_text_length_max").toInt();
    do {
        QString message = plainText.mid(currentMessagePos, maxMessageLength);
        QScopedPointer<QTdSendMessageRequest> request(new QTdSendMessageRequest);
        request->setChatId(m_chat->id());
        QTdInputMessageText *messageText = new QTdInputMessageText();
        messageText->setText(message);
        messageText->setEntities(formatEntities);
        request->setContent(messageText);
        if (isFirstMessage) {
            request->setReplyToMessageId(replyToMessageId);
            isFirstMessage = false;
        }
        currentMessageLength = message.length();
        QTdClient::instance()->send(request.data());
        currentMessagePos += maxMessageLength;
    } while (currentMessagePos < plainText.length());
}

void QTdMessageListModel::setCaptionProperties(IMessageCaptionContent *contentObj, const QString &caption) {
    if (contentObj != nullptr) {
        QString plainText;
        QJsonArray formatEntities = QTdHelpers::formatPlainTextMessage(caption, plainText);
        contentObj->setCaption(plainText);
        contentObj->setCaptionEntities(formatEntities);
    }
}

void QTdMessageListModel::setAttachmentProperties(IMessageAttachmentContent *contentObj, const QString &attachmentUrl) {
    if (contentObj != nullptr) {
        contentObj->setAttachmentPath(attachmentUrl);
    }
}

void QTdMessageListModel::prepareAndSendAttachmentMessage(QTdInputMessageContent *content, const qint64 &replyToMessageId) {
    if (!m_chat) {
        return;
    }
    QScopedPointer<QTdSendMessageRequest> request(new QTdSendMessageRequest);
    request->setChatId(m_chat->id());
    request->setContent(content);
    request->setReplyToMessageId(replyToMessageId);
    QTdClient::instance()->send(request.data());
}

void QTdMessageListModel::sendPhoto(const QString &url, const QString &caption, const qint64 &replyToMessageId)
{
    QScopedPointer<QTdInputMessagePhoto> messageContent(new QTdInputMessagePhoto);
    setAttachmentProperties(messageContent.data(), url);
    setCaptionProperties(messageContent.data(), caption);
    prepareAndSendAttachmentMessage(messageContent.data(), replyToMessageId);
}

void QTdMessageListModel::sendVideo(const QString &url, const QString &caption, const qint64 &replyToMessageId)
{
    QScopedPointer<QTdInputMessageVideo> messageContent(new QTdInputMessageVideo);
    setAttachmentProperties(messageContent.data(), url);
    setCaptionProperties(messageContent.data(), caption);
    prepareAndSendAttachmentMessage(messageContent.data(), replyToMessageId);
}

void QTdMessageListModel::sendAudio(const QString &url, const QString &caption, const qint64 &replyToMessageId)
{
    QScopedPointer<QTdInputMessageAudio> messageContent(new QTdInputMessageAudio);
    setAttachmentProperties(messageContent.data(), url);
    setCaptionProperties(messageContent.data(), caption);
    prepareAndSendAttachmentMessage(messageContent.data(), replyToMessageId);
}

void QTdMessageListModel::sendDocument(const QString &url, const QString &caption, const qint64 &replyToMessageId)
{
    QScopedPointer<QTdInputMessageDocument> messageContent(new QTdInputMessageDocument);
    setAttachmentProperties(messageContent.data(), url);
    setCaptionProperties(messageContent.data(), caption);
    prepareAndSendAttachmentMessage(messageContent.data(), replyToMessageId);
}

void QTdMessageListModel::sendLocation(const double latitude, const double longitude, const qint32 livePeriod)
{
    QScopedPointer<QTdInputMessageLocation> messageContent(new QTdInputMessageLocation);
    messageContent->setLocation(latitude, longitude);
    messageContent->setLivePeriod(livePeriod);
    prepareAndSendAttachmentMessage(messageContent.data(), 0);
}

void QTdMessageListModel::editMessageText(qint64 messageId, const QString &message)
{
    if (!m_chat) {
        return;
    }

    QString plainText;
    QJsonArray formatEntities = QTdHelpers::formatPlainTextMessage(message, plainText);

    QScopedPointer<QTdEditMessageTextRequest> request(new QTdEditMessageTextRequest);
    request->setChatId(m_chat->id());
    request->setMessageId(messageId);
    request->setText(plainText);
    request->setEntities(formatEntities);

    QFuture<QTdResponse> response = request->sendAsync();
    await(response);
    if (response.result().isError()) {
        emit error(response.result().errorString());
    }
}

void QTdMessageListModel::editMessageText(const QString &messageId, const QString &message)
{
    editMessageText(messageId.toLongLong(), message);
}

void QTdMessageListModel::editMessageCaption(qint64 messageId, const QString &message)
{
    if (!m_chat) {
        return;
    }

    QString plainText;
    QJsonArray formatEntities = QTdHelpers::formatPlainTextMessage(message, plainText);

    QScopedPointer<QTdEditMessageCaptionRequest> request(new QTdEditMessageCaptionRequest);
    request->setChatId(m_chat->id());
    request->setMessageId(messageId);
    request->setText(plainText);
    request->setEntities(formatEntities);

    QFuture<QTdResponse> response = request->sendAsync();
    await(response);
    if (response.result().isError()) {
        emit error(response.result().errorString());
    }
}

void QTdMessageListModel::editMessageCaption(const QString &messageId, const QString &message)
{
    editMessageCaption(messageId.toLongLong(), message);
}

void QTdMessageListModel::sendReplyToMessage(const qint64 &replyToMessageId, const QString &message)
{
    sendMessage(message, replyToMessageId);
}

void QTdMessageListModel::sendReplyToMessage(const QString &replyToMessageId, const QString &message)
{
    sendMessage(message, replyToMessageId.toLongLong());
}

void QTdMessageListModel::deleteMessage(const qint64 messageId)
{
    QList<qint64> messages;
    messages << messageId;
    deleteMessages(messages);
}

void QTdMessageListModel::deleteMessages(const QList<qint64> &messageIds)
{
    QScopedPointer<QTdDeleteMessagesRequest> req(new QTdDeleteMessagesRequest);
    req->setChatId(m_chat->id());
    req->setMessageIds(messageIds);
    QTdClient::instance()->send(req.data());
}

void QTdMessageListModel::setMessagesRead(QList<qint64> &messages)
{
    //TODO: Determine how to detect which messages are in the visible part of the window
    QScopedPointer<QTdViewMessagesRequest> req(new QTdViewMessagesRequest);
    req->setChatId(m_chat->id());
    req->setMessageIds(messages);
    QTdClient::instance()->send(req.data());
    if(messages.contains(m_chat->lastMessage()->id())) {
        QTdClient::instance()->clearNotificationFor(m_chat->id());
    }
}

void QTdMessageListModel::jumpToMessage(const QString &messageId)
{
    cleanUp();
    m_chat->positionMessageListViewAtIndex(-1);
    emit modelChanged();
    m_jumpToMessageId = messageId;
    m_messageHandler = &jumpToWindowMessageHandler;
    loadMessages(messageId, MESSAGE_LOAD_WINDOW / 2, MESSAGE_LOAD_WINDOW / 2);

void QTdMessageListModel::pinMessage(const QString &superGroupId, const QString &messageId)
{
    pinMessage(superGroupId.toInt(), messageId.toLongLong());
}

void QTdMessageListModel::pinMessage(const qint32 &superGroupId, const qint64 &messageId)
{
    QScopedPointer<QTdPinMessageRequest> req(new QTdPinMessageRequest);
    qDebug() << "pinning message with id: " << messageId << " in group with id: " << superGroupId;
    req->setSuperGroupId(superGroupId);
    req->setMessageId(messageId);
    QTdClient::instance()->send(req.data());
}

