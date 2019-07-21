#include "qtdmessagecontentfactory.h"
#include <QDebug> // TEMP
#include "content/qtdmessageanimation.h"
#include "content/qtdmessageaudio.h"
#include "content/qtdmessagebasicgroupchatcreate.h"
#include "content/qtdmessagecall.h"
#include "content/qtdmessagechataddmembers.h"
#include "content/qtdmessagechatchangephoto.h"
#include "content/qtdmessagechangechattitle.h"
#include "content/qtdmessagechatdeletemember.h"
#include "content/qtdmessagechatdeletephoto.h"
#include "content/qtdmessagechatjoinbylink.h"
#include "content/qtdmessagechatsetttl.h"
#include "content/qtdmessagechatupgradefrom.h"
#include "content/qtdmessagechatupgradeto.h"
#include "content/qtdmessagedocument.h"
#include "content/qtdmessagelocation.h"
#include "content/qtdmessagephoto.h"
#include "content/qtdmessagesticker.h"
#include "content/qtdmessagetext.h"
#include "content/qtdmessagevideo.h"

QTdMessageContent *QTdMessageContentFactory::create(const QJsonObject &json, QObject *parent)
{
    const QString type = json["@type"].toString();

    // TODO: create a map of QMap<@type, QTdObject::Type> so we can do fast lookups and
    // switch on the type. Otherwise this is/elseif is going to get huge supporting all content
    // types
    if (type == "messageText") {
        return new QTdMessageText(parent);
    } else if (type == "messageSticker") {
        return new QTdMessageSticker(parent);
    } else if (type == "messagePhoto") {
          return new QTdMessagePhoto(parent);
    } else if (type == "messageAnimation") {
          return new QTdMessageAnimation(parent);
    } else if (type == "messageAudio") {
          return new QTdMessageAudio(parent);
    } else if (type == "messageDocument") {
          return new QTdMessageDocument(parent);
    } else if (type == "messageLocation") {
        return new QTdMessageLocation(parent);
    } else if (type == "messageVideo") {
          return new QTdMessageVideo(parent);
    } else if (type == "messageContactRegistered"){
        return new QTdMessageAction(parent);
    } else if (type =="messageChatJoinByLink"){
        return new QTdMessageChatJoinByLink(parent);
    } else if (type == "messageBasicGroupChatCreate") {
        return new QTdMessageBasicGroupChatCreate(parent);
    } else if (type == "messageCall") {
        return new QTdMessageCall(parent);
    } else if (type == "messageChatAddMembers") {
        return new QTdMessageChatAddMembers(parent);
    } else if (type == "messageChatChangePhoto") {
        return new QTdMessageChatChangePhoto(parent);
    } else if (type == "messageChatChangeTitle") {
        return new QTdMessageChatChangeTitle(parent);
    } else if (type == "messageChatDeleteMember") {
        return new QTdMessageChatDeleteMember(parent);
    } else if (type == "messageChatDeletePhoto") {
        return new QTdMessageChatDeletePhoto(parent);
    } else if (type == "messageChatSetTtl") {
        return new QTdMessageChatSetTTL(parent);
    } else if (type == "messageChatUgradeFrom") {
        return new QTdMessageChatUpgradeFrom(parent);
    } else if (type == "messageChatUgradeTo") {
        return new QTdMessageChatUpgradeTo(parent);
    }
    qDebug()<< "Message type "<< type << json;
    return new QTdMessageContent(parent);
}
