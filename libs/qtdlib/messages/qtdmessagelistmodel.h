#ifndef QTDMESSAGELISTMODEL_H
#define QTDMESSAGELISTMODEL_H

#include <QObject>
#include <QPointer>
#include "chat/qtdchat.h"
#include "models/QmlObjectListModel.h"
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

public slots:
    void setChat(QTdChat *chat);
    void loadMore();
    void sendMessage(const QString &message);

private slots:
    void cleanUp();
    void handleMessages(const QJsonObject &json);
    void handleUpdateChatLastMessage(const QJsonObject &json);
    void handleUpdateMessageSendSucceeded(const QJsonObject &json);
    void loadMessages(const QJsonValue &fromMsgId,
                      int amount = MESSAGE_LOAD_WINDOW);

  private:
    Q_DISABLE_COPY(QTdMessageListModel)
    QPointer<QQmlObjectListModel<QTdMessage>> m_model;
    QPointer<QTdChat> m_chat;

    void setMessagesRead(QList<qint64> messages);

    int messagesToLoad = -1;
};

#endif // QTDMESSAGELISTMODEL_H
