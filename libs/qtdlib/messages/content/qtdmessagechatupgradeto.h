#ifndef QTDMESSAGECHATUPGRADETO_H
#define QTDMESSAGECHATUPGRADETO_H

#include <QObject>
#include "messages/qtdmessagecontent.h"
#include "chat/qtdsupergroupchat.h"

class QTdMessageChatUpgradeTo : public QTdMessageContent
{
    Q_OBJECT
    Q_DISABLE_COPY(QTdMessageChatUpgradeTo)
    Q_PROPERTY(QString superGroupId READ qmlSuperGroupId NOTIFY contentChanged)
    Q_PROPERTY(QTdSuperGroupChat* superGroup READ superGroup NOTIFY contentChanged)
public:
    explicit QTdMessageChatUpgradeTo(QObject *parent = nullptr);

    QString qmlSuperGroupId() const;

    qint32 superGroupId() const;

    QTdSuperGroupChat *superGroup() const;

    void unmarshalJson(const QJsonObject &json);

signals:
    void contentChanged();

private:
    QTdInt32 m_sgId;
    QScopedPointer<QTdSuperGroupChat> m_sg;
};

#endif // QTDMESSAGECHATUPGRADETO_H
