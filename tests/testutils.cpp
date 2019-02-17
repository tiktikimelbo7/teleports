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

QTdChat *TestUtils::getChat(const QString &path)
{
    const QJsonObject json = getChatJson(path);
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
    QDir dir(":/assets/chats");
    for(const QString &file : dir.entryList()) {
        data << getChatJson(QString(":/assets/chat/%1").arg(file));
    }
    return data;
}

QJsonObject TestUtils::getChatJson()
{
    QDir dir(":/assets/chats");
    return getChatJson(QString(":/assets/chat/%1").arg(dir.entryList().first()));
}

QJsonObject TestUtils::getChatJson(const QString &path)
{
    return loadJsonData(path);
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
