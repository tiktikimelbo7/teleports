#include "chats.h"
#include <QDebug>
#include "chat/qtdchats.h"

Chats::Chats(QObject *parent)
    : QObject(parent)
{
    emit modelChanged();
}

QObject *Chats::model() const
{
    return QTdChats::instance()->model();
}

void Chats::createOrOpenPrivateChat(const int &userId)
{
    QTdChats::instance()->createOrOpenPrivateChat(userId);
}

QStringList Chats::forwardingMessages() const
{
    return QTdChats::instance()->forwardingMessages();
}

QTdChat *Chats::currentChat() const
{
    return QTdChats::instance()->currentChat();
}

QTdChats::ListMode Chats::listMode() const
{
    return QTdChats::instance()->listMode();
}

void Chats::cancelPositionInfo()
{
    QTdChats::instance()->cancelPositionInfo();
}

QTdChat *Chats::forwardedFromChat() const
{
    return QTdChats::instance()->forwardedFromChat();
}

void Chats::sendForwardMessage(const QStringList &forwardMessageIds,
                               const qint64 &receivingChatId,
                               const qint64 &fromChatId,
                               const QString &message)
{
    QTdChats::instance()->sendForwardMessage(forwardMessageIds, receivingChatId, fromChatId, message);
}

void Chats::loadMoreChats(const QString &chatList)
{
    QTdChats::instance()->loadMoreChats(chatList);
}

void Chats::createOrOpenSavedMessages()
{
    QTdChats::instance()->createOrOpenSavedMessages();
}

void Chats::setForwardedFromChat(QTdChat *currentChat)
{
    QTdChats::instance()->setForwardedFromChat(currentChat);
}

void Chats::setListMode(QTdChats::ListMode listMode)
{
    QTdChats::instance()->setListMode(listMode);
}

void Chats::createOrOpenSecretChat(const int &userId)
{
    QTdChats::instance()->createOrOpenSecretChat(userId);
}

void Chats::setCurrentChat(QTdChat *currentChat)
{
    QTdChats::instance()->setCurrentChat(currentChat);
}

void Chats::joinChat(const qint64 &chatId) const
{
    QTdChats::instance()->joinChat(chatId);
}

void Chats::setChatDraftMessage(const QString &draftText,
                                const qint64 &replyToMessageId,
                                const qint64 &chatId)
{
    QTdChats::instance()->setChatDraftMessage(draftText, replyToMessageId, chatId);
}

void Chats::setCurrentChatByUsername(const QString &userName)
{
    QTdChats::instance()->setCurrentChatByUsername(userName);
}

void Chats::setForwardingMessages(QStringList forwardingMessages)
{
    QTdChats::instance()->setForwardingMessages(forwardingMessages);
}

void Chats::joinChatByInviteLink(const QString &inviteLink)
{
    QTdChats::instance()->joinChatByInviteLink(inviteLink);
}

void Chats::checkChatInviteLink(const QString &inviteLink)
{
    QTdChats::instance()->checkChatInviteLink(inviteLink);
}

void Chats::requestPositionInfo()
{
    QTdChats::instance()->requestPositionInfo();
}
