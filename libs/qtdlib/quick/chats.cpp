#include "chats.h"
#include <QDebug>
#include "chat/qtdchats.h"

Chats::Chats(QObject *parent)
    : QObject(parent)
{
    emit modelChanged();
}

QObject *Chats::model() const
{
    return QTdChats::instance()->model();
}
