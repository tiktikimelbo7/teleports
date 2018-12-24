#include "qtdmessagelistmodel.h"
#include <QDebug>
#include <QJsonArray>
#include <QScopedPointer>
#include "client/qtdclient.h"
#include "requests/qtdsendmessagerequest.h"
#include "messages/requests/qtdviewmessagesrequest.h"
#include "common/qtdhelpers.h"

QTdMessageListModel::QTdMessageListModel(QObject *parent) : QObject(parent),
    m_model(Q_NULLPTR), m_chat(Q_NULLPTR)
{
    m_model = new QQmlObjectListModel<QTdMessage>(this, "", "id");
    connect(QTdClient::instance(), &QTdClient::messages, this, &QTdMessageListModel::handleMessages);
    connect(QTdClient::instance(), &QTdClient::updateChatLastMessage, this, &QTdMessageListModel::handleUpdateChatLastMessage);
    connect(QTdClient::instance(), &QTdClient::updateMessageSendSucceeded, this, &QTdMessageListModel::handleUpdateMessageSendSucceeded);
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
    setAllMessagesRead(unreadMessages);
    emit modelChanged();
    loadMessages(m_model->last()->jsonId(), messages.count());
}

void QTdMessageListModel::handleUpdateNewMessage(const QJsonObject &json)
{
    if (!m_chat || json.isEmpty())
    {
        return;
    }
    const qint64 id = qint64(json["chat_id"].toDouble());
    if (id != m_chat->id())
    {
        return;
    }
    QList<qint64> unreadMessages;
    const QJsonObject data = json["message"].toObject();
    const qint64 mid = qint64(data["id"].toDouble());
    unreadMessages << mid;
    setAllMessagesRead(unreadMessages);
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

void QTdMessageListModel::sendMessage(const QString &message)
{
    if (!m_chat) {
        return;
    }

    //First call tdlib to markup all complex entities
    auto parseRequest = QJsonObject {
        {"@type", "getTextEntities"},
        {"text", message}
    };
    auto result = QTdClient::instance()->exec(parseRequest);
    result.waitForFinished();
    auto entities = result.result()["entities"].toArray();

    //Then do the text formatting
    QString plainText;
    QJsonArray formatEntities;
    QTdHelpers::getEntitiesFromMessage(message, plainText, entities);
    QScopedPointer<QTdSendMessageRequest> request(new QTdSendMessageRequest);
    request->setChatId(m_chat->id());
    request->setText(plainText);
    formatEntities << entities;
    request->setEntities(entities);
    QTdClient::instance()->send(request.data());
}

void QTdMessageListModel::setAllMessagesRead( QList<qint64> messages)
{
    //TODO: Determine how to detect which messages are in the visible part of the window
    QScopedPointer<QTdViewMessagesRequest> req(new QTdViewMessagesRequest);
    req->setChatId(m_chat->id());
    req->setMessageIds(messages);
    QTdClient::instance()->send(req.data());
}
