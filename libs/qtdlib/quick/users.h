#ifndef USERS_H
#define USERS_H

#include <QObject>
#include <QJsonObject>
#include "../user/qtduser.h"

class Users : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* model READ model NOTIFY modelChanged)
    Q_PROPERTY(QTdUser* me READ meMyself)
public:
    explicit Users(QObject *parent = nullptr);

    QObject* model() const;

    QTdUser* meMyself() const;
signals:
    void modelChanged();
};

#endif // USERS_H
