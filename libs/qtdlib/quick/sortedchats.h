#ifndef SORTEDCHATS_H
#define SORTEDCHATS_H

#include <QObject>
#include <QJsonObject>
#include "../chat/qtdchat.h"

class SortedChats : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* model READ model NOTIFY modelChanged)
    Q_PROPERTY(int chatFilters READ chatFilters WRITE setChatFilters NOTIFY chatFiltersChanged)
    Q_PROPERTY(bool filterBarVisible READ filterBarVisible NOTIFY filterBarVisibleChanged)
public:
    explicit SortedChats(QObject *parent = nullptr);

    QObject* model() const;

signals:
    void modelChanged();
    void chatFiltersChanged(int chatFilters);
    void filterBarVisibleChanged();
};

#endif // SORTEDCHATS_H
