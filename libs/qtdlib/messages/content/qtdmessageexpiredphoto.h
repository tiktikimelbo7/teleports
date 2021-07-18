#ifndef QTDMESSAGEEXPIREDPHOTO_H
#define QTDMESSAGEEXPIREDPHOTO_H

#include <QObject>
#include <QScopedPointer>
#include "messages/qtdmessagecontent.h"

class QTdMessageExpiredPhoto : public QTdMessageContent
{
    Q_OBJECT
public:
    explicit QTdMessageExpiredPhoto(QObject *parent = nullptr);

    void unmarshalJson(const QJsonObject &json);

private:
    Q_DISABLE_COPY(QTdMessageExpiredPhoto)
};

#endif // QTDMESSAGEEXPIREDPHOTO_H
