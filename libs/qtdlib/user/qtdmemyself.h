#ifndef QTDMEMYSELF_H
#define QTDMEMYSELF_H

#include "qtduser.h"
#include "qtdusers.h"

class QTdMeMyself : public QTdUser
{
    Q_OBJECT

public:

    ~QTdMeMyself();

    static QTdUser *instance();

private:
    Q_DISABLE_COPY(QTdMeMyself)
};

#endif // QTDMEMYSELF_H
