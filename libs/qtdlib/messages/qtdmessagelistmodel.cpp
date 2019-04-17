#include "qtdmessagelistmodel.h"
#include <QDebug>
#include <QJsonArray>
#include <QScopedPointer>
#include "client/qtdclient.h"
#include "requests/qtdsendmessagerequest.h"
#include "requests/qtdeditmessagetextrequest.h"
#include "requests/qtdeditmessagecaptionrequest.h"
#include "requests/content/qtdinputmessagetext.h"
#include "requests/content/qtdinputmessagephoto.h"
#include "requests/content/qtdinputmessagedocument.h"
#include "qtdmessagecontentfactory.h"
#include "qtdmessagecontent.h"
#include "messages/requests/qtdviewmessagesrequest.h"
#include "messages/requests/qtddeletemessagesrequest.h"

#include "common/qtdhelpers.h"
#include "utils/await.h"

QTdMessageListModel::QTdMessageListModel(QObject *parent) : QObject(parent),
    m_model(Q_NULLPTR), m_chat(Q_NULLPTR)
{
    m_model = new QQmlObjectListModel<QTdMessage>(this, "", "id");
    connect(QTdClient::instance(), &QTdClient::messages, this, &QTdMessageListModel::handleMessages);
    connect(QTdClient::instance(), &QTdClient::updateChatLastMessage, this, &QTdMessageListModel::handleUpdateChatLastMessage);
    connect(QTdClient::instance(), &QTdClient::updateMessageSendSucceeded, this, &QTdMessageListModel::handleUpdateMessageSendSucceeded);
    connect(QTdClient::instance(), &QTdClient::updateMessageContent, this, &QTdMessageListModel::handleUpdateMessageContent);

}

QTdChat *QTdMessageListModel::chat() const
{
    return m_chat;
}

QObject *QTdMessageListModel::qmlModel() const
{
    return m_model;
}

void QTdMessageListModel::setChat(QTdChat *chat)
{
    if (m_chat == chat)
        return;
    if (m_chat) {
        disconnect(m_chat, &QTdChat::closed, this, &QTdMessageListModel::cleanUp);
    }

    if (!m_model->isEmpty()) {
        cleanUp();
    }

    m_chat = chat;

    if (!m_chat) {
        return;
    }

    if (m_chat) {
        auto *lastMessage = new QTdMessage();
        lastMessage->unmarshalJson(m_chat->lastMessageJson());
        m_model->append(lastMessage);
        connect(m_chat, &QTdChat::closed, this, &QTdMessageListModel::cleanUp);
        loadMessages(m_model->first()->jsonId());
    }
    m_chat->openChat();
    emit chatChanged(m_chat);
}

void QTdMessageListModel::loadMore()
{
    if (!m_chat) {
        return;
    }
    loadMessages(m_model->last()->jsonId());
}

void QTdMessageListModel::cleanUp()
{
    if (m_model->isEmpty()) {
        return;
    }
    m_model->clear();
}

void QTdMessageListModel::handleMessages(const QJsonObject &json)
{

  QJsonArray messages = json["messages"].toArray();
  if (messages.count() == 0) {
      messagesToLoad = -1;
      return;
  }
  QList<qint64> unreadMessages;
  auto oldestMessage = m_chat->lastReadInboxMessageId();
  auto newestMessage = m_chat->lastMessage()->id();
  for (const QJsonValue &msgData : messages)
  {
      const QJsonObject data = msgData.toObject();
      const qint64 mid = qint64(data["id"].toDouble());
      auto *msg = m_model->getByUid(QString::number(mid));
      if (!msg)
      {
          auto *message = new QTdMessage;
          message->unmarshalJson(data);
          if (!m_model->isEmpty())
          {
              auto *last = m_model->last();
              message->setPreviousSenderId(last->senderUserId());
              last->setNextSenderId(message->senderUserId());
              /**
               * If there is a date change between the new and last message
               * then we insert a date message to group messages from the same
               * day
               */
              const QDate lastDate = last->qmlDate().date();
              const QDate newDate = message->qmlDate().date();
              if (lastDate.year() > newDate.year()
                    || lastDate.month() > newDate.month()
                    || lastDate.day() > newDate.day()) {
                  auto *dateMessage = new QTdMessage;
                  dateMessage->unmarshalJson(QJsonObject{
                                                 {"dateLabel", last->date()}
                                             });
                  m_model->append(dateMessage);
              }
          }
          m_model->append(message);
          if (mid > oldestMessage && mid <= newestMessage)
              unreadMessages << mid;
      }
    }
    unreadMessages << newestMessage;
    setMessagesRead(unreadMessages);
    emit modelChanged();
    loadMessages(m_model->last()->jsonId(), messages.count());
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
    const QJsonObject message = json["last_message"].toObject();
    const qint64 mid = qint64(message["id"].toDouble());
    auto *msg = m_model->getByUid(QString::number(mid));
    if (msg) {
        msg->unmarshalJson(message);
        return;
    }
    auto *m = new QTdMessage();

    if (!m_model->isEmpty()) {
        auto *first = m_model->first();
        m->setPreviousSenderId(first->senderUserId());
        first->setNextSenderId(m->senderUserId());

        const QDate firstDate = first->qmlDate().date();
        const QDate newDate = m->qmlDate().date();
        if (firstDate.year() < newDate.year()
              || firstDate.month() < newDate.month()
              || firstDate.day() < newDate.day()) {
            auto *dateMessage = new QTdMessage;
            dateMessage->unmarshalJson(QJsonObject{
                                           {"dateLabel", m->date()}
                                       });
            m_model->append(dateMessage);
        }
    }
    m->unmarshalJson(message);
    m_model->prepend(m);
    QList<qint64> unreadMessages;
    unreadMessages << mid;
    setMessagesRead(unreadMessages);
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

void QTdMessageListModel::handleUpdateMessageContent(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }

    const qint64 messageId = qint64(json["message_id"].toDouble());
    const QJsonObject newContent = json["new_content"].toObject();
    QTdMessage* message = m_model->getByUid(QString::number(messageId));
    if (message == nullptr) {
        return;
    }
    message->unmarshalUpdateContent(newContent);
}

