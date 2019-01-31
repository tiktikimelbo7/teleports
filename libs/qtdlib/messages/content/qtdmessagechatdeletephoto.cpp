#include "qtdmessagechatdeletephoto.h"

QTdMessageChatDeletePhoto::QTdMessageChatDeletePhoto(QObject *parent) : QTdMessageContent(parent)
{
    setType(MESSAGE_CHAT_DELETE_PHOTO);
}
