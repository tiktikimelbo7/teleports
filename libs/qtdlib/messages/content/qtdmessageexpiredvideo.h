#ifndef QTDMESSAGEEXPIREDVIDEO_H
#define QTDMESSAGEEXPIREDVIDEO_H

#include <QObject>
#include <QScopedPointer>
#include "messages/qtdmessagecontent.h"

class QTdMessageExpiredVideo : public QTdMessageContent
{
    Q_OBJECT
public:
    explicit QTdMessageExpiredVideo(QObject *parent = nullptr);

    void unmarshalJson(const QJsonObject &json);

private:
    Q_DISABLE_COPY(QTdMessageExpiredVideo)
};

#endif // QTDMESSAGEEXPIREDVIDEO_H
