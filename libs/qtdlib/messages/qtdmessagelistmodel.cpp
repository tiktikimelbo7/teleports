#include "qtdmessagelistmodel.h"
#include <QDebug>
#include <QJsonArray>
#include "client/qtdclient.h"
#include "requests/qtdsendmessagerequest.h"
#include "messages/requests/qtdviewmessagesrequest.h"

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

    QList<qint64> unreadMessages;
    auto oldestMessage = m_chat->lastReadInboxMessageId();
    auto newestMessage = m_chat->lastMessage()->id();
    QJsonArray messages = json["messages"].toArray();
    for (const QJsonValue &msgData : messages)
    {
        const QJsonObject data = msgData.toObject();
        const qint64 mid = qint64(data["id"].toDouble());
        auto *msg = m_model->getByUid(QString::number(mid));
        if (!msg)
        {
            auto *message = new QTdMessage;
            message->unmarshalJson(data);
            m_model->append(message);
            if (mid > oldestMessage && mid <= newestMessage)
                unreadMessages << mid;
        }
    }
    unreadMessages << newestMessage;
    setAllMessagesRead(unreadMessages);
    emit modelChanged();

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
void QTdMessageListModel::loadMessages(const QJsonValue &fromMsgId)
{
    QTdClient::instance()->send(QJsonObject{
                                    {"@type", "getChatHistory"},
                                    {"chat_id", m_chat->jsonId()},
                                    {"from_message_id", fromMsgId},
                                    {"offset", 0},
                                    {"limit", 100},
                                    {"only_local", false},
                                });
}

void QTdMessageListModel::sendMessage(const QString &message)
{
    if (!m_chat) {
        return;
    }
    auto request = new QTdSendMessageRequest();
    request->setChatId(m_chat->id());
    request->setText(message);
    QTdClient::instance()->send(request);
}

void QTdMessageListModel::setAllMessagesRead( QList<qint64> messages)
{
        //TODO: Determine how to detect which messages are in the visible part of the window
        QTdViewMessagesRequest *req = new QTdViewMessagesRequest;
        req->setChatId(m_chat->id());
        req->setMessageIds(messages);
        QTdClient::instance()->send(req);
        req->deleteLater();

}
