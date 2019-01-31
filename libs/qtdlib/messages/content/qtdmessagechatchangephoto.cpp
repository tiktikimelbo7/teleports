#include "qtdmessagechatchangephoto.h"

QTdMessageChatChangePhoto::QTdMessageChatChangePhoto(QObject *parent) : QTdMessageContent(parent),
    m_photo(new QTdPhoto)
{
    setType(MESSAGE_CHAT_CHANGE_PHOTO);
}

QTdPhoto *QTdMessageChatChangePhoto::photo() const
{
    return m_photo.data();
}

void QTdMessageChatChangePhoto::unmarshalJson(const QJsonObject &json)
{
    if (json.contains("photo")) {
        m_photo->unmarshalJson(json["photo"].toObject());
        emit contentChanged();
    }
}
