#include "users.h"
#include <QDebug>
#include "user/qtdusers.h"

Users::Users(QObject *parent) : QObject(parent)
{
    emit modelChanged();
    connect(QTdUsers::instance(), &QTdUsers::meMyselfChanged, this, &Users::meChanged);
}

QObject *Users::model() const
{
    return QTdUsers::instance()->model();
}

QTdUser *Users::meMyself() const
{
    return QTdUsers::instance()->meMyself();
}
