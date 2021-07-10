#ifndef CHATS_H
#define CHATS_H

#include <QObject>
#include <QJsonObject>
#include "../chat/qtdchat.h"
#include "../chat/qtdchats.h"

class Chats : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* model READ model NOTIFY modelChanged)
    Q_PROPERTY(QTdChat *currentChat READ currentChat WRITE setCurrentChat NOTIFY currentChatChanged)
    Q_PROPERTY(QTdChats::ListMode listMode READ listMode WRITE setListMode NOTIFY listModeChanged)
    Q_PROPERTY(QTdChat *forwardedFromChat READ forwardedFromChat WRITE setForwardedFromChat NOTIFY modelChanged)
    Q_PROPERTY(QStringList forwardingMessages READ forwardingMessages WRITE setForwardingMessages NOTIFY modelChanged)

public:

    explicit Chats(QObject *parent = nullptr);

    QObject* model() const;
    QTdChat *forwardedFromChat() const;
    QStringList forwardingMessages() const;
    QTdChat *currentChat() const;
    QTdChats::ListMode listMode() const;

public slots:
    void setCurrentChat(QTdChat *currentChat);
    void setListMode(QTdChats::ListMode listMode);
    void setForwardedFromChat(QTdChat *currentChat);
    void setForwardingMessages(QStringList forwardingMessages);
    void createOrOpenPrivateChat(const int &userId);
    void createOrOpenSavedMessages();
    void createOrOpenSecretChat(const int &userId);
    void setCurrentChatByUsername(const QString &username);
    void loadMoreChats(const QString &chatList);
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

signals:
    void modelChanged(QObject *model);
    void sortedModelChanged(QObject *model);
    void contentsChanged();
    void chatStatusChanged();
    void currentChatChanged();
    void listModeChanged();
    void positionInfoReceived(double latitude, double longitude);
    void positionInfoTimeout();
    void invalidChatUsername(const QString &username);
    void modelPopulatedCompleted();
    void showChatInviteLinkInfo(QTdChatInviteLinkInfo *info, const QString &inviteLink);
};

#endif // CHATS_H
