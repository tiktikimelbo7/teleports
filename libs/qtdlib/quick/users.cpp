#include "users.h"
#include <QDebug>
#include "user/qtdusers.h"

Users::Users(QObject *parent) : QObject(parent)
{
    emit modelChanged();
}

QObject *Users::model() const
{
    return QTdUsers::instance()->model();
}

QTdUser *Users::meMyself() const
{
    return QTdUsers::instance()->meMyself();
}
QTdUser *Users::currentUser() const
{
    return QTdUsers::instance()->currentUser();
}
void Users::setCurrentUser(QTdUser *currentUser)
{
    QTdUsers::instance()->setCurrentUser(currentUser);
    emit currentUserChanged(currentUser);
}
void Users::clearCurrentUser()
{
    qDebug()<<"clear";
    QTdUsers::instance()->clearCurrentUser();
    emit currentUserChanged(Q_NULLPTR);
}
