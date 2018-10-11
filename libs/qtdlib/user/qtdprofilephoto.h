#ifndef QTDPROFILEPHOTO_H
#define QTDPROFILEPHOTO_H

#include <QObject>

#include "common/qabstractint64id.h"
#include "files/qtdphoto.h"

class QTdProfilePhoto : public QTdPhoto
{
    Q_OBJECT
    Q_DISABLE_COPY(QTdProfilePhoto)
public:
    explicit QTdProfilePhoto(QObject *parent = nullptr);
};

#endif // QTDPROFILEPHOTO_H