void QTdMessageListModel::loadMessages(const QJsonValue &fromMsgId, int amount)
{
  if (messagesToLoad > -1) {
      messagesToLoad -= amount;
      if (messagesToLoad <= 0) {
        messagesToLoad = -1;
        return;
        }
    } else {
        messagesToLoad = amount;
        qWarning() << "Trying to load " << messagesToLoad << "messages";
    }

    QTdClient::instance()->send(QJsonObject{
        {"@type", "getChatHistory"},
        {"chat_id", m_chat->jsonId()},
        {"from_message_id", fromMsgId},
        {"offset", 0},
        {"limit", messagesToLoad},
        {"only_local", false},
    });
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
        if(isFirstMessage) {
            request->setReplyToMessageId(replyToMessageId);
            isFirstMessage = false;
        }
        currentMessageLength = message.length();
        QTdClient::instance()->send(request.data());
        currentMessagePos += maxMessageLength;
    } while (currentMessagePos < plainText.length());
}
void QTdMessageListModel::sendPhoto(const QString &url, const QString &caption, const qint64 &replyToMessageId)
{
    qDebug() << "send Photo";
    if (!m_chat) {
        return;
    }

    QString plainText;
    QJsonArray formatEntities = QTdHelpers::formatPlainTextMessage(caption, plainText);

    QScopedPointer<QTdSendMessageRequest> request(new QTdSendMessageRequest);
    request->setChatId(m_chat->id());
    QTdInputMessagePhoto *messageContent = new QTdInputMessagePhoto();
    messageContent->setPhoto(url);
    messageContent->setCaption(caption);
    messageContent->setCaptionEntities(formatEntities);
    request->setContent(messageContent);
    request->setReplyToMessageId(replyToMessageId);
    QTdClient::instance()->send(request.data());
}
void QTdMessageListModel::sendDocument(const QString &url, const QString &caption, const qint64 &replyToMessageId)
{
    qDebug() << "send Document";
    if (!m_chat) {
        return;
    }

    QString plainText;
    QJsonArray formatEntities = QTdHelpers::formatPlainTextMessage(caption, plainText);

    QScopedPointer<QTdSendMessageRequest> request(new QTdSendMessageRequest);
    request->setChatId(m_chat->id());
    QTdInputMessageDocument *messageContent = new QTdInputMessageDocument();
    messageContent->setDocument(url);
    messageContent->setCaption(caption);
    messageContent->setCaptionEntities(formatEntities);
    request->setContent(messageContent);
    request->setReplyToMessageId(replyToMessageId);
    QTdClient::instance()->send(request.data());
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

void QTdMessageListModel::deleteMessage(qint64 messageId)
{
    QScopedPointer<QTdDeleteMessagesRequest> req(new QTdDeleteMessagesRequest);
    QList<qint64> messages;
    messages << messageId;
    req->setChatId(m_chat->id());
    req->setMessageIds(messages);
    QTdClient::instance()->send(req.data());
    auto *msgDeleted = m_model->getByUid(QString::number(messageId));
    if (msgDeleted) {
        m_model->remove(msgDeleted);
        return;
    }
}

void QTdMessageListModel::setMessagesRead( QList<qint64> messages)
{
    //TODO: Determine how to detect which messages are in the visible part of the window
    QScopedPointer<QTdViewMessagesRequest> req(new QTdViewMessagesRequest);
    req->setChatId(m_chat->id());
    req->setMessageIds(messages);
    QTdClient::instance()->send(req.data());
}
