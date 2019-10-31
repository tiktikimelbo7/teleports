#include "qtdmessagepinmessage.h"
#include <QDebug>
#include <QJsonArray>
#include <QScopedPointer>
#include "user/requests/qtdgetuserrequest.h"
#include "utils/await.h"
#include "qtdmessagetext.h"
#include "qtdmessagedate.h"
#include "qtdmessageunreadlabel.h"
#include "qtdmessagesticker.h"
#include "qtdmessagedocument.h"
#include "qtdmessagelocation.h"
#include "messages/requests/qtdgetmessagerequest.h"
#include "utils/i18n.h"

QTdMessagePinMessage::QTdMessagePinMessage(QObject *parent)
    : QTdMessageContent(parent)
    , m_message(Q_NULLPTR)
    , m_messageId(0)
    , m_chatId(0)
{
    setType(MESSAGE_PIN_MESSAGE);
}

QString QTdMessagePinMessage::message() const
{
    return m_messageString;
}

qint64 QTdMessagePinMessage::messageId() const
{
    return m_messageId;
}

void QTdMessagePinMessage::unmarshalJson(const QJsonObject &json)
{
    m_messageId = qint64(json["message_id"].toDouble());
    emit messageIdChanged();
}

QString QTdMessagePinMessage::getMessageString() {
    if (m_message == Q_NULLPTR) {
        qWarning() << "null message!";
        return "";
    }
    switch (m_message->content()->type()) {
    case QTdObject::MESSAGE_TEXT: {
        auto *c = qobject_cast<QTdMessageText *>(m_message->content());
        return "<i>" + c->text()->text().left(10) +"</i>";
    }
    case QTdObject::MESSAGE_STICKER: {
        auto *c = qobject_cast<QTdMessageSticker *>(m_message->content());
        return c->sticker()->emoji() + " " + gettext("Sticker");
    }
    case QTdObject::MESSAGE_CALL: {
        return gettext("Phone call");
    }
    case QTdObject::MESSAGE_AUDIO: {
        return gettext("an audio message");
    }
    case QTdObject::MESSAGE_PHOTO: {
        return gettext("a photo");
    }
    case QTdObject::MESSAGE_DOCUMENT: {
        auto *c = qobject_cast<QTdMessageDocument *>(m_message->content());
        return c->document()->fileName();
    }
    case QTdObject::MESSAGE_LOCATION: {
        return gettext("a location");
    }
    case QTdObject::MESSAGE_VIDEO: {
        return gettext("a video");
    }
    case QTdObject::MESSAGE_VIDEO_NOTE: {
        return gettext("a video note");
    }
    case QTdObject::MESSAGE_VOICE_NOTE: {
        return gettext("a voice note");
    }
    default:
        return gettext("an unknown message %1");
    }
}
void QTdMessagePinMessage::setChatId(const qint64 &chatId)
{
    if (m_message != Q_NULLPTR) {
        return;
    }
    QScopedPointer<QTdGetMessageRequest>
            req(new QTdGetMessageRequest);
    req->setChatId(chatId);
    req->setMessageId(m_messageId);
    QFuture<QTdResponse> resp = req->sendAsync();
    await(resp, 2000);
    if (resp.result().isError()) {
        qWarning() << "Failed to get pinned message with error: " << resp.result().errorString();
        return;
    }
    m_message = new QTdMessage(this);
    m_message->unmarshalJson(resp.result().json());
    m_messageString = getMessageString();
    emit messageChanged();
}
