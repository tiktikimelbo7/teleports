#include "messagecontentdelegatemap.h"
#include <qtdlib/common/qabstracttdobject.h>

MessageContentDelegateMap::MessageContentDelegateMap(QObject *parent) : QObject(parent)
{
    m_delegates.insert(QTdObject::MESSAGE_ANIMATION, QStringLiteral("qrc:///delegates/MessageContentAnimation.qml"));
    m_delegates.insert(QTdObject::MESSAGE_AUDIO, QStringLiteral("qrc:///delegates/MessageContentAudio.qml"));
    m_delegates.insert(QTdObject::MESSAGE_DOCUMENT, QStringLiteral("qrc:///delegates/MessageContentDocument.qml"));
    m_delegates.insert(QTdObject::MESSAGE_LOCATION, QStringLiteral("qrc:///delegates/MessageContentLocation.qml"));
    m_delegates.insert(QTdObject::MESSAGE_PHOTO, QStringLiteral("qrc:///delegates/MessageContentPhoto.qml"));
    m_delegates.insert(QTdObject::MESSAGE_STICKER, QStringLiteral("qrc:///delegates/MessageContentSticker.qml"));
    m_delegates.insert(QTdObject::MESSAGE_TEXT, QStringLiteral("qrc:///delegates/MessageContentText.qml"));
    m_delegates.insert(QTdObject::MESSAGE_VIDEO, QStringLiteral("qrc:///delegates/MessageContentVideo.qml"));
    m_delegates.insert(QTdObject::MESSAGE_CALL, QStringLiteral("qrc:///delegates/MessageContentCall.qml"));
    m_delegates.insert(QTdObject::MESSAGE_DATE, QStringLiteral("qrc:///delegates/MessageDateItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_CHAT_JOIN_BY_LINK, QStringLiteral("qrc:///delegates/MessageJoinByLink.qml"));
    m_delegates.insert(QTdObject::MESSAGE_CHAT_ADD_MEMBERS, QStringLiteral("qrc:///delegates/MessageChatAddMembersItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_CUSTOM_SERVICE_ACTION, QStringLiteral("qrc:///delegates/MessageCustomServiceAction.qml"));
    m_delegates.insert(QTdObject::MESSAGE_SCREENSHOT_TAKEN, QStringLiteral("qrc:///delegates/MessageScreenshotTaken.qml"));

    m_default = QStringLiteral("qrc:///delegates/NotImplementedYet.qml");
}

QString MessageContentDelegateMap::findComponent(const int &type) {
    return m_delegates.value(type, m_default);
}
