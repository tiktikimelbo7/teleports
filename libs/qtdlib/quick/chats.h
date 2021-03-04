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
    Q_PROPERTY(QTdChat::ListMode listMode READ listMode WRITE setListMode NOTIFY listModeChanged)
    Q_PROPERTY(qint32 forwardingMessagesCount READ forwardingMessagesCount NOTIFY modelChanged)
    Q_PROPERTY(QTdChat *forwardedFromChat READ forwardedFromChat WRITE setForwardedFromChat NOTIFY modelChanged)
    Q_PROPERTY(QStringList forwardingMessages READ forwardingMessages WRITE setForwardingMessages NOTIFY modelChanged)

public:

    explicit Chats(QObject *parent = nullptr);

    QObject* model() const;

signals:
    void modelChanged();
    void currentChatChanged();
    void listModeChanged();
};

#endif // CHATS_H
