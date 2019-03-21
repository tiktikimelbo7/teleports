#include "qtdinputmessagecontentfactory.h"
#include <QDebug> // TEMP
// #include "content/qtdinputMessageAnimation.h"
// #include "content/qtdinputMessageAudio.h"
// #include "content/qtdinputMessageContact.h"
// #include "content/qtdinputMessageDocument.h"
// #include "content/qtdinputMessageForwarded.h"
// #include "content/qtdinputMessageGame.h"
// #include "content/qtdinputMessageInvoice.h"
// #include "content/qtdinputMessageLocation.h"
#include "content/qtdinputmessagephoto.h"
// #include "content/inputMessageSticker.h"
#include "content/qtdinputmessagetext.h"
// #include "content/inputMessageVenue.h"
// #include "content/inputMessageVideo.h"
// #include "content/inputMessageVideoNote.h"
// #include "content/inputMessageVoiceNote..h"

// QTdInputMessageContent *QTdInputMessageContentFactory::create(const QJsonObject &json, QObject *parent)
// {
//     const QString type = json["@type"].toString();
//
//     // TODO: create a map of QMap<@type, QTdObject::Type> so we can do fast lookups and
//     // switch on the type. Otherwise this is/elseif is going to get huge supporting all content
//     // types
//     if (type == "messageText") {
//         return new QTdMessageText(parent);
//     } else if (type == "messageSticker") {
//         return new QTdMessageSticker(parent);
//     } else if (type == "messagePhoto") {
//           return new QTdMessagePhoto(parent);
//     } else if (type == "messageAnimation") {
//           return new QTdMessageAnimation(parent);
//     } else if (type == "messageAudio") {
//           return new QTdMessageAudio(parent);
//     } else if (type == "messageDocument") {
//           return new QTdMessageDocument(parent);
//     } else if (type == "messageVideo") {
//           return new QTdMessageVideo(parent);
//     } else if (type == "messageContactRegistered"){
//         return new QTdMessageAction(parent);
//     } else if (type =="messageChatJoinByLink"){
//         return new QTdMessageHidden(parent);
//     } else if (type == "messageBasicGroupChatCreate") {
//         return new QTdMessageBasicGroupChatCreate(parent);
//     } else if (type == "messageCall") {
//         return new QTdMessageCall(parent);
//     } else if (type == "messageChatAddMembers") {
//         return new QTdMessageChatAddMembers(parent);
//     } else if (type == "messageChatChangePhoto") {
//         return new QTdMessageChatChangePhoto(parent);
//     } else if (type == "messageChatChangeTitle") {
//         return new QTdMessageChatChangeTitle(parent);
//     } else if (type == "messageChatDeleteMember") {
//         return new QTdMessageChatDeleteMember(parent);
//     } else if (type == "messageChatDeletePhoto") {
//         return new QTdMessageChatDeletePhoto(parent);
//     }
//     qDebug()<< "Message type "<< type << json;
//     return new QTdMessageContent(parent);
// }
