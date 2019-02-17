#include "testutils.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QJsonDocument>

TestUtils::TestUtils(QObject *parent) : QObject(parent)
{

}

QTdChat *TestUtils::getChat()
{
    const QJsonObject json = getChatJson();
    return getChat(json);
}

QTdChat *TestUtils::getChat(const QString &id)
{
    const QJsonObject json = getChatJson(id);
    return getChat(json);
}

QTdChat *TestUtils::getChat(const QJsonObject &json)
{
    QTdChat *chat = new QTdChat(this);
    chat->unmarshalJson(json);
    return chat;
}

QList<QJsonObject> TestUtils::getChatsJson()
{
    QList<QJsonObject> data;
    QDir dir(":/assets/chat");
    for(const QString &file : dir.entryList()) {
        data << getChatJson(QString(":/assets/chat/%1").arg(file));
    }
    return data;
}

QJsonObject TestUtils::getChatJson()
{
    QDir dir(":/assets/chat");
    return loadJsonData(QString(":/assets/chat/%1").arg(dir.entryList().first()));
}

QJsonObject TestUtils::getChatJson(const QString &id)
{
    QDir dir(":/assets/chat");
    for (const QString &chat : dir.entryList()) {
        if (chat.split('.').first() == id) {
            return loadJsonData(QString(":/assets/chat/%1").arg(chat));
        }
    }
    return QJsonObject();
}

QTdMessage *TestUtils::getMessage()
{
    const QJsonObject json = getMessageJson();
    return getMessage(json);
}

QTdMessage *TestUtils::getMessage(const QString &chat_id, const QString &msg_id)
{
    const QJsonObject json = getMessageJson(chat_id, msg_id);
    return getMessage(json);
}

QTdMessage *TestUtils::getMessage(const QJsonObject &json)
{
    QTdMessage *msg = new QTdMessage(this);
    msg->unmarshalJson(json);
    return msg;
}

QJsonObject TestUtils::getMessageJson()
{
    QDir dir(":/assets/message");
    return loadJsonData(QString(":/assets/message/%1").arg(dir.entryList().first()));
}

QJsonObject TestUtils::getMessageJson(const QString &chat_id, const QString &msg_id)
{
    const QString msg = QString("%1_%2.json").arg(chat_id, msg_id);
    QDir dir(":/assets/message");
    for (const QString &message : dir.entryList()) {
        if (message == msg) {
            return loadJsonData(QString(":/assets/message/%1").arg(message));
        }
    }
    return QJsonObject();
}

QTdUser *TestUtils::getUser()
{
    const QJsonObject json = getUserJson();
    return getUser(json);
}

QTdUser *TestUtils::getUser(const QString &id)
{
    const QJsonObject json = getUserJson(id);
    return getUser(json);
}

QTdUser *TestUtils::getUser(const QJsonObject &json)
{
    QTdUser *user = new QTdUser(this);
    user->unmarshalJson(json);
    return user;
}

QJsonObject TestUtils::getUserJson()
{
    QDir dir(":/assets/user");
    return getChatJson(QString(":/assets/user/%1").arg(dir.entryList().first()));
}

QJsonObject TestUtils::getUserJson(const QString &id)
{
    QDir dir(":/assets/users");
    for (const QString &user : dir.entryList()) {
        if (user.split('.').first() == id) {
            return loadJsonData(QString(":/assets/user/%1").arg(user));
        }
    }
    return QJsonObject();
}

QJsonObject TestUtils::loadJsonData(const QString &path)
{
    QFileInfo f(path);
    if (!f.exists()) {
        qWarning() << "Failed to read test asset: " << path;
        return QJsonObject();
    }

    QFile file;
    file.setFileName(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    const QByteArray &json = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(json);
    return doc.object();
}
