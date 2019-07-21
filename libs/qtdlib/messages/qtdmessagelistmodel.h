#ifndef QTDMESSAGELISTMODEL_H
#define QTDMESSAGELISTMODEL_H

#include <QObject>
#include <QPointer>
#include <QDebug>
#include <QtPositioning/QGeoPositionInfoSource>
#include "chat/qtdchat.h"
#include "models/QmlObjectListModel.h"
#include "qtdchatstate.h"
#include "qtdmessage.h"

#define MESSAGE_LOAD_WINDOW 100

class QTdMessageListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QTdChat *chat READ chat WRITE setChat NOTIFY chatChanged)
    Q_PROPERTY(QObject *model READ qmlModel NOTIFY modelChanged)
public:
    explicit QTdMessageListModel(QObject *parent = nullptr);

    QTdChat *chat() const;
    QObject *qmlModel() const;

signals:
    void chatChanged(QTdChat *chat);
    void modelChanged();
    void error(const QString &errorMessage);

public slots:
    void setChat(QTdChat *chat);
    void loadMore();
    void sendMessage(const QString &message, const qint64 &replyToMessageId = 0);
    void sendPhoto(const QString &url, const QString &message, const qint64 &replyToMessageId);
    void sendDocument(const QString &url, const QString &message, const qint64 &replyToMessageId);
    void sendLocation();
    void editMessageText(qint64 messageId, const QString &message);
    void editMessageText(const QString &messageId, const QString &message);
    void editMessageCaption(qint64 messageId, const QString &message);
    void editMessageCaption(const QString &messageId, const QString &message);
    void sendReplyToMessage(const qint64 &replyToMessageId, const QString &message);
    void sendReplyToMessage(const QString &replyToMessageId, const QString &message);
    void deleteMessage(qint64 messageId);

private slots:
    void cleanUp();
    void handleMessages(const QJsonObject &json);
    void handleUpdateChatLastMessage(const QJsonObject &json);
    void handleUpdateMessageSendSucceeded(const QJsonObject &json);
    void handleUpdateMessageContent(const QJsonObject &json);
    void loadMessages(const QJsonValue &fromMsgId,
                      int amount = MESSAGE_LOAD_WINDOW);
    void positionUpdated(const QGeoPositionInfo &info);

private:
    Q_DISABLE_COPY(QTdMessageListModel)
    QPointer<QQmlObjectListModel<QTdMessage>> m_model;
    QPointer<QTdChat> m_chat;
    QPointer<QGeoPositionInfoSource> positionInfoSource;
   
    void setMessagesRead(QList<qint64> messages);

    int messagesToLoad = -1;
};

#endif // QTDMESSAGELISTMODEL_H
