#include "messagedelegatemap.h"
#include <qtdlib/common/qabstracttdobject.h>

MessageDelegateMap::MessageDelegateMap(QObject *parent) : QObject(parent)
{
    m_delegates.insert(QTdObject::MESSAGE_ANIMATION, QStringLiteral("qrc:///delegates/MessageAnimationItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_AUDIO, QStringLiteral("qrc:///delegates/MessageAudioItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_ACTION, QStringLiteral("qrc:///delegates/MessageActionItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_DOCUMENT, QStringLiteral("qrc:///delegates/MessageDocumentItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_HIDDEN, QStringLiteral("qrc:///delegates/MessageHiddenItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_PHOTO, QStringLiteral("qrc:///delegates/MessagePhotoItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_STICKER, QStringLiteral("qrc:///delegates/MessageStickerItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_TEXT, QStringLiteral("qrc:///delegates/MessageTextItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_VIDEO, QStringLiteral("qrc:///delegates/MessageVideoItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_DATE, QStringLiteral("qrc:///delegates/MessageDateItem.qml"));
    m_default = QStringLiteral("qrc:///delegates/NotImplementedYet.qml");
}

QString MessageDelegateMap::findComponent(const int &type) {
    return m_delegates.value(type, m_default);
}
