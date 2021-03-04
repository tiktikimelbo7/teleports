#ifndef SORTEDCHATS_H
#define SORTEDCHATS_H

#include <QObject>
#include <QJsonObject>
#include "../chat/qtdchat.h"

class SortedChats : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* model READ model NOTIFY modelChanged)
public:
    explicit SortedChats(QObject *parent = nullptr);

    QObject* model() const;

signals:
    void modelChanged();
};

#endif // SORTEDCHATS_H
