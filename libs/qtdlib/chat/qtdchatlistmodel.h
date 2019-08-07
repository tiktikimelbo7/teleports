#ifndef QTDCHATLISTMODEL_H
#define QTDCHATLISTMODEL_H

#include <QObject>
#include <QPointer>
#include "auth/qtdauthstate.h"
#include "models/QmlObjectListModel.h"
#include "qtdchat.h"

typedef QList<qint64> PinnedChats;

class QTdChatListModel : public QObject
{

    Q_OBJECT
    Q_PROPERTY(QObject* model READ model NOTIFY modelChanged)
    Q_PROPERTY(QTdChat* currentChat READ currentChat WRITE setCurrentChat NOTIFY currentChatChanged)
    Q_PROPERTY(ListMode listMode READ listMode WRITE setListMode NOTIFY listModeChanged)
    Q_PROPERTY(qint32 forwardingMessagesCount READ forwardingMessagesCount NOTIFY modelChanged)
    Q_PROPERTY(QTdChat* forwardedFromChat READ forwardedFromChat  WRITE setForwardedFromChat NOTIFY modelChanged)
    Q_PROPERTY(QStringList forwardingMessages READ forwardingMessages WRITE setForwardingMessages NOTIFY modelChanged)

public:
    enum ListMode { Idle, ForwardingMessages, ImportingAttachments };
    Q_ENUM(ListMode)

    explicit QTdChatListModel(QObject *parent = nullptr);

    QObject* model() const;
    QTdChat *currentChat() const;
    QTdChat *chatById(const qint64 &chatId) const;
    QTdChat *forwardedFromChat() const;
    qint32 forwardingMessagesCount() const;
    ListMode listMode() const;
    QStringList forwardingMessages() const;

public slots:
    void setCurrentChat(QTdChat* currentChat);
    void setCurrentChatById(const int &chatId);
    void setForwardedFromChat(QTdChat *currentChat);
    void setForwardingMessages(QStringList forwardingMessages);
    void setListMode(ListMode listMode);
    void clearCurrentChat();
    void sendForwardMessage(const QStringList  &forwardMessageIds,
                            const qint64 &recievingChatId,
                            const qint64 &fromChatId,
                            const QString &message);

signals:
    void modelChanged(QObject* model);
    void contentsChanged();
    void chatStatusChanged();
    void currentChatChanged(QTdChat* currentChat);
    void listModeChanged();

private slots:
    void handleUpdateNewChat(const QJsonObject &chat);
    void handleUpdateChatOrder(const QJsonObject &json);
    void handleUpdateChatLastMessage(const QJsonObject chat);
    void handleAuthStateChanges(const QTdAuthState *state);
    void updateChatReadInbox(const QJsonObject &json);
    void updateChatReadOutbox(const QJsonObject &json);
    void handleUpdateChatIsPinned(const QJsonObject &json);
    void handleUpdateChatPhoto(const QJsonObject &chat);
    void handleUpdateChatReplyMarkup(const QJsonObject &chat);
    void handleUpdateChatTitle(const QJsonObject &chat);
    void handleUpdateChatUnreadMentionCount(const QJsonObject &chat);
    void handleUpdateChatNotificationSettings(const QJsonObject &chat);
    void handleForwardingMessagesAction();

    /**
     * @brief Set the pinned chats for the user
     *
     * We maintain an internal list m_pinnedChats which get's updated on
     * changes via updateChatIsPinned events.
     */
    void handlePinChatAction(const qint64 &chatId, const bool &pinned);
private:
    Q_DISABLE_COPY(QTdChatListModel)
    QPointer<QQmlObjectListModel<QTdChat>> m_model;
    PinnedChats m_pinnedChats;
    QPointer<QTdChat> m_currentChat;
    ListMode m_listMode;
    QStringList m_forwardingMessages;
    QPointer<QTdChat> m_forwardedFromChat;

};

#endif // QTDCHATLISTMODEL_H
