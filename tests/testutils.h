#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <QObject>
#include <QFile>
#include <QJsonObject>
#include "chat/qtdchat.h"

class TestUtils : public QObject
{
    Q_OBJECT
public:
    explicit TestUtils(QObject *parent = nullptr);

    // Get's the first chat in :/assets/chats
    Q_INVOKABLE QTdChat *getChat();
    // Loads a chat by it's id from :/assets/chats
    QTdChat *getChat(const QString &id);
    // Loads a chat from a it's json data
    QTdChat *getChat(const QJsonObject &json);
    // Loads all chats in :/assets/chats and returns list of QJsonObject
    QList<QJsonObject> getChatsJson();
    // Gets the first chat's json data in :/assets/chats
    Q_INVOKABLE QJsonObject getChatJson();
    // Loads chat json data from :/assets/chats
    QJsonObject getChatJson(const QString &id);

    Q_INVOKABLE QTdMessage *getMessage();

    QTdMessage *getMessage(const QString &chat_id, const QString &msg_id);

    QTdMessage *getMessage(const QJsonObject &json);

    QJsonObject getMessageJson();

    QJsonObject getMessageJson(const QString &chat_id, const QString &msg_id);

    Q_INVOKABLE QTdUser *getUser();

    QTdUser *getUser(const QString &id);

    QTdUser *getUser(const QJsonObject &json);

    QJsonObject getUserJson();

    QJsonObject getUserJson(const QString &id);

private:
    QJsonObject loadJsonData(const QString &path);
};


#endif // TESTUTILS_H
