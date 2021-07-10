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
    return QTdChats::instance()->sortedModel();
}

int SortedChats::chatFilters() const
{
    return QTdChats::instance()->chatFilters();
}

bool SortedChats::filterBarVisible() const
{
    return QTdChats::instance()->filterBarVisible();
}

void SortedChats::setChatFilters(int chatFilters)
{
    QTdChats::instance()->setChatFilters(chatFilters);
}

void SortedChats::setChatNameFilter(const QString &nameFilter)
{
    QTdChats::instance()->setChatNameFilter(nameFilter);
}

void SortedChats::toggleFilterBar(const bool &value)
{
    QTdChats::instance()->toggleFilterBar(value);
}
