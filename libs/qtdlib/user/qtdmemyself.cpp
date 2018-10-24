#include "common/qtdint.h"
#include "../client/qtdclient.h"
#include "qtdmemyself.h"


QTdMeMyself::~QTdMeMyself()
{
}


static QPointer<QTdMeMyself> s_me;
QTdUser *QTdMeMyself::instance()
{
    const qint32 uid = qint32(QTdClient::instance()->getOption("my_id").toInt());
    QTdUser *pUser = QTdUsers::instance()->model()->getByUid(QString::number(uid));
    return pUser;
}

