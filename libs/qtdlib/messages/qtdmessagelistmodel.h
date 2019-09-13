#ifndef QTDMESSAGELISTMODEL_H
#define QTDMESSAGELISTMODEL_H

#include <QObject>
#include <QPointer>
#include <QDebug>
#include "chat/qtdchat.h"
#include "models/QmlObjectListModel.h"
#include "qtdchatstate.h"
#include "qtdmessage.h"
#include "requests/qtdinputmessagecontent.h"
#include "requests/content/imessageattachmentcontent.h"
#include "requests/content/imessagecaptioncontent.h"

#define MESSAGE_LOAD_WINDOW 60

class QTdMessageListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QTdChat *chat READ chat WRITE setChat NOTIFY chatChanged)
    Q_PROPERTY(QObject *model READ qmlModel NOTIFY modelChanged)

public:
    explicit QTdMessageListModel(QObject *parent = nullptr);

    QTdChat *chat() const;
    QObject *qmlModel() const;

    class QTdAbstractMessageHandler : public QObject
    {
    public:
        virtual ~QTdAbstractMessageHandler() {}
        virtual void handle(QTdMessageListModel &messageListModel, const QJsonArray &messages) const = 0;
        QTdMessage *messageFromJson(const QJsonValue &msgData) const
        {
            const QJsonObject data = msgData.toObject();
            auto *message = new QTdMessage;
            message->unmarshalJson(data);
            return message;
        }
    };

    class QTdNewerMessagesHandler : public QTdAbstractMessageHandler
    {
        void handle(QTdMessageListModel &messageListModel, const QJsonArray &messages) const;
    };

    class QTdOlderMessagesHandler : public QTdAbstractMessageHandler
    {
        void handle(QTdMessageListModel &messageListModel, const QJsonArray &messages) const;
    };

    class QTdUnreadLabelWindowMessageHandler : public QTdAbstractMessageHandler
    {
        void handle(QTdMessageListModel &messageListModel, const QJsonArray &messages) const;
    };

signals:
    void chatChanged(QTdChat *chat);
    void modelChanged();
    void error(const QString &errorMessage);

public slots:
    void setChat(QTdChat *chat);
    void loadNewer();
    void loadOlder();

    void setCaptionProperties(IMessageCaptionContent *contentObj, const QString &caption);
    void setAttachmentProperties(IMessageAttachmentContent *contentObj, const QString &attachmentUrl);
    void prepareAndSendAttachmentMessage(QTdInputMessageContent *contentObj, const qint64 &replyToMessageId);
    void sendMessage(const QString &message, const qint64 &replyToMessageId = 0);
    void sendPhoto(const QString &url, const QString &message, const qint64 &replyToMessageId);
    void sendDocument(const QString &url, const QString &message, const qint64 &replyToMessageId);
    void sendVideo(const QString &url, const QString &message, const qint64 &replyToMessageId);
    void sendAudio(const QString &url, const QString &message, const qint64 &replyToMessageId);
    void sendLocation(const double latitude, const double longitude, const qint32 livePeriod);

    void editMessageText(qint64 messageId, const QString &message);
    void editMessageText(const QString &messageId, const QString &message);
    void editMessageCaption(qint64 messageId, const QString &message);
    void editMessageCaption(const QString &messageId, const QString &message);
    void sendReplyToMessage(const qint64 &replyToMessageId, const QString &message);
    void sendReplyToMessage(const QString &replyToMessageId, const QString &message);
    void deleteMessage(const qint64 messageId);
    void deleteMessages(const QList<qint64> &messageIds);

private slots:
    void cleanUp();
    void handleMessages(const QJsonObject &json);
    void handleUpdateChatLastMessage(const QJsonObject &json);
    void handleUpdateMessageSendSucceeded(const QJsonObject &json);
    void handleUpdateMessageContent(const QJsonObject &json);
    void handleUpdateDeleteMessages(const QJsonObject &json);
    void handleUpdateMessageEdited(const QJsonObject &json);
    void handleUpdateMessageViews(const QJsonObject &json);
    void loadMessages(const QJsonValue &fromMsgId,
                      unsigned int amountBefore,
                      unsigned int amountAfter);

private:
    Q_DISABLE_COPY(QTdMessageListModel)
    QPointer<QQmlObjectListModel<QTdMessage>> m_model;
    QPointer<QTdChat> m_chat;
    QPointer<QTdAbstractMessageHandler> m_messageHandler;
    QTdNewerMessagesHandler newerMessagesHandler;
    QTdOlderMessagesHandler olderMessagesHandler;
    QTdUnreadLabelWindowMessageHandler unreadLabelWindowMessageHandler;
    bool m_isHandleUpdateLastChatMessageConnected;

    QTdMessage *getDateLabelIfNeeded(QTdMessage *firstMessage, QTdMessage *secondMessage);
    void setMessagesRead(QList<qint64> &messages);
    void appendMessage(QTdMessage *message);
    void prependMessage(QTdMessage *message);
    bool isUpToDateAndFollowing() const;
    bool hasNewer() const;
};

#endif // QTDMESSAGELISTMODEL_H
