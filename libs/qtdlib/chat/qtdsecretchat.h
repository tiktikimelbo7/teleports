#ifndef QTDSECRETCHAT_H
#define QTDSECRETCHAT_H

#include <QObject>
#include <QPointer>
#include "qtdchat.h"
#include "qtdsecretchatstate.h"

class QTdSecretChat : public QTdChat
{
    Q_OBJECT
    Q_PROPERTY(QString secretChatId READ qmlSecretChatId NOTIFY secretChatChanged)
    Q_PROPERTY(QString userId READ qmlUserId NOTIFY secretChatChanged)
    Q_PROPERTY(QTdSecretChatState *state READ state NOTIFY stateChanged)
    Q_PROPERTY(bool isOutbound READ isOutbound NOTIFY secretChatChanged)
    Q_PROPERTY(qint32 ttl READ ttl NOTIFY secretChatChanged)
    Q_PROPERTY(QString keyHash READ keyHash NOTIFY secretChatChanged)
    Q_PROPERTY(qint32 layer READ layer NOTIFY secretChatChanged)
    Q_PROPERTY(bool isPending READ isPending NOTIFY stateChanged)
    Q_PROPERTY(bool isClosed READ isClosed NOTIFY stateChanged)

public:
    explicit QTdSecretChat(QObject *parent = nullptr);

    QString qmlSecretChatId() const;
    qint32 secretChatId() const;
    QString qmlUserId() const;
    qint32 userId() const;
    bool isOutbound() const;
    bool isWritable() const override;
    bool isPending() const;
    bool isClosed() const;
    qint32 ttl() const;
    QString keyHash() const;
    qint32 layer() const;

    QTdSecretChatState *state() const;


signals:
    void secretChatChanged();
    void stateChanged(QTdSecretChatState *state);

private:
    virtual void onChatOpened();

private slots:
    void getSecretChatData();
    void updateSecretChat(const QJsonObject &data);

private:
    QTdInt32 m_secretChatId;
    QTdInt32 m_userId;
    bool m_isOutbound;
    qint32 m_ttl;
    QString m_keyHash;
    qint32 m_layer;
    QPointer<QTdSecretChatState> m_state;
};

#endif // QTDSECRETCHAT_H
