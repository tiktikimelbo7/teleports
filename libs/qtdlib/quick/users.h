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
    Q_PROPERTY(QTdUser* currentUser READ currentUser WRITE setCurrentUser NOTIFY currentUserChanged)
public slots:
    void setCurrentUser(QTdUser* currentUser);
    void clearCurrentUser();

public:
    explicit Users(QObject *parent = nullptr);

    QObject* model() const;

    QTdUser* meMyself() const;
    QTdUser* currentUser() const;
signals:
    void modelChanged();
    void currentUserChanged(QTdUser* currentUser);

};

#endif // USERS_H
