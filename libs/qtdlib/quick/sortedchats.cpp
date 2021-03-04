#include "sortedchats.h"
#include <QDebug>
#include "chat/qtdchats.h"

SortedChats::SortedChats(QObject *parent)
    : QObject(parent)
{
    emit modelChanged();
}

QObject *SortedChats::model() const
{
    return QTdChats::instance()->sortedmodel();
}