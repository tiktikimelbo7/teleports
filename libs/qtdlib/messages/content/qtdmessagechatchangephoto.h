#ifndef QTDMESSAGECHATCHANGEPHOTO_H
#define QTDMESSAGECHATCHANGEPHOTO_H

#include <QObject>
#include <QScopedPointer>
#include "files/qtdphoto.h"
#include "messages/qtdmessagecontent.h"

/**
 * @brief The QTdMessageChatChangePhoto class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1message_chat_change_photo.html
 */
class QTdMessageChatChangePhoto : public QTdMessageContent
{
    Q_OBJECT
    Q_PROPERTY(QTdPhoto* photo READ photo NOTIFY contentChanged)
public:
    explicit QTdMessageChatChangePhoto(QObject *parent = nullptr);

    QTdPhoto *photo() const;

    void unmarshalJson(const QJsonObject &json);

signals:
    void contentChanged();

private:
    QScopedPointer<QTdPhoto> m_photo;
};

#endif // QTDMESSAGECHATCHANGEPHOTO_H
