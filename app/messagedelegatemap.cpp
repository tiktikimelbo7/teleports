#include "messagedelegatemap.h"
#include <qtdlib/common/qabstracttdobject.h>

MessageDelegateMap::MessageDelegateMap(QObject *parent) : QObject(parent)
{
    m_delegates.insert(QTdObject::MESSAGE_ANIMATION, QStringLiteral("qrc:///delegates/MessageAnimationItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_ACTION, QStringLiteral("qrc:///delegates/MessageActionItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_HIDDEN, QStringLiteral("qrc:///delegates/MessageHiddenItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_STICKER, QStringLiteral("qrc:///delegates/MessageStickerItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_TEXT, QStringLiteral("qrc:///delegates/MessageTextItem.qml"));
    m_delegates.insert(QTdObject::MESSAGE_PHOTO, QStringLiteral("qrc:///delegates/MessagePhotoItem.qml"));
    m_default = QStringLiteral("qrc:///delegates/NotImplementedYet.qml");
}

QString MessageDelegateMap::findComponent(const int &type) {
    return m_delegates.value(type, m_default);
}
