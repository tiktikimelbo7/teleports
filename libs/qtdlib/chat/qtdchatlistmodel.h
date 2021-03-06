#ifndef QTDCHATLISTMODEL_H
#define QTDCHATLISTMODEL_H

#include <QObject>
#include <QPointer>
#include <QTimer>
#include <QtPositioning/QGeoPositionInfoSource>
#include "auth/qtdauthstate.h"
#include "models/QmlObjectListModel.h"
#include "qtdchat.h"
#include "chat/qtdchatinvitelinkinfo.h"

typedef QList<qint64> PinnedChats;

class QTdChatListModel : public QObject
{

    Q_OBJECT
    Q_PROPERTY(QObject *model READ model NOTIFY modelChanged)
    Q_PROPERTY(QTdChat *currentChat READ currentChat WRITE setCurrentChat NOTIFY currentChatChanged)
    Q_PROPERTY(ListMode listMode READ listMode WRITE setListMode NOTIFY listModeChanged)
    Q_PROPERTY(qint32 forwardingMessagesCount READ forwardingMessagesCount NOTIFY modelChanged)
    Q_PROPERTY(QTdChat *forwardedFromChat READ forwardedFromChat WRITE setForwardedFromChat NOTIFY modelChanged)
    Q_PROPERTY(QStringList forwardingMessages READ forwardingMessages WRITE setForwardingMessages NOTIFY modelChanged)

public:
    enum ListMode { Idle,
                    ForwardingMessages,
                    ImportingAttachments };
    Q_ENUM(ListMode)

    explicit QTdChatListModel(QObject *parent = nullptr);

    static QTdChatListModel *instance();

    QObject *model() const;
    QTdChat *currentChat() const;
    QTdChat *chatById(const qint64 &chatId) const;
    QTdChat *forwardedFromChat() const;
    qint32 forwardingMessagesCount() const;
    ListMode listMode() const;
    QStringList forwardingMessages() const;

public slots:
    void setCurrentChat(QTdChat *currentChat);
    void setCurrentChatById(const qint64 &chatId);
    void setCurrentChatByUsername(const QString &username);
    void createOrOpenPrivateChat(const qint64 &userId);
    void createOrOpenSavedMessages();
    void createOrOpenSecretChat(const qint64 &userId);
    void setForwardedFromChat(QTdChat *currentChat);
    void setForwardingMessages(QStringList forwardingMessages);
    void setListMode(ListMode listMode);
    void sendForwardMessage(const QStringList &forwardMessageIds,
                            const qint64 &recievingChatId,
                            const qint64 &fromChatId,
                            const QString &message);
    void requestPositionInfo();
    void cancelPositionInfo();
    void setChatDraftMessage(const QString &draftText,
                             const qint64 &replyToMessageId,
                             const qint64 &chatId);
    void joinChat(const qint64 &chatId) const;
    void checkChatInviteLink(const QString &inviteLink);
    void joinChatByInviteLink(const QString &inviteLink);
    void setChatToOpenOnUpdate(const qint64 &chatId);
    void openMessageContent(const QString chatId, const QString messageId);

    void loadMoreChats(const QString &chatList);

    void sendSetTTL(const QString &chatId, const int &ttl);
signals:
    void modelChanged(QObject *model);
    void contentsChanged();
    void chatCreated(const qint64 &chatId);
    void chatStatusChanged();
    void currentChatChanged();
    void listModeChanged();
    void positionInfoReceived(double latitude, double longitude);
    void positionInfoTimeout();
    void invalidChatUsername(const QString &username);
    void modelPopulatedCompleted();
    void showChatInviteLinkInfo(QTdChatInviteLinkInfo *info, const QString &inviteLink);

private slots:
    void handleChats(const QJsonObject &data);
    void handleChat(const QJsonObject &data);
    void handleUpdateNewChat(const QJsonObject &data);
    void handleUpdateChatPosition(const QJsonObject &data);
    void handleUpdateChatLastMessage(const QJsonObject &data);
    void handleAuthStateChanges(const QTdAuthState *state);
    void updateChatReadInbox(const QJsonObject &data);
    void updateChatReadOutbox(const QJsonObject &data);
    void handleUpdateChatPhoto(const QJsonObject &data);
    void handleUpdateChatReplyMarkup(const QJsonObject &data);
    void handleUpdateChatDraftMessage(const QJsonObject &data);
    void handleUpdateChatTitle(const QJsonObject &data);
    void handleUpdateChatUnreadMentionCount(const QJsonObject &data);
    void handleUpdateChatNotificationSettings(const QJsonObject &data);
    void handleUpdateChatOnlineMemberCount(const QJsonObject &data);
    void handleForwardingMessagesAction();
    void positionUpdated(const QGeoPositionInfo &info);
    void onPositionInfoTimeout();

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
    static QPointer<QTdChatListModel> s_chatlistmodel;
    PinnedChats m_pinnedChats;
    QPointer<QTdChat> m_currentChat;
    qint64 m_chatToOpenOnUpdate;
    ListMode m_listMode;
    QStringList m_forwardingMessages;
    QPointer<QTdChat> m_forwardedFromChat;
    QList<qint64> m_receivedChatIds;
    QPointer<QGeoPositionInfoSource> m_positionInfoSource;
    QTimer *m_positionWaitTimer;
};

#endif // QTDCHATLISTMODEL_H
