#include "messagedelegatemap.h"
#include <qtdlib/common/qabstracttdobject.h>

MessageDelegateMap::MessageDelegateMap(QObject *parent) : QObject(parent)
{
    m_delegates.insert(QTdObject::MESSAGE_ANIMATION, QStringLiteral("qrc:///delegates/MessageAnimationItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_AUDIO, QStringLiteral("qrc:///delegates/MessageAudioItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_ACTION, QStringLiteral("qrc:///delegates/MessageActionItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_CONTACT, QStringLiteral("qrc:///delegates/MessageContactItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_DOCUMENT, QStringLiteral("qrc:///delegates/MessageDocumentItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_LOCATION, QStringLiteral("qrc:///delegates/MessageLocationItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_HIDDEN, QStringLiteral("qrc:///delegates/MessageHiddenItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_PHOTO, QStringLiteral("qrc:///delegates/MessagePhotoItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_STICKER, QStringLiteral("qrc:///delegates/MessageStickerItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_TEXT, QStringLiteral("qrc:///delegates/MessageTextItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_VIDEO, QStringLiteral("qrc:///delegates/MessageVideoItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_DATE, QStringLiteral("qrc:///delegates/MessageDateItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_CHAT_JOIN_BY_LINK, QStringLiteral("qrc:///delegates/MessageJoinByLink.qml"));
    m_delegates.insert(QTdObject::MESSAGE_CHAT_ADD_MEMBERS, QStringLiteral("qrc:///delegates/MessageChatAddMembersItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_CALL, QStringLiteral("qrc:///delegates/MessageCallItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_CUSTOM_SERVICE_ACTION, QStringLiteral("qrc:///delegates/MessageCustomServiceAction.qml"));
    m_delegates.insert(QTdObject::MESSAGE_BASIC_GROUP_CHAT_CREATE, QStringLiteral("qrc:///delegates/MessageBasicGroupChatCreate.qml"));
    m_delegates.insert(QTdObject::MESSAGE_CHAT_DELETE_MEMBER, QStringLiteral("qrc:///delegates/MessageChatDeleteMember.qml"));

    // TODO: implement delegates
    m_delegates.insert(QTdObject::MESSAGE_CHAT_CHANGE_PHOTO, QStringLiteral("qrc:///delegates/NotImplementedYet.qml"));
    m_delegates.insert(QTdObject::MESSAGE_CHAT_CHANGE_TITLE, QStringLiteral("qrc:///delegates/NotImplementedYet.qml"));
    m_delegates.insert(QTdObject::MESSAGE_CHAT_DELETE_PHOTO, QStringLiteral("qrc:///delegates/NotImplementedYet.qml"));
    m_delegates.insert(QTdObject::MESSAGE_CHAT_SET_TTL, QStringLiteral("qrc:///delegates/NotImplementedYet.qml"));
    m_delegates.insert(QTdObject::MESSAGE_CHAT_UPGRADE_FROM, QStringLiteral("qrc:///delegates/NotImplementedYet.qml"));
    m_delegates.insert(QTdObject::MESSAGE_CHAT_UPGRADE_TO, QStringLiteral("qrc:///delegates/NotImplementedYet.qml"));
    m_default = QStringLiteral("qrc:///delegates/NotImplementedYet.qml");
}

QString MessageDelegateMap::findComponent(const int &type) {
    return m_delegates.value(type, m_default);
}
