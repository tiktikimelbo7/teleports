#ifndef QTDMESSAGE_H
#define QTDMESSAGE_H

#include "common/qabstractint64id.h"
#include "qtdmessagecontent.h"
#include "qtdmessagesendingstate.h"
#include "replymarkup/qtdreplymarkup.h"
#include "forwardinfo/qtdmessageforwardinfo.h"
#include "user/qtduser.h"
#include <QDateTime>
#include <QObject>
#include <QPointer>

class QTdMessage : public QAbstractInt64Id
{
    Q_OBJECT
    Q_PROPERTY(QDateTime date READ qmlDate NOTIFY messageChanged)
    Q_PROPERTY(QString senderUserId READ qmlSenderUserId NOTIFY messageChanged)
    Q_PROPERTY(QString chatId READ qmlChatId NOTIFY messageChanged)
    Q_PROPERTY(QTdMessageSendingState *sendingState READ sendingState NOTIFY sendingStateChanged)
    Q_PROPERTY(bool isOutgoing READ isOutgoing NOTIFY messageChanged)
    Q_PROPERTY(bool isEdited READ isEdited WRITE setIsEdited NOTIFY messageChanged)
    Q_PROPERTY(bool canBeEdited READ canBeEdited NOTIFY messageChanged)
    Q_PROPERTY(bool canBeForwarded READ canBeForwarded NOTIFY messageChanged)
    Q_PROPERTY(bool canBeDeletedOnlyForSelf READ canBeDeletedOnlyForSelf NOTIFY messageChanged)
    Q_PROPERTY(bool canBeDeletedForAllUsers READ canBeDeletedForAllUsers NOTIFY messageChanged)
    Q_PROPERTY(bool isChannelPost READ isChannelPost NOTIFY messageChanged)
    Q_PROPERTY(QString views READ views NOTIFY messageChanged)
    Q_PROPERTY(bool containsUnreadMention READ containsUnreadMention NOTIFY messageChanged)
    Q_PROPERTY(QTdMessageContent *content READ content NOTIFY messageChanged)
    Q_PROPERTY(QTdReplyMarkup *replyMarkup READ replyMarkup NOTIFY messageChanged)
    // These aren't original properties of the tdlib message class but we can
    // can make life easier for use in QML.
    Q_PROPERTY(QTdUser *sender READ sender NOTIFY senderChanged)
    // Provide a summary text for display in the chatlist
    Q_PROPERTY(QString summary READ summary NOTIFY senderChanged)
    // Shows if the sender of the previous message is the same as this one.
    Q_PROPERTY(bool sameUserAsPreviousMessage READ sameUserAsPreviousMessage NOTIFY previousSenderChanged)
    // Shows if the sender of the next message is the same as this one.
    Q_PROPERTY(bool sameUserAsNextMessage READ sameUserAsNextMessage NOTIFY nextSenderChanged)
    // Indicates if this message is the first/latest message in the model
    Q_PROPERTY(bool isLatest READ isLatest NOTIFY nextSenderChanged)
    Q_PROPERTY(QString replyToMessageId READ qmlReplyToMessageId NOTIFY messageChanged)
    Q_PROPERTY(QTdMessage * messageRepliedTo READ messageRepliedTo NOTIFY messageRepliedToChanged)
    Q_PROPERTY(bool isReply READ isReply NOTIFY messageChanged)
    Q_PROPERTY(bool isCollapsed READ isCollapsed NOTIFY messageChanged)
    Q_PROPERTY(bool isForwarded READ isForwarded NOTIFY messageChanged)
    Q_PROPERTY(QString forwardedFromDetails READ forwardedFromDetails NOTIFY messageChanged)

public:
    explicit QTdMessage(QObject *parent = nullptr);

    QDateTime qmlDate() const;
    qint32 date() const;
    QString qmlSenderUserId() const;
    qint32 senderUserId() const;
    QString qmlChatId() const;
    qint64 chatId() const;
    QString getSenderInitials() const;

    QTdUser *sender() const;

    void unmarshalJson(const QJsonObject &json);
    void unmarshalUpdateContent(const QJsonObject &content);

    QTdMessageSendingState *sendingState() const;

    bool isOutgoing() const;

    bool isEdited() const;

    void setIsEdited(const bool value);

    bool canBeEdited() const;

    bool canBeForwarded() const;

    bool canBeDeletedOnlyForSelf() const;

    bool canBeDeletedForAllUsers() const;

    bool isChannelPost() const;

    QString views() const;

    void setViews(const qint32 value);

    bool containsUnreadMention() const;

    QTdMessageContent *content() const;

    QTdReplyMarkup *replyMarkup() const;

    bool isForwarded() const;

    QString forwardedFromDetails() const;

    QString summary() const;

    Q_INVOKABLE QString formatDate(const QDateTime &dt);

    bool isValid() const;

    bool sameUserAsPreviousMessage() const;

    void setPreviousSenderId(const qint32 &id);

    bool sameUserAsNextMessage() const;

    void setNextSenderId(const qint32 &id);

    bool isLatest() const;

    bool isCollapsed() const;

    void collapse();

    qint64 replyToMessageId() const;
    QString qmlReplyToMessageId() const;
    bool isReply() const;
    QTdMessage * messageRepliedTo();

signals:
    void messageChanged();
    void senderChanged();
    void sendingStateChanged();
    void previousSenderChanged();
    void nextSenderChanged();
    void messageRepliedToChanged();

private slots:
    void updateSender(const qint32 &senderId);
    void updateSendingState(const QJsonObject &json);
    void handleMessage(const QJsonObject &json);

private:
    qint32 m_date;
    QTdInt32 m_sender_user_id;
    QTdInt64 m_chatId;
    QTdInt64 m_replyToMessageId;
    QPointer<QTdUser> m_sender;
    bool m_waitingForSender;
    QPointer<QTdMessageSendingState> m_sendingState;
    bool m_isOutgoing;
    bool m_isEdited;
    bool m_canBeEdited;
    bool m_canBeForwarded;
    bool m_canBeDeletedOnlyForSelf;
    bool m_canBeDeletedForAllUsers;
    bool m_isChannelPost;
    qint32 m_views;
    bool m_containsUnreadMention;
    QPointer<QTdMessageContent> m_content;
    bool m_isValid;
    QTdInt32 m_previousSender, m_nextSender;
    QPointer<QTdReplyMarkup> m_replyMarkup;
    QString m_forwardedFromDetails;
    QPointer<QTdMessageForwardInfo> m_forwardInfo;
    QPointer<QTdMessage> m_messageRepliedTo;
    bool m_isCollapsed;
};

#endif // QTDMESSAGE_H
